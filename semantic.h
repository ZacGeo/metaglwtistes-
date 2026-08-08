#ifndef CPP_SEMANTIC_H
#define CPP_SEMANTIC_H

#include "ast.h"
#include "symtab.h"

void semantic_init(void);
void semantic_finish(void);
void semantic_error(int line, const char *format, ...);
int semantic_error_count(void);

Type *semantic_resolve_type(const char *name, int line);
Symbol *semantic_declare(const char *name, SymbolKind kind, Type *type,
                         StorageClass storage, int line, int defined);
Symbol *semantic_declare_function(const char *name, Type *type, int line, int definition);
void semantic_verify_all_prototypes(void);

AstNode *semantic_identifier(const char *name, int line);
AstNode *semantic_binary(const char *op, AstNode *left, AstNode *right, int line);
AstNode *semantic_unary(const char *op, AstNode *operand, int line);
AstNode *semantic_assignment(AstNode *left, AstNode *right, int line);
AstNode *semantic_call(AstNode *function, AstNode *arguments, int line);
AstNode *semantic_index(AstNode *array, AstNode *index, int line);
AstNode *semantic_member(AstNode *object, const char *name, int line);
AstNode *semantic_condition(AstNode *expression, int line);

void semantic_begin_function(Type *return_type);
void semantic_end_function(void);
AstNode *semantic_return(AstNode *expression, int line);
void semantic_enter_loop(void);
void semantic_leave_loop(void);
AstNode *semantic_loop_jump(AstKind kind, int line);

#endif
