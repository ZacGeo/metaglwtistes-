#define _POSIX_C_SOURCE 200809L
#include "codegen.h"
#include "regalloc.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAME_ADMIN_BYTES 80L /* Χώρος διαχείρισης πλαισίου. */

/* Αντιστοίχιση string literal σε ετικέτα. */
typedef struct LiteralLabel {
    const AstNode *node;
    char *label;
    struct LiteralLabel *next;
} LiteralLabel;

/* Αντιστοίχιση μορφής I/O σε ετικέτα. */
typedef struct FormatLabel {
    const AstNode *node;
    char *label;
    char *format;
    struct FormatLabel *next;
} FormatLabel;

/* Κατάσταση παραγωγής κώδικα μιας συνάρτησης. */
typedef struct Context {
    FILE *out;
    IrFunction *function;
    long frame_size;
    long this_offset;
    int label_counter;
    const char *break_labels[64];
    const char *continue_labels[64];
    int loop_depth;
} Context;

/* Λίστες ετικετών και μετρητές μοναδικών ονομάτων. */
static LiteralLabel *literal_labels;
static FormatLabel *format_labels;
static int literal_counter;
static int format_counter;

/* Ευθυγραμμίζει μέγεθος σε όριο τεσσάρων bytes. */
static long align4(long value) { return (value + 3) & ~3L; }

/* Αντιγράφει συμβολοσειρά με έλεγχο μνήμης. */
static char *xstrdup(const char *text)
{
    char *copy = strdup(text ? text : "");
    if (!copy) { fputs("Out of memory\n", stderr); exit(EXIT_FAILURE); }
    return copy;
}

/* Μετατρέπει όνομα CPP σε έγκυρη MIPS ετικέτα. */
static void mangle(char *target, size_t size, const char *name)
{
    size_t used = 0;
    const unsigned char *cursor = (const unsigned char *)(name ? name : "anonymous");
    while (*cursor && used + 2 < size) {
        if (isalnum(*cursor) || *cursor == '_') target[used++] = (char)*cursor;
        else if (*cursor == ':' && cursor[1] == ':') {
            target[used++] = '_'; target[used++] = '_'; cursor++;
        } else target[used++] = '_';
        cursor++;
    }
    target[used] = '\0';
}

/* Διαφεύγει συμβολοσειρά για το τμήμα .data. */
static void escape_string(FILE *out, const char *text)
{
    const unsigned char *cursor = (const unsigned char *)(text ? text : "");
    fputc('"', out);
    while (*cursor) {
        switch (*cursor) {
        case '\n': fputs("\\n", out); break;
        case '\t': fputs("\\t", out); break;
        case '\\': fputs("\\\\", out); break;
        case '"': fputs("\\\"", out); break;
        default:
            if (isprint(*cursor)) fputc(*cursor, out);
            else fprintf(out, "\\%03o", *cursor);
        }
        cursor++;
    }
    fputc('"', out);
}

/* Συλλέγει literals και μορφές I/O από το AST. */
static void collect_literals(AstNode *node)
{
    int index;
    while (node) {
        if (node->kind == AST_OUTPUT || node->kind == AST_INPUT) {
            AstNode *item;
            size_t length = 1;
            char label[48];
            FormatLabel *entry = calloc(1, sizeof(*entry));
            if (!entry) { fputs("Out of memory\n", stderr); exit(EXIT_FAILURE); }
            for (item = node->child[0]; item; item = item->next)
                length += node->kind == AST_INPUT && item->type &&
                          item->type->kind == TYPE_STRING ? 5 : 2;
            entry->format = calloc(length, 1);
            if (!entry->format) { fputs("Out of memory\n", stderr); exit(EXIT_FAILURE); }
            for (item = node->child[0]; item; item = item->next) {
                const char *field = item->type && item->type->kind == TYPE_FLOAT ? "%f" :
                    item->type && item->type->kind == TYPE_STRING ?
                    (node->kind == AST_INPUT ? "%255s" : "%s") :
                    item->type && item->type->kind == TYPE_CHAR ? "%c" : "%d";
                strcat(entry->format, field);
            }
            snprintf(label, sizeof(label), "__iofmt_%d", format_counter++);
            entry->node = node;
            entry->label = xstrdup(label);
            entry->next = format_labels;
            format_labels = entry;
        }
        if (node->kind == AST_STRING || node->constant_kind == CONST_VALUE_STRING) {
            LiteralLabel *entry = calloc(1, sizeof(*entry));
            char label[48];
            if (!entry) { fputs("Out of memory\n", stderr); exit(EXIT_FAILURE); }
            snprintf(label, sizeof(label), "__str_%d", literal_counter++);
            entry->node = node;
            entry->label = xstrdup(label);
            entry->next = literal_labels;
            literal_labels = entry;
        }
        for (index = 0; index < 4; index++) collect_literals(node->child[index]);
        node = node->next;
    }
}

/* Βρίσκει την ετικέτα μορφής ενός κόμβου I/O. */
static const char *format_label(const AstNode *node)
{
    FormatLabel *entry;
    for (entry = format_labels; entry; entry = entry->next)
        if (entry->node == node) return entry->label;
    return "__empty_string";
}

/* Βρίσκει την ετικέτα ενός string literal. */
static const char *literal_label(const AstNode *node)
{
    LiteralLabel *entry;
    for (entry = literal_labels; entry; entry = entry->next)
        if (entry->node == node) return entry->label;
    return "__empty_string";
}

/* Αποδεσμεύει τις προσωρινές λίστες ετικετών. */
static void free_literals(void)
{
    while (literal_labels) {
        LiteralLabel *next = literal_labels->next;
        free(literal_labels->label);
        free(literal_labels);
        literal_labels = next;
    }
    while (format_labels) {
        FormatLabel *next = format_labels->next;
        free(format_labels->label);
        free(format_labels->format);
        free(format_labels);
        format_labels = next;
    }
}

/* Ελέγχει αν ο τύπος είναι float. */
static int is_float_type(const Type *type)
{
    return type && type->kind == TYPE_FLOAT;
}

/* Ελέγχει αν ο τύπος είναι σύνθετος. */
static int is_aggregate_type(const Type *type)
{
    return type && (type->kind == TYPE_STRING || type->kind == TYPE_ARRAY ||
                    type->kind == TYPE_CLASS || type->kind == TYPE_UNION);
}

/* Επιστρέφει τον ακέραιο καταχωρητή του κόμβου. */
static const char *ireg(const AstNode *node)
{
    return regalloc_integer_name(node ? node->register_color : -1);
}

/* Επιστρέφει τον float καταχωρητή του κόμβου. */
static const char *freg(const AstNode *node)
{
    return regalloc_float_name(node ? node->register_color : -1);
}

/* Αποθηκεύει spilled τιμή στη στοίβα. */
static void store_spill(Context *context, AstNode *node, const char *reg)
{
    if (node && node->spill_offset >= 0)
        fprintf(context->out, "    %s %s, %ld($fp)\n",
                is_float_type(node->type) ? "s.s" : "sw", reg, node->spill_offset);
}

/* Φορτώνει τιμή από καταχωρητή ή spill θέση. */
static const char *load_value(Context *context, AstNode *node, const char *scratch)
{
    if (!node) return "$zero";
    if (node->spill_offset >= 0) {
        fprintf(context->out, "    %s %s, %ld($fp)\n",
                is_float_type(node->type) ? "l.s" : "lw", scratch, node->spill_offset);
        return scratch;
    }
    return is_float_type(node->type) ? freg(node) : ireg(node);
}

/* Αποθηκεύει τους προσωρινούς καταχωρητές. */
static void save_volatile_registers(Context *context)
{
    int index;
    for (index = 0; index < MIPS_TEMP_REGISTER_COUNT; index++)
        fprintf(context->out, "    sw $t%d, %ld($fp)\n", index,
                context->frame_size - FRAME_ADMIN_BYTES + index * 4L);
    for (index = 0; index < MIPS_TEMP_REGISTER_COUNT; index++)
        fprintf(context->out, "    s.s %s, %ld($fp)\n", regalloc_float_name(index),
                context->frame_size - 48 + index * 4L);
}

/* Επαναφέρει τους προσωρινούς καταχωρητές. */
static void restore_volatile_registers(Context *context)
{
    int index;
    for (index = 0; index < MIPS_TEMP_REGISTER_COUNT; index++)
        fprintf(context->out, "    lw $t%d, %ld($fp)\n", index,
                context->frame_size - FRAME_ADMIN_BYTES + index * 4L);
    for (index = 0; index < MIPS_TEMP_REGISTER_COUNT; index++)
        fprintf(context->out, "    l.s %s, %ld($fp)\n", regalloc_float_name(index),
                context->frame_size - 48 + index * 4L);
}

/* Παράγει κώδικα για μία έκφραση. */
static void emit_expr(Context *context, AstNode *node);

/* Μετατρέπει μία τιμή σε λογικό 0 ή 1. */
static void emit_boolean_value(Context *context, AstNode *node, const char *target)
{
    const char *value;
    if (is_float_type(node ? node->type : NULL)) {
        char true_label[64], end_label[64];
        value = load_value(context, node, "$f18");
        snprintf(true_label, sizeof(true_label), "__truth_true_%d",
                 context->label_counter++);
        snprintf(end_label, sizeof(end_label), "__truth_end_%d",
                 context->label_counter++);
        fprintf(context->out, "    mtc1 $zero, $f20\n    cvt.s.w $f20, $f20\n"
                "    c.eq.s %s, $f20\n    bc1f %s\n    nop\n"
                "    li %s, 0\n    j %s\n    nop\n%s:\n"
                "    li %s, 1\n%s:\n", value, true_label, target,
                end_label, true_label, target, end_label);
    } else {
        value = load_value(context, node, "$t9");
        fprintf(context->out, "    sne %s, %s, $zero\n", target, value);
    }
}

/* Υπολογίζει τη διεύθυνση μιας lvalue έκφρασης. */
static void emit_address(Context *context, AstNode *node, const char *target)
{
    Symbol *symbol;
    if (!node) { fprintf(context->out, "    move %s, $zero\n", target); return; }
    if (node->kind == AST_IDENTIFIER || node->kind == AST_DECL) {
        symbol = node->symbol;
        if (!symbol && node->text && !strcmp(node->text, "this")) {
            fprintf(context->out, "    lw %s, %ld($fp)\n", target, context->this_offset);
            return;
        }
        if (!symbol) { fprintf(context->out, "    # unresolved address: %s\n", node->text); return; }
        if (symbol->storage == STORAGE_GLOBAL || symbol->storage == STORAGE_STATIC_LOCAL)
            fprintf(context->out, "    addiu %s, $gp, %ld\n", target, symbol->offset);
        else if (symbol->storage == STORAGE_PARAMETER_REFERENCE)
            fprintf(context->out, "    lw %s, %ld($fp)\n", target, symbol->offset);
        else if (symbol->storage == STORAGE_MEMBER) {
            fprintf(context->out, "    lw %s, %ld($fp)\n", target, context->this_offset);
            fprintf(context->out, "    addiu %s, %s, %ld\n", target, target, symbol->offset);
        } else
            fprintf(context->out, "    addiu %s, $fp, %ld\n", target, symbol->offset);
        return;
    }
    if (node->kind == AST_MEMBER) {
        emit_address(context, node->child[0], target);
        fprintf(context->out, "    addiu %s, %s, %ld\n", target, target,
                node->symbol ? node->symbol->offset : 0L);
        return;
    }
    if (node->kind == AST_INDEX) {
        const char *index_register;
        emit_expr(context, node->child[1]);
        index_register = load_value(context, node->child[1], "$t9");
        fprintf(context->out, "    li $v1, %zu\n", node->type ? node->type->size : 4u);
        fprintf(context->out, "    mul $v1, %s, $v1\n", index_register);
        fputs("    addiu $sp, $sp, -4\n    sw $v1, 0($sp)\n", context->out);
        emit_address(context, node->child[0], target);
        fputs("    lw $v1, 0($sp)\n    addiu $sp, $sp, 4\n", context->out);
        fprintf(context->out, "    addu %s, %s, $v1\n", target, target);
        return;
    }
    fprintf(context->out, "    # address requested for %s\n", ast_kind_name(node->kind));
}

/* Παράγει κλήση συνάρτησης ή μεθόδου. */
static void emit_call(Context *context, AstNode *node)
{
    AstNode *actual;
    int argument = 0;
    int actual_count = 0;
    int implicit_this = node->child[0] && node->child[0]->kind == AST_MEMBER;
    int total_arguments;
    int call_area;
    char label[256];
    for (actual = node->child[1]; actual; actual = actual->next) actual_count++;
    total_arguments = actual_count + implicit_this;
    call_area = (total_arguments < 4 ? 4 : total_arguments) * 4;
    fprintf(context->out, "    addiu $sp, $sp, -%d\n", call_area);
    if (implicit_this) {
        emit_address(context, node->child[0]->child[0], "$t9");
        fputs("    sw $t9, 0($sp)\n", context->out);
        argument = 1;
    }
    for (actual = node->child[1]; actual; actual = actual->next, argument++) {
        const char *value;
        if (actual->is_lvalue && node->child[0] && node->child[0]->type &&
            node->child[0]->type->kind == TYPE_FUNCTION) {
            Parameter *formal = node->child[0]->type->parameters;
            int position;
            int formal_index = argument - implicit_this;
            for (position = 0; formal && position < formal_index; position++)
                formal = formal->next;
            if (formal && formal->mode == PASS_REFERENCE) {
                emit_address(context, actual, "$t9");
                fprintf(context->out, "    sw $t9, %d($sp)\n", argument * 4);
                continue;
            }
        }
        emit_expr(context, actual);
        value = load_value(context, actual, is_float_type(actual->type) ? "$f18" : "$t9");
        if (is_float_type(actual->type))
            fprintf(context->out, "    mfc1 $t9, %s\n", value);
        else fprintf(context->out, "    move $t9, %s\n", value);
        fprintf(context->out, "    sw $t9, %d($sp)\n", argument * 4);
    }
    if (implicit_this && node->child[0]->symbol && node->child[0]->symbol->owner_type) {
        char qualified[256];
        snprintf(qualified, sizeof(qualified), "%s::%s",
                 node->child[0]->symbol->owner_type->name,
                 node->child[0]->text ? node->child[0]->text : "method");
        mangle(label, sizeof(label), qualified);
    } else mangle(label, sizeof(label), node->child[0] && node->child[0]->text
                  ? node->child[0]->text : "unknown_function");
    save_volatile_registers(context);
    fputs("    lw $a0, 0($sp)\n    lw $a1, 4($sp)\n"
          "    lw $a2, 8($sp)\n    lw $a3, 12($sp)\n", context->out);
    fprintf(context->out, "    jal %s\n    nop\n", label);
    restore_volatile_registers(context);
    fprintf(context->out, "    addiu $sp, $sp, %d\n", call_area);
    if (is_float_type(node->type)) fprintf(context->out, "    mov.s %s, $f0\n", freg(node));
    else fprintf(context->out, "    move %s, $v0\n", ireg(node));
    store_spill(context, node, is_float_type(node->type) ? freg(node) : ireg(node));
}

/* Παράγει MIPS ανά κατηγορία έκφρασης. */
static void emit_expr(Context *context, AstNode *node)
{
    const char *destination;
    const char *left;
    const char *right;
    if (!node) return;
    destination = is_float_type(node->type) ? freg(node) : ireg(node);
    if (node->is_constant && node->constant_kind != CONST_VALUE_NONE &&
        node->constant_kind != CONST_VALUE_AGGREGATE) {
        if (node->constant_kind == CONST_VALUE_STRING)
            fprintf(context->out, "    la %s, %s\n", destination, literal_label(node));
        else if (node->constant_kind == CONST_VALUE_FLOAT)
            fprintf(context->out, "    li.s %s, %.9g\n", destination, node->float_value);
        else
            fprintf(context->out, "    li %s, %lld\n", destination, node->int_value);
        store_spill(context, node, destination);
        return;
    }
    switch (node->kind) {
    case AST_INT:
        fprintf(context->out, "    li %s, %lld\n", destination, node->int_value); break;
    case AST_FLOAT:
        fprintf(context->out, "    li.s %s, %.9g\n", destination, node->float_value); break;
    case AST_CHAR:
        fprintf(context->out, "    li %s, %d\n", destination,
                node->text && node->text[0] ? (unsigned char)node->text[0] : 0); break;
    case AST_STRING:
        fprintf(context->out, "    la %s, %s\n", destination, literal_label(node)); break;
    case AST_LIST: {
        AstNode *item;
        int count = 0;
        for (item = node->child[0]; item; item = item->next) count++;
        save_volatile_registers(context);
        fprintf(context->out, "    li $a0, %d\n    li $a1, %zu\n"
                "    jal __list_new\n    nop\n",
                count, node->type && node->type->base ? node->type->base->size : 4u);
        restore_volatile_registers(context);
        fprintf(context->out, "    move %s, $v0\n", destination);
        for (item = node->child[0]; item; item = item->next) {
            fprintf(context->out, "    addiu $sp, $sp, -4\n"
                    "    sw %s, 0($sp)\n", destination);
            emit_expr(context, item);
            fputs("    lw $a0, 0($sp)\n", context->out);
            if (is_float_type(item->type))
                fprintf(context->out, "    mfc1 $a1, %s\n",
                        load_value(context, item, "$f18"));
            else fprintf(context->out, "    move $a1, %s\n",
                         load_value(context, item, "$t9"));
            save_volatile_registers(context);
            fputs("    jal __list_push\n    nop\n", context->out);
            restore_volatile_registers(context);
            fprintf(context->out, "    lw %s, 0($sp)\n"
                    "    addiu $sp, $sp, 4\n", destination);
        }
        break;
    }
    case AST_IDENTIFIER:
    case AST_MEMBER:
    case AST_INDEX:
        if (node->is_constant) {
            if (node->constant_kind == CONST_VALUE_STRING)
                fprintf(context->out, "    la %s, %s\n", destination, literal_label(node));
            else if (node->constant_kind == CONST_VALUE_FLOAT)
                fprintf(context->out, "    li.s %s, %.9g\n", destination, node->float_value);
            else fprintf(context->out, "    li %s, %lld\n", destination, node->int_value);
        } else if (is_aggregate_type(node->type)) {
            emit_address(context, node, destination);
        } else {
            emit_address(context, node, "$t8");
            fprintf(context->out, "    %s %s, 0($t8)\n",
                    is_float_type(node->type) ? "l.s" :
                    (node->type && node->type->kind == TYPE_CHAR ? "lb" : "lw"), destination);
        }
        break;
    case AST_CAST:
        emit_expr(context, node->child[0]);
        if (is_float_type(node->type) && !is_float_type(node->child[0]->type)) {
            left = load_value(context, node->child[0], "$t9");
            fprintf(context->out, "    mtc1 %s, %s\n    cvt.s.w %s, %s\n",
                    left, destination, destination, destination);
        } else if (!is_float_type(node->type) && is_float_type(node->child[0]->type)) {
            left = load_value(context, node->child[0], "$f18");
            fprintf(context->out, "    cvt.w.s $f18, %s\n    mfc1 %s, $f18\n", left, destination);
        } else fprintf(context->out, "    move %s, %s\n", destination,
                       load_value(context, node->child[0], "$t9"));
        break;
    case AST_BINARY: {
        int floating_operands;
        int comparison;
        if (!strcmp(node->text, "&&") || !strcmp(node->text, "||")) {
            char shortcut[64], end[64];
            snprintf(shortcut, sizeof(shortcut), "__logic_short_%d",
                     context->label_counter++);
            snprintf(end, sizeof(end), "__logic_end_%d",
                     context->label_counter++);
            emit_expr(context, node->child[0]);
            emit_boolean_value(context, node->child[0], "$v0");
            fprintf(context->out, "    %s $v0, $zero, %s\n    nop\n",
                    !strcmp(node->text, "&&") ? "beq" : "bne", shortcut);
            emit_expr(context, node->child[1]);
            emit_boolean_value(context, node->child[1], destination);
            fprintf(context->out, "    j %s\n    nop\n%s:\n    li %s, %d\n%s:\n",
                    end, shortcut, destination,
                    !strcmp(node->text, "&&") ? 0 : 1, end);
            break;
        }
        emit_expr(context, node->child[0]); emit_expr(context, node->child[1]);
        floating_operands = is_float_type(node->child[0]->type) ||
                            is_float_type(node->child[1]->type);
        comparison = !strcmp(node->text, "==") || !strcmp(node->text, "!=") ||
                     !strcmp(node->text, "<") || !strcmp(node->text, "<=") ||
                     !strcmp(node->text, ">") || !strcmp(node->text, ">=");
        left = load_value(context, node->child[0],
                          is_float_type(node->child[0]->type) ? "$f18" : "$t8");
        right = load_value(context, node->child[1],
                           is_float_type(node->child[1]->type) ? "$f20" : "$t9");
        if (is_float_type(node->type)) {
            const char *instruction = !strcmp(node->text, "+") ? "add.s" :
                !strcmp(node->text, "-") ? "sub.s" : !strcmp(node->text, "*") ? "mul.s" : "div.s";
            fprintf(context->out, "    %s %s, %s, %s\n", instruction, destination, left, right);
        } else if (floating_operands && comparison) {
            char true_label[64], end_label[64];
            const char *condition;
            int branch_on_true = 1;
            if (!strcmp(node->text, "==")) condition = "c.eq.s";
            else if (!strcmp(node->text, "!=")) { condition = "c.eq.s"; branch_on_true = 0; }
            else if (!strcmp(node->text, "<")) condition = "c.lt.s";
            else if (!strcmp(node->text, "<=")) condition = "c.le.s";
            else if (!strcmp(node->text, ">")) {
                condition = "c.lt.s"; { const char *swap = left; left = right; right = swap; }
            } else {
                condition = "c.le.s"; { const char *swap = left; left = right; right = swap; }
            }
            snprintf(true_label, sizeof(true_label), "__fcmp_true_%d", context->label_counter++);
            snprintf(end_label, sizeof(end_label), "__fcmp_end_%d", context->label_counter++);
            fprintf(context->out, "    %s %s, %s\n    %s %s\n    nop\n",
                    condition, left, right, branch_on_true ? "bc1t" : "bc1f", true_label);
            fprintf(context->out, "    li %s, 0\n    j %s\n    nop\n%s:\n"
                    "    li %s, 1\n%s:\n", destination, end_label, true_label,
                    destination, end_label);
        } else if (!strcmp(node->text, "+")) fprintf(context->out, "    addu %s, %s, %s\n", destination, left, right);
        else if (!strcmp(node->text, "-")) fprintf(context->out, "    subu %s, %s, %s\n", destination, left, right);
        else if (!strcmp(node->text, "*")) fprintf(context->out, "    mul %s, %s, %s\n", destination, left, right);
        else if (!strcmp(node->text, "/")) fprintf(context->out, "    div %s, %s\n    mflo %s\n", left, right, destination);
        else if (!strcmp(node->text, "%")) fprintf(context->out, "    div %s, %s\n    mfhi %s\n", left, right, destination);
        else fprintf(context->out, "    s%s %s, %s, %s\n",
                     !strcmp(node->text, "==") ? "eq" : !strcmp(node->text, "!=") ? "ne" :
                     !strcmp(node->text, "<") ? "lt" : !strcmp(node->text, "<=") ? "le" :
                     !strcmp(node->text, ">") ? "gt" : "ge", destination, left, right);
        break;
    }
    case AST_UNARY:
        emit_expr(context, node->child[0]);
        left = load_value(context, node->child[0], is_float_type(node->type) ? "$f18" : "$t9");
        if (!strcmp(node->text, "-")) fprintf(context->out, "    %s %s, %s\n",
            is_float_type(node->type) ? "neg.s" : "negu", destination, left);
        else if (!strcmp(node->text, "!")) {
            emit_boolean_value(context, node->child[0], "$t9");
            fprintf(context->out, "    xori %s, $t9, 1\n", destination);
        }
        else if (!strcmp(node->text, "++") || !strcmp(node->text, "--") ||
                 !strcmp(node->text, "post++") || !strcmp(node->text, "post--")) {
            int increment = strstr(node->text, "++") != NULL;
            int postfix = !strncmp(node->text, "post", 4);
            const char *stored = destination;
            if (is_float_type(node->type)) {
                if (postfix) fprintf(context->out, "    mov.s %s, %s\n", destination, left);
                fputs("    li.s $f20, 1.0\n", context->out);
                fprintf(context->out, "    %s %s, %s, $f20\n",
                        increment ? "add.s" : "sub.s",
                        postfix ? "$f20" : destination, left);
                if (postfix) stored = "$f20";
            } else {
                if (postfix) fprintf(context->out, "    move %s, %s\n", destination, left);
                fprintf(context->out, "    addiu %s, %s, %d\n",
                        postfix ? "$t9" : destination, left, increment ? 1 : -1);
                if (postfix) stored = "$t9";
            }
            fprintf(context->out, "    addiu $sp, $sp, -4\n    %s %s, 0($sp)\n",
                    is_float_type(node->type) ? "s.s" : "sw", stored);
            emit_address(context, node->child[0], "$t8");
            fprintf(context->out, "    %s %s, 0($sp)\n    addiu $sp, $sp, 4\n"
                    "    %s %s, 0($t8)\n",
                    is_float_type(node->type) ? "l.s" : "lw",
                    is_float_type(node->type) ? "$f20" : "$t9",
                    is_float_type(node->type) ? "s.s" : "sw",
                    is_float_type(node->type) ? "$f20" : "$t9");
            if (!postfix)
                fprintf(context->out, "    %s %s, %s\n",
                        is_float_type(node->type) ? "mov.s" : "move", destination,
                        is_float_type(node->type) ? "$f20" : "$t9");
        } else if (!strcmp(node->text, "sizeof") ||
                   (!strcmp(node->text, "length") && node->is_constant))
            fprintf(context->out, "    li %s, %lld\n", destination, node->int_value);
        else if (!strcmp(node->text, "length")) {
            if (node->child[0] && node->child[0]->type &&
                node->child[0]->type->kind == TYPE_STRING) {
                save_volatile_registers(context);
                fprintf(context->out, "    move $a0, %s\n    jal _strlen\n    nop\n", left);
                restore_volatile_registers(context);
                fprintf(context->out, "    move %s, $v0\n", destination);
            }
            else
                fprintf(context->out, "    lw %s, 0(%s)\n", destination, left);
        }
        else {
            const char *cursor;
            int drops = 0;
            int has_car = 0;
            for (cursor = node->text ? node->text : ""; *cursor; cursor++) {
                if (toupper((unsigned char)*cursor) == 'D') drops++;
                if (toupper((unsigned char)*cursor) == 'A') has_car = 1;
            }
            if (has_car) {
                fprintf(context->out, "    lw $t8, 8(%s)\n    li $t9, %d\n"
                        "    mul $t8, $t8, $t9\n    addu $t8, $t8, %s\n"
                        "    %s %s, 12($t8)\n", left, drops, left,
                        is_float_type(node->type) ? "l.s" :
                        (node->type && node->type->kind == TYPE_CHAR ? "lb" : "lw"),
                        destination);
            } else {
                save_volatile_registers(context);
                fprintf(context->out, "    move $a0, %s\n    li $a1, %d\n"
                        "    jal __list_drop\n    nop\n", left, drops);
                restore_volatile_registers(context);
                fprintf(context->out, "    move %s, $v0\n", destination);
            }
        }
        break;
    case AST_ASSIGN:
        emit_expr(context, node->child[1]);
        right = load_value(context, node->child[1], is_float_type(node->type) ? "$f18" : "$t9");
        if (is_aggregate_type(node->type)) {
            emit_address(context, node->child[0], "$t8");
            fputs("    move $a0, $t8\n", context->out);
            fprintf(context->out, "    move $a1, %s\n    li $a2, %zu\n",
                    right, node->type->size);
            save_volatile_registers(context);
            fputs("    jal _memcpy\n    nop\n", context->out);
            restore_volatile_registers(context);
            fprintf(context->out, "    move %s, $v0\n", destination);
        } else {
            emit_address(context, node->child[0], "$t8");
            fprintf(context->out, "    %s %s, 0($t8)\n",
                    is_float_type(node->type) ? "s.s" :
                    (node->type && node->type->kind == TYPE_CHAR ? "sb" : "sw"), right);
            if (is_float_type(node->type)) fprintf(context->out, "    mov.s %s, %s\n", destination, right);
            else fprintf(context->out, "    move %s, %s\n", destination, right);
        }
        break;
    case AST_CALL: emit_call(context, node); return;
    default:
        fprintf(context->out, "    # expression node %s\n", ast_kind_name(node->kind));
    }
    store_spill(context, node, destination);
}

/* Αρχικοποιεί τιμή σύμφωνα με τον δηλωμένο τύπο. */
static void emit_typed_initializer(Context *context, Type *type,
                                   AstNode *initializer, const char *base_register,
                                   long base_offset);

/* Αρχικοποιεί διαδοχικά τα πεδία μιας κλάσης. */
static void emit_class_initializers(Context *context, Type *type, AstNode **item,
                                    const char *base_register, long base_offset)
{
    Symbol *member;
    if (!type || !item) return;
    if (type->parent)
        emit_class_initializers(context, type->parent, item,
                                base_register, base_offset);
    for (member = symtab_first_member(type); member && *item;
         member = symtab_next_declared(member)) {
        if (member->kind != SYMBOL_FIELD) continue;
        emit_typed_initializer(context, member->type, *item, base_register,
                               base_offset + member->offset);
        *item = (*item)->next;
    }
}

/* Παράγει αρχικοποίηση scalar ή σύνθετης τιμής. */
static void emit_typed_initializer(Context *context, Type *type,
                                   AstNode *initializer, const char *base_register,
                                   long base_offset)
{
    AstNode *item;
    const char *value;
    long index;
    if (!type || !initializer) return;
    if (type->kind == TYPE_ARRAY && initializer->kind == AST_INIT_LIST) {
        item = initializer->child[0];
        if (type->base && type->base->kind == TYPE_ARRAY && item &&
            item->kind != AST_INIT_LIST) {
            Type *leaf = type;
            long cursor = 0;
            while (leaf && leaf->kind == TYPE_ARRAY) leaf = leaf->base;
            for (; item; item = item->next) {
                emit_typed_initializer(context, leaf, item, base_register,
                                       base_offset + cursor);
                cursor += leaf && leaf->size ? (long)leaf->size : 4;
            }
            return;
        }
        for (index = 0; item && index < type->array_length;
             index++, item = item->next)
            emit_typed_initializer(context, type->base, item, base_register,
                                   base_offset + index * (long)type->base->size);
        return;
    }
    if (type->kind == TYPE_CLASS && initializer->kind == AST_INIT_LIST) {
        item = initializer->child[0];
        emit_class_initializers(context, type, &item, base_register, base_offset);
        return;
    }
    if (type->kind == TYPE_UNION && initializer->kind == AST_INIT_LIST) {
        Symbol *member = symtab_first_member(type);
        while (member && member->kind != SYMBOL_FIELD)
            member = symtab_next_declared(member);
        if (member && initializer->child[0])
            emit_typed_initializer(context, member->type, initializer->child[0],
                                   base_register, base_offset + member->offset);
        return;
    }
    emit_expr(context, initializer);
    value = load_value(context, initializer, is_float_type(type) ? "$f18" : "$t9");
    if (type->kind == TYPE_STRING) {
        fprintf(context->out, "    addiu $a0, %s, %ld\n    move $a1, %s\n",
                base_register, base_offset, value);
        save_volatile_registers(context);
        fputs("    jal _strcpy\n    nop\n", context->out);
        restore_volatile_registers(context);
    } else {
        fprintf(context->out, "    %s %s, %ld(%s)\n",
                is_float_type(type) ? "s.s" :
                (type->kind == TYPE_CHAR ? "sb" : "sw"),
                value, base_offset, base_register);
    }
}

/* Παράγει τις αρχικοποιήσεις καθολικών μεταβλητών. */
static void emit_global_initializers(Context *context)
{
    AstNode *declaration;
    fputs("    # initialize global/static data\n", context->out);
    for (declaration = ir_globals(); declaration; declaration = declaration->next) {
        Symbol *symbol = declaration->symbol;
        if (!symbol || symbol->storage != STORAGE_GLOBAL || !declaration->child[0]) continue;
        emit_typed_initializer(context, symbol->type, declaration->child[0],
                               "$gp", symbol->offset);
    }
}

/* Δημιουργεί μοναδική ετικέτα ελέγχου ροής. */
static void make_label(Context *context, char *buffer, size_t size, const char *prefix)
{
    fprintf(context->out, "    # label %s_%d\n", prefix, context->label_counter);
    snprintf(buffer, size, "__%s_%d", prefix, context->label_counter++);
}

/* Παράγει MIPS για λίστα εντολών. */
static void emit_statement(Context *context, AstNode *node)
{
    while (node) {
        char first[64], second[64], third[64];
        const char *value;
        switch (node->kind) {
        case AST_BLOCK: emit_statement(context, node->child[0]); break;
        case AST_DECL:
            if (node->child[0]) {
                emit_address(context, node, "$t8");
                if (node->child[0]->kind == AST_INIT_LIST) {
                    fputs("    move $s7, $t8\n", context->out);
                    emit_typed_initializer(context, node->type, node->child[0],
                                           "$s7", 0);
                } else if (node->type && node->type->kind == TYPE_STRING) {
                    emit_expr(context, node->child[0]);
                    value = load_value(context, node->child[0], "$t9");
                    emit_address(context, node, "$t8");
                    fprintf(context->out, "    move $a0, $t8\n    move $a1, %s\n", value);
                    save_volatile_registers(context);
                    fputs("    jal _strcpy\n    nop\n", context->out);
                    restore_volatile_registers(context);
                } else if (is_aggregate_type(node->type)) {
                    emit_expr(context, node->child[0]);
                    value = load_value(context, node->child[0], "$t9");
                    emit_address(context, node, "$t8");
                    fprintf(context->out, "    move $a0, $t8\n    move $a1, %s\n"
                            "    li $a2, %zu\n",
                            value, node->type->size);
                    save_volatile_registers(context);
                    fputs("    jal _memcpy\n    nop\n", context->out);
                    restore_volatile_registers(context);
                } else {
                    emit_expr(context, node->child[0]);
                    value = load_value(context, node->child[0],
                                       is_float_type(node->type) ? "$f18" : "$t9");
                    emit_address(context, node, "$t8");
                    fprintf(context->out, "    %s %s, 0($t8)\n",
                            is_float_type(node->type) ? "s.s" :
                            (node->type && node->type->kind == TYPE_CHAR ? "sb" : "sw"),
                            value);
                }
            }
            break;
        case AST_IF:
            make_label(context, first, sizeof(first), "else"); make_label(context, second, sizeof(second), "endif");
            emit_expr(context, node->child[0]);
            emit_boolean_value(context, node->child[0], "$v0");
            fprintf(context->out, "    beq $v0, $zero, %s\n    nop\n",
                    node->child[2] ? first : second);
            emit_statement(context, node->child[1]);
            if (node->child[2]) { fprintf(context->out, "    j %s\n    nop\n%s:\n", second, first); emit_statement(context, node->child[2]); }
            fprintf(context->out, "%s:\n", second); break;
        case AST_WHILE:
            make_label(context, first, sizeof(first), "while"); make_label(context, second, sizeof(second), "endwhile");
            context->continue_labels[context->loop_depth] = xstrdup(first);
            context->break_labels[context->loop_depth++] = xstrdup(second);
            fprintf(context->out, "%s:\n", first); emit_expr(context, node->child[0]);
            emit_boolean_value(context, node->child[0], "$v0");
            fprintf(context->out, "    beq $v0, $zero, %s\n    nop\n", second);
            emit_statement(context, node->child[1]); fprintf(context->out, "    j %s\n    nop\n%s:\n", first, second);
            free((char *)context->continue_labels[--context->loop_depth]); free((char *)context->break_labels[context->loop_depth]); break;
        case AST_FOR:
            make_label(context, first, sizeof(first), "for"); make_label(context, second, sizeof(second), "forstep"); make_label(context, third, sizeof(third), "endfor");
            if (node->child[0]) emit_expr(context, node->child[0]);
            context->continue_labels[context->loop_depth] = xstrdup(second);
            context->break_labels[context->loop_depth++] = xstrdup(third);
            fprintf(context->out, "%s:\n", first);
            if (node->child[1]) { emit_expr(context, node->child[1]);
                emit_boolean_value(context, node->child[1], "$v0");
                fprintf(context->out, "    beq $v0, $zero, %s\n    nop\n", third); }
            emit_statement(context, node->child[3]); fprintf(context->out, "%s:\n", second);
            if (node->child[2]) emit_expr(context, node->child[2]);
            fprintf(context->out, "    j %s\n    nop\n%s:\n", first, third);
            free((char *)context->continue_labels[--context->loop_depth]); free((char *)context->break_labels[context->loop_depth]); break;
        case AST_RETURN:
            if (node->child[0]) { emit_expr(context, node->child[0]); value = load_value(context, node->child[0], is_float_type(node->child[0]->type) ? "$f18" : "$t9");
                if (is_float_type(node->child[0]->type)) fprintf(context->out, "    mov.s $f0, %s\n", value); else fprintf(context->out, "    move $v0, %s\n", value); }
            mangle(first, sizeof(first), context->function->name); fprintf(context->out, "    j %s__exit\n    nop\n", first); break;
        case AST_BREAK:
            if (context->loop_depth)
                fprintf(context->out, "    j %s\n    nop\n",
                        context->break_labels[context->loop_depth - 1]);
            break;
        case AST_CONTINUE:
            if (context->loop_depth)
                fprintf(context->out, "    j %s\n    nop\n",
                        context->continue_labels[context->loop_depth - 1]);
            break;
        case AST_OUTPUT:
        case AST_INPUT: {
            AstNode *item;
            int slot = 1;
            int total_slots;
            int call_area;
            for (item = node->child[0]; item; item = item->next) {
                if (node->kind == AST_OUTPUT && is_float_type(item->type)) {
                    if (slot & 1) slot++;
                    slot += 2;
                } else slot++;
            }
            total_slots = slot;
            call_area = (total_slots < 4 ? 4 : total_slots) * 4;
            save_volatile_registers(context);
            fprintf(context->out, "    addiu $sp, $sp, -%d\n", call_area);
            fprintf(context->out, "    la $t9, %s\n    sw $t9, 0($sp)\n",
                    format_label(node));
            slot = 1;
            for (item = node->child[0]; item; item = item->next) {
                if (node->kind == AST_INPUT) {
                    emit_address(context, item, "$t9");
                    fprintf(context->out, "    sw $t9, %d($sp)\n", slot * 4);
                    slot++;
                } else {
                    emit_expr(context, item);
                    value = load_value(context, item,
                                       is_float_type(item->type) ? "$f18" : "$t9");
                    if (is_float_type(item->type)) {
                        if (slot & 1) slot++;
                        fprintf(context->out, "    cvt.d.s $f20, %s\n"
                                "    s.d $f20, %d($sp)\n",
                                value, slot * 4);
                        slot += 2;
                    } else {
                        fprintf(context->out, "    move $t9, %s\n"
                                "    sw $t9, %d($sp)\n", value, slot * 4);
                        slot++;
                    }
                }
            }
            fputs("    lw $a0, 0($sp)\n    lw $a1, 4($sp)\n"
                  "    lw $a2, 8($sp)\n    lw $a3, 12($sp)\n", context->out);
            fprintf(context->out, "    jal %s\n    nop\n",
                    node->kind == AST_INPUT ? "_scanf" : "_printf");
            fprintf(context->out, "    addiu $sp, $sp, %d\n", call_area);
            restore_volatile_registers(context);
            break;
        }
        case AST_EMPTY: break;
        default: emit_expr(context, node); break;
        }
        node = node->next;
    }
}

/* Παράγει prologue, σώμα και epilogue συνάρτησης. */
static void emit_function(FILE *out, IrFunction *function)
{
    Context context = { 0 };
    RegisterAllocation allocation;
    Symbol *symbol;
    long local_bytes = align4(symtab_scope_total_size(function->scope));
    int argument = function->owner_type ? 1 : 0;
    char label[256];
    allocation = regalloc_function(function, local_bytes + (function->owner_type ? 4 : 0));
    context.out = out; context.function = function;
    context.this_offset = function->owner_type ? local_bytes : -1;
    context.frame_size = align4(local_bytes + (function->owner_type ? 4 : 0) +
                                allocation.spill_bytes + FRAME_ADMIN_BYTES);
    mangle(label, sizeof(label), function->name);
    fprintf(out, "\n# function %s: temps=%d colors=%d spills=%d\n%s:\n",
            function->name, allocation.temporary_count, allocation.colored_count,
            allocation.spilled_count, label);
    fprintf(out, "    addiu $sp, $sp, -%ld\n    sw $ra, %ld($sp)\n"
            "    sw $fp, %ld($sp)\n    sw $s7, %ld($sp)\n    move $fp, $sp\n",
            context.frame_size, context.frame_size - 4, context.frame_size - 8,
            context.frame_size - 12);
    if (!strcmp(function->name, "main")) {
        fprintf(out, "    la $gp, __global_base\n");
        emit_global_initializers(&context);
    }
    if (function->owner_type) fprintf(out, "    sw $a0, %ld($fp)\n", context.this_offset);
    for (symbol = symtab_scope_first_symbol(function->scope); symbol; symbol = symtab_next_declared(symbol)) {
        if (symbol->kind != SYMBOL_PARAMETER) continue;
        if (argument < 4) fprintf(out, "    sw $a%d, %ld($fp)\n", argument, symbol->offset);
        else fprintf(out, "    lw $t8, %ld($fp)\n    sw $t8, %ld($fp)\n",
                     context.frame_size + argument * 4L, symbol->offset);
        argument++;
    }
    emit_statement(&context, function->body);
    fprintf(out, "%s__exit:\n    move $sp, $fp\n    lw $s7, %ld($sp)\n"
            "    lw $fp, %ld($sp)\n    lw $ra, %ld($sp)\n    addiu $sp, $sp, %ld\n",
            label, context.frame_size - 12, context.frame_size - 8,
            context.frame_size - 4, context.frame_size);
    fprintf(out, "    jr $ra\n    nop\n");
}

/* Ενσωματώνει τις βοηθητικές ρουτίνες λιστών. */
static void emit_list_runtime(FILE *out)
{
    fputs(
        "\n# CPP list runtime: descriptor = {length, capacity, element-size, data...}\n"
        "__list_new:\n"
        "    addiu $sp, $sp, -16\n"
        "    sw $ra, 12($sp)\n"
        "    sw $a0, 0($sp)\n"
        "    sw $a1, 4($sp)\n"
        "    mul $a0, $a0, $a1\n"
        "    addiu $a0, $a0, 12\n"
        "    jal _malloc\n"
        "    nop\n"
        "    lw $t8, 0($sp)\n"
        "    lw $t9, 4($sp)\n"
        "    sw $zero, 0($v0)\n"
        "    sw $t8, 4($v0)\n"
        "    sw $t9, 8($v0)\n"
        "    lw $ra, 12($sp)\n"
        "    addiu $sp, $sp, 16\n"
        "    jr $ra\n"
        "    nop\n"
        "__list_push:\n"
        "    lw $t8, 0($a0)\n"
        "    lw $t9, 8($a0)\n"
        "    mul $v1, $t8, $t9\n"
        "    addu $v1, $v1, $a0\n"
        "    beq $t9, 1, __list_push_byte\n"
        "    nop\n"
        "    sw $a1, 12($v1)\n"
        "    j __list_push_done\n"
        "    nop\n"
        "__list_push_byte:\n"
        "    sb $a1, 12($v1)\n"
        "__list_push_done:\n"
        "    addiu $t8, $t8, 1\n"
        "    sw $t8, 0($a0)\n"
        "    jr $ra\n"
        "    nop\n"
        "__list_drop:\n"
        "    addiu $sp, $sp, -32\n"
        "    sw $ra, 28($sp)\n"
        "    sw $a0, 0($sp)\n"
        "    sw $a1, 4($sp)\n"
        "    lw $t8, 0($a0)\n"
        "    subu $t8, $t8, $a1\n"
        "    bgez $t8, __list_drop_count_ok\n"
        "    nop\n"
        "    move $t8, $zero\n"
        "__list_drop_count_ok:\n"
        "    sw $t8, 8($sp)\n"
        "    lw $a1, 8($a0)\n"
        "    sw $a1, 12($sp)\n"
        "    move $a0, $t8\n"
        "    jal __list_new\n"
        "    nop\n"
        "    sw $v0, 16($sp)\n"
        "    lw $t8, 8($sp)\n"
        "    sw $t8, 0($v0)\n"
        "    addiu $a0, $v0, 12\n"
        "    lw $t9, 12($sp)\n"
        "    lw $v1, 4($sp)\n"
        "    mul $v1, $v1, $t9\n"
        "    lw $a1, 0($sp)\n"
        "    addu $a1, $a1, $v1\n"
        "    addiu $a1, $a1, 12\n"
        "    mul $a2, $t8, $t9\n"
        "    jal _memcpy\n"
        "    nop\n"
        "    lw $v0, 16($sp)\n"
        "    lw $ra, 28($sp)\n"
        "    addiu $sp, $sp, 32\n"
        "    jr $ra\n"
        "    nop\n", out);
}

/* Παράγει ολόκληρο το τελικό αρχείο MIPS. */
int codegen_mips(const char *output_path)
{
    FILE *out;
    IrFunction *function;
    LiteralLabel *literal;
    FormatLabel *format;
    long global_size = align4(symtab_global_size());
    literal_labels = NULL; format_labels = NULL;
    literal_counter = 0; format_counter = 0;
    collect_literals(ir_globals());
    for (function = ir_functions(); function; function = function->next)
        collect_literals(function->body);
    out = fopen(output_path, "w");
    if (!out) { perror(output_path); free_literals(); return -1; }
    fputs("# MIPS final code generated by the CPP Stage-3 compiler\n"
          ".data\n.align 2\n__global_base:\n", out);
    fprintf(out, "    .space %ld\n", global_size > 0 ? global_size : 4);
    fputs("__empty_string: .asciiz \"\"\n"
          "__fmt_int:    .asciiz \"%d\"\n"
          "__fmt_float:  .asciiz \"%f\"\n"
          "__fmt_char:   .asciiz \"%c\"\n"
          "__fmt_string: .asciiz \"%s\"\n", out);
    for (literal = literal_labels; literal; literal = literal->next) {
        fprintf(out, "%s: .asciiz ", literal->label);
        escape_string(out, literal->node->constant_kind == CONST_VALUE_STRING
                      ? literal->node->constant_text : literal->node->text);
        fputc('\n', out);
    }
    for (format = format_labels; format; format = format->next) {
        fprintf(out, "%s: .asciiz ", format->label);
        escape_string(out, format->format);
        fputc('\n', out);
    }
    fputs("\n.text\n.globl main\n.extern _printf\n.extern _scanf\n"
          ".extern _strlen\n.extern _strcpy\n.extern _memcpy\n.extern _malloc\n", out);
    for (function = ir_functions(); function; function = function->next)
        emit_function(out, function);
    emit_list_runtime(out);
    fclose(out);
    free_literals();
    return 0;
}
