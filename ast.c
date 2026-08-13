#define _POSIX_C_SOURCE 200809L
#include "ast.h"

#include <stdlib.h>
#include <string.h>

static void die(void) { fprintf(stderr, "Out of memory\n"); exit(EXIT_FAILURE); }

AstNode *ast_new(AstKind kind, int line, const char *text)
{
    AstNode *node = calloc(1, sizeof(*node));
    if (!node) die();
    node->kind = kind;
    node->line = line;
    node->text = text ? strdup(text) : NULL;
    return node;
}

AstNode *ast_int(long long value, int line)
{
    AstNode *node = ast_new(AST_INT, line, NULL);
    node->type = type_int(); node->int_value = value; node->is_constant = 1;
    node->constant_kind = CONST_VALUE_INT;
    return node;
}

AstNode *ast_float(double value, int line)
{
    AstNode *node = ast_new(AST_FLOAT, line, NULL);
    node->type = type_float(); node->float_value = value; node->is_constant = 1;
    node->constant_kind = CONST_VALUE_FLOAT;
    return node;
}

AstNode *ast_text(AstKind kind, const char *value, Type *type, int line)
{
    AstNode *node = ast_new(kind, line, value);
    node->type = type; node->is_constant = kind == AST_CHAR || kind == AST_STRING;
    if (kind == AST_CHAR) node->constant_kind = CONST_VALUE_CHAR;
    if (kind == AST_STRING) node->constant_kind = CONST_VALUE_STRING;
    if (node->is_constant) {
        node->constant_text = strdup(value ? value : "");
        if (!node->constant_text) die();
    }
    return node;
}

AstNode *ast_init_list(AstNode *items, int line)
{
    AstNode *node = ast_new(AST_INIT_LIST, line, NULL);
    node->child[0] = items;
    node->is_constant = 1;
    node->constant_kind = CONST_VALUE_AGGREGATE;
    return node;
}

AstNode *ast_unary(const char *op, AstNode *operand, Type *type, int line)
{
    AstNode *node = ast_new(AST_UNARY, line, op);
    node->type = type; node->child[0] = operand;
    return node;
}

AstNode *ast_binary(const char *op, AstNode *left, AstNode *right, Type *type, int line)
{
    AstNode *node = ast_new(AST_BINARY, line, op);
    node->type = type; node->child[0] = left; node->child[1] = right;
    return node;
}

AstNode *ast_append(AstNode *list, AstNode *item)
{
    AstNode *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

AstNode *ast_cast(AstNode *node, Type *destination, int line)
{
    AstNode *cast;
    if (type_equal(node->type, destination)) return node;
    cast = ast_new(AST_CAST, line, NULL);
    cast->type = destination; cast->child[0] = node;
    if (node->is_constant && type_is_numeric(node->type) && type_is_numeric(destination)) {
        cast->is_constant = 1;
        if (destination->kind == TYPE_FLOAT) {
            cast->constant_kind = CONST_VALUE_FLOAT;
            cast->float_value = node->constant_kind == CONST_VALUE_FLOAT
                ? node->float_value : (double)node->int_value;
        } else {
            cast->constant_kind = CONST_VALUE_INT;
            cast->int_value = node->constant_kind == CONST_VALUE_FLOAT
                ? (long long)node->float_value : node->int_value;
        }
    }
    return cast;
}

const char *ast_kind_name(AstKind kind)
{
    static const char *names[] = { "PROGRAM", "FUNCTION", "BLOCK", "DECL", "EMPTY",
        "IDENTIFIER", "INT", "FLOAT", "CHAR", "STRING", "LIST", "UNARY", "BINARY",
        "ASSIGN", "CAST", "INDEX", "MEMBER", "CALL", "IF", "WHILE", "FOR", "RETURN",
        "INPUT", "OUTPUT", "BREAK", "CONTINUE", "INIT_LIST" };
    return names[(int)kind];
}

void ast_print(FILE *stream, const AstNode *node, int indent)
{
    int i;
    while (node) {
        fprintf(stream, "%*s%s", indent, "", ast_kind_name(node->kind));
        if (node->text) fprintf(stream, " '%s'", node->text);
        if (node->kind == AST_INT) fprintf(stream, " %lld", node->int_value);
        if (node->kind == AST_FLOAT) fprintf(stream, " %.17g", node->float_value);
        if (node->type) { fputs(" : ", stream); type_print(stream, node->type); }
        if (node->is_lvalue) fputs(" [lvalue]", stream);
        if (node->is_constant) fputs(" [constant]", stream);
        fprintf(stream, " (line %d)\n", node->line);
        for (i = 0; i < 4; i++) if (node->child[i]) ast_print(stream, node->child[i], indent + 2);
        node = node->next;
    }
}

void ast_free(AstNode *node)
{
    while (node) {
        AstNode *next = node->next;
        int i;
        for (i = 0; i < 4; i++) ast_free(node->child[i]);
        free(node->text);
        free(node->constant_text);
        free(node);
        node = next;
    }
}
