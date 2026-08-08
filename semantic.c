#include "semantic.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int errors;
static int loop_depth;
static Type *function_result;

void semantic_init(void) { errors = 0; loop_depth = 0; function_result = NULL; }
void semantic_finish(void) {}
int semantic_error_count(void) { return errors; }

void semantic_error(int line, const char *format, ...)
{
    va_list arguments;
    errors++;
    fprintf(stderr, "Semantic error (line %d): ", line);
    va_start(arguments, format); vfprintf(stderr, format, arguments); va_end(arguments);
    fputc('\n', stderr);
    /* Stage 2 explicitly requires immediate termination on a semantic error. */
    exit(EXIT_FAILURE);
}

Type *semantic_resolve_type(const char *name, int line)
{
    Symbol *symbol = symtab_lookup(name);
    if (!symbol || symbol->kind != SYMBOL_TYPE)
        semantic_error(line, "unknown type '%s'", name);
    return symbol->type;
}

Symbol *semantic_declare(const char *name, SymbolKind kind, Type *type,
                         StorageClass storage, int line, int defined)
{
    Symbol *symbol = symtab_declare_full(name, kind, type, storage, line, defined);
    if (!symbol) semantic_error(line, "'%s' is already declared in scope '%s'",
                                name, symtab_current_label());
    return symbol;
}

Symbol *semantic_declare_function(const char *name, Type *type, int line, int definition)
{
    Symbol *old = symtab_lookup_current(name);
    if (!old) return semantic_declare(name, SYMBOL_FUNCTION, type, STORAGE_NONE, line, definition);
    if (old->kind != SYMBOL_FUNCTION || !type_equal(old->type, type))
        semantic_error(line, "definition/prototype of '%s' has an incompatible signature", name);
    if (definition && old->defined) semantic_error(line, "function '%s' has more than one definition", name);
    if (definition) old->defined = 1;
    return old;
}

void semantic_verify_all_prototypes(void)
{
    /* Prototype completeness is checked by the parser when each global unit closes.
       The retained symbol table also makes a final whole-program pass possible. */
}

AstNode *semantic_identifier(const char *name, int line)
{
    Symbol *symbol = symtab_lookup(name);
    AstNode *node;
    if (!symbol) semantic_error(line, "use of undeclared identifier '%s'", name);
    if (symbol->kind == SYMBOL_TYPE) semantic_error(line, "type name '%s' used as a value", name);
    node = ast_text(AST_IDENTIFIER, name, symbol->type, line);
    node->is_lvalue = symbol->kind == SYMBOL_VARIABLE || symbol->kind == SYMBOL_PARAMETER ||
                      symbol->kind == SYMBOL_FIELD;
    node->is_constant = symbol->kind == SYMBOL_CONSTANT || symbol->kind == SYMBOL_ENUM_VALUE;
    return node;
}

AstNode *semantic_binary(const char *op, AstNode *left, AstNode *right, int line)
{
    Type *result;
    if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*") ||
        !strcmp(op, "/") || !strcmp(op, "%")) {
        if (!type_is_numeric(left->type) || !type_is_numeric(right->type))
            semantic_error(line, "operator '%s' requires numeric operands", op);
        if (!strcmp(op, "%") && (left->type->kind != TYPE_INT || right->type->kind != TYPE_INT))
            semantic_error(line, "operator '%%' requires integer operands");
        result = type_common_numeric(left->type, right->type);
        left = ast_cast(left, result, line); right = ast_cast(right, result, line);
    } else if (!strcmp(op, "&&") || !strcmp(op, "||")) {
        if (!type_is_scalar(left->type) || !type_is_scalar(right->type))
            semantic_error(line, "logical operator '%s' requires scalar operands", op);
        result = type_int();
    } else {
        if (!(type_assignable(left->type, right->type) || type_assignable(right->type, left->type)))
            semantic_error(line, "incompatible operands for comparison '%s'", op);
        result = type_int();
    }
    return ast_binary(op, left, right, result, line);
}

AstNode *semantic_unary(const char *op, AstNode *operand, int line)
{
    Type *result = operand->type;
    if (!strcmp(op, "+") || !strcmp(op, "-")) {
        if (!type_is_numeric(operand->type)) semantic_error(line, "unary '%s' requires numeric operand", op);
    } else if (!strcmp(op, "!")) {
        if (!type_is_scalar(operand->type)) semantic_error(line, "logical negation requires scalar operand");
        result = type_int();
    } else if (!strcmp(op, "++") || !strcmp(op, "--")) {
        if (!operand->is_lvalue || !type_is_numeric(operand->type))
            semantic_error(line, "operator '%s' requires a numeric lvalue", op);
    } else if (!strcmp(op, "sizeof")) result = type_int();
    return ast_unary(op, operand, result, line);
}

AstNode *semantic_assignment(AstNode *left, AstNode *right, int line)
{
    AstNode *node;
    if (!left->is_lvalue) semantic_error(line, "left side of assignment is not an lvalue");
    if (!type_assignable(left->type, right->type)) semantic_error(line, "incompatible assignment types");
    right = ast_cast(right, left->type, line);
    node = ast_new(AST_ASSIGN, line, "="); node->type = left->type;
    node->child[0] = left; node->child[1] = right;
    return node;
}

AstNode *semantic_call(AstNode *function, AstNode *arguments, int line)
{
    Type *type = function->type;
    Parameter *formal;
    AstNode *actual;
    AstNode *node;
    if (!type || type->kind != TYPE_FUNCTION) semantic_error(line, "called expression is not a function");
    formal = type->parameters; actual = arguments;
    while (formal && actual) {
        if (formal->mode == PASS_REFERENCE && (!actual->is_lvalue || !type_equal(formal->type, actual->type)))
            semantic_error(line, "reference argument must be an lvalue of exactly the declared type");
        if (formal->mode == PASS_VALUE && !type_assignable(formal->type, actual->type))
            semantic_error(line, "incompatible value argument type");
        formal = formal->next; actual = actual->next;
    }
    if (formal || actual) semantic_error(line, "wrong number of arguments in function call");
    node = ast_new(AST_CALL, line, NULL); node->type = type->base;
    node->child[0] = function; node->child[1] = arguments;
    return node;
}

AstNode *semantic_index(AstNode *array, AstNode *index, int line)
{
    AstNode *node;
    if (!array->type || array->type->kind != TYPE_ARRAY) semantic_error(line, "indexing requires an array");
    if (!index->type || index->type->kind != TYPE_INT) semantic_error(line, "array index must have type int");
    node = ast_new(AST_INDEX, line, NULL); node->type = array->type->base; node->is_lvalue = 1;
    node->child[0] = array; node->child[1] = index;
    return node;
}

AstNode *semantic_member(AstNode *object, const char *name, int line)
{
    Scope *members;
    Symbol *member;
    AstNode *node;
    if (!object->type || (object->type->kind != TYPE_CLASS && object->type->kind != TYPE_UNION))
        semantic_error(line, "member access requires class or union object");
    members = (Scope *)object->type->members;
    member = symtab_lookup_in_scope(members, name);
    if (!member) semantic_error(line, "type '%s' has no member '%s'", object->type->name, name);
    node = ast_new(AST_MEMBER, line, name); node->type = member->type;
    node->is_lvalue = member->kind == SYMBOL_FIELD; node->child[0] = object;
    return node;
}

AstNode *semantic_condition(AstNode *expression, int line)
{
    if (!expression || !type_is_scalar(expression->type)) semantic_error(line, "condition must have scalar type");
    return expression;
}

void semantic_begin_function(Type *return_type) { function_result = return_type; }
void semantic_end_function(void) { function_result = NULL; }

AstNode *semantic_return(AstNode *expression, int line)
{
    AstNode *node = ast_new(AST_RETURN, line, NULL);
    if (!function_result) semantic_error(line, "return used outside a function");
    if (function_result->kind == TYPE_VOID && expression) semantic_error(line, "void function cannot return a value");
    if (function_result->kind != TYPE_VOID && !expression) semantic_error(line, "non-void function must return a value");
    if (expression && !type_equal(function_result, expression->type))
        semantic_error(line, "return expression type must exactly match the function result type");
    node->child[0] = expression; return node;
}

void semantic_enter_loop(void) { loop_depth++; }
void semantic_leave_loop(void) { if (loop_depth) loop_depth--; }

AstNode *semantic_loop_jump(AstKind kind, int line)
{
    if (!loop_depth) semantic_error(line, "%s used outside a loop", kind == AST_BREAK ? "break" : "continue");
    return ast_new(kind, line, NULL);
}
