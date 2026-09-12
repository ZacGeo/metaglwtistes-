#include "regalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Ζωντανό διάστημα μιας προσωρινής τιμής. */
typedef struct Temporary {
    AstNode *node;
    int birth;
    int death;
    int degree;
} Temporary;

/* Κατάσταση συλλογής προσωρινών τιμών. */
typedef struct Builder {
    Temporary *items;
    int count;
    int capacity;
    int position;
} Builder;

/* Τερματίζει αν αποτύχει δέσμευση μνήμης. */
static void die(void)
{
    fputs("Out of memory\n", stderr);
    exit(EXIT_FAILURE);
}

/* Ελέγχει αν ο κόμβος παράγει προσωρινή τιμή. */
static int produces_value(const AstNode *node)
{
    if (!node) return 0;
    switch (node->kind) {
    case AST_IDENTIFIER: case AST_INT: case AST_FLOAT: case AST_CHAR:
    case AST_STRING: case AST_LIST: case AST_UNARY: case AST_BINARY:
    case AST_ASSIGN: case AST_CAST: case AST_INDEX: case AST_MEMBER:
    case AST_CALL: case AST_INIT_LIST:
        return 1;
    default:
        return 0;
    }
}

/* Καταχωρίζει νέα προσωρινή τιμή. */
static int add_temporary(Builder *builder, AstNode *node)
{
    Temporary *temporary;
    if (builder->count == builder->capacity) {
        int capacity = builder->capacity ? builder->capacity * 2 : 64;
        Temporary *items = realloc(builder->items, (size_t)capacity * sizeof(*items));
        if (!items) die();
        builder->items = items;
        builder->capacity = capacity;
    }
    temporary = &builder->items[builder->count];
    temporary->node = node;
    temporary->birth = builder->position;
    temporary->death = builder->position;
    temporary->degree = 0;
    node->temporary_id = builder->count;
    node->register_color = -1;
    node->spill_offset = -1;
    return builder->count++;
}

/* Αριθμεί το AST και υπολογίζει τα ζωντανά διαστήματα. */
static int number_tree(Builder *builder, AstNode *node)
{
    int child_result[4] = { -1, -1, -1, -1 };
    int result = -1;
    int index;
    if (!node) return -1;
    for (index = 0; index < 4; index++)
        child_result[index] = number_tree(builder, node->child[index]);
    builder->position++;
    if (produces_value(node)) result = add_temporary(builder, node);
    for (index = 0; index < 4; index++) {
        int child = child_result[index];
        if (child >= 0 && builder->items[child].death < builder->position)
            builder->items[child].death = builder->position;
    }
    if (node->next) number_tree(builder, node->next);
    return result;
}

/* Ελέγχει αν δύο ζωντανά διαστήματα επικαλύπτονται. */
static int overlaps(const Temporary *left, const Temporary *right)
{
    return left->birth <= right->death && right->birth <= left->death;
}

/* Κατανέμει καταχωρητές και θέσεις spill σε μία συνάρτηση. */
RegisterAllocation regalloc_function(IrFunction *function, long first_spill_offset)
{
    Builder builder = { 0 };
    RegisterAllocation result = { 0 };
    unsigned char *edges;
    int left, right;
    long next_spill = first_spill_offset;
    if (!function || !function->body) return result;
    number_tree(&builder, function->body);
    result.temporary_count = builder.count;
    edges = calloc((size_t)builder.count * (size_t)builder.count, 1);
    if (builder.count && !edges) die();
    for (left = 0; left < builder.count; left++) {
        for (right = left + 1; right < builder.count; right++) {
            if (overlaps(&builder.items[left], &builder.items[right])) {
                edges[(size_t)left * (size_t)builder.count + (size_t)right] = 1;
                edges[(size_t)right * (size_t)builder.count + (size_t)left] = 1;
                builder.items[left].degree++;
                builder.items[right].degree++;
            }
        }
    }
    /* Χρωματίζει άπληστα τον γράφο παρεμβολής. */
    for (left = 0; left < builder.count; left++) {
        unsigned used = 0;
        int color;
        for (right = 0; right < left; right++) {
            int previous_color = builder.items[right].node->register_color;
            if (edges[(size_t)left * (size_t)builder.count + (size_t)right] &&
                previous_color >= 0)
                used |= 1u << previous_color;
        }
        for (color = 0; color < MIPS_TEMP_REGISTER_COUNT; color++)
            if (!(used & (1u << color))) break;
        if (color < MIPS_TEMP_REGISTER_COUNT) {
            builder.items[left].node->register_color = color;
            result.colored_count++;
        } else {
            next_spill = (next_spill + 3) & ~3L;
            builder.items[left].node->spill_offset = next_spill;
            next_spill += 4;
            result.spilled_count++;
        }
    }
    result.spill_bytes = next_spill - first_spill_offset;
    free(edges);
    free(builder.items);
    return result;
}

/* Επιστρέφει τον ακέραιο καταχωρητή ενός χρώματος. */
const char *regalloc_integer_name(int color)
{
    static const char *names[MIPS_TEMP_REGISTER_COUNT] = {
        "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7"
    };
    return color >= 0 && color < MIPS_TEMP_REGISTER_COUNT ? names[color] : "$t8";
}

/* Επιστρέφει τον float καταχωρητή ενός χρώματος. */
const char *regalloc_float_name(int color)
{
    static const char *names[MIPS_TEMP_REGISTER_COUNT] = {
        "$f2", "$f4", "$f6", "$f8", "$f10", "$f12", "$f14", "$f16"
    };
    return color >= 0 && color < MIPS_TEMP_REGISTER_COUNT ? names[color] : "$f18";
}
