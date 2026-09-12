#ifndef CPP_AST_H
#define CPP_AST_H

#include "types.h"

struct Symbol;

typedef enum AstKind {
    AST_PROGRAM, AST_FUNCTION, AST_BLOCK, AST_DECL, AST_EMPTY,
    AST_IDENTIFIER, AST_INT, AST_FLOAT, AST_CHAR, AST_STRING, AST_LIST,
    AST_UNARY, AST_BINARY, AST_ASSIGN, AST_CAST, AST_INDEX, AST_MEMBER, AST_CALL,
    AST_IF, AST_WHILE, AST_FOR, AST_RETURN, AST_INPUT, AST_OUTPUT,
    AST_BREAK, AST_CONTINUE, AST_INIT_LIST
} AstKind;

typedef struct AstNode {
    AstKind kind;
    Type *type;
    int line;
    unsigned is_lvalue : 1;
    unsigned is_constant : 1;
    ConstantValueKind constant_kind;
    char *text;

    char *constant_text;
    long long int_value;
    double float_value;
    struct Symbol *symbol;
    int temporary_id;
    int register_color;
    long spill_offset;
    struct AstNode *child[4];
    struct AstNode *next;
} AstNode;

AstNode *ast_new(AstKind kind, int line, const char *text);
AstNode *ast_int(long long value, int line);
AstNode *ast_float(double value, int line);
AstNode *ast_text(AstKind kind, const char *value, Type *type, int line);
AstNode *ast_init_list(AstNode *items, int line);
AstNode *ast_unary(const char *op, AstNode *operand, Type *type, int line);
AstNode *ast_binary(const char *op, AstNode *left, AstNode *right, Type *type, int line);
AstNode *ast_append(AstNode *list, AstNode *item);
AstNode *ast_cast(AstNode *node, Type *destination, int line);
void ast_print(FILE *stream, const AstNode *node, int indent);
void ast_free(AstNode *node);
const char *ast_kind_name(AstKind kind);

#endif
