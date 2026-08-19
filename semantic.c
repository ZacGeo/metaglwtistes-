#define _POSIX_C_SOURCE 200809L
#include "semantic.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int errors;
static int loop_depth;
static Type *function_result;
static Type *function_class;

static double constant_number(const AstNode *node)
{
    return node->constant_kind == CONST_VALUE_FLOAT
        ? node->float_value : (double)node->int_value;
}

static void set_integer_constant(AstNode *node, long long value)
{
    node->is_constant = 1;
    node->constant_kind = CONST_VALUE_INT;
    node->int_value = value;
}

static void set_numeric_constant(AstNode *node, Type *type, double value)
{
    node->is_constant = 1;
    if (type->kind == TYPE_FLOAT) {
        node->constant_kind = CONST_VALUE_FLOAT;
        node->float_value = value;
    } else {
        node->constant_kind = CONST_VALUE_INT;
        node->int_value = (long long)value;
    }
}

void semantic_init(void) { errors = 0; loop_depth = 0; function_result = NULL; function_class = NULL; }
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
    SymbolKind expected = symtab_current_owner_type() ? SYMBOL_METHOD : SYMBOL_FUNCTION;
    StorageClass storage = expected == SYMBOL_METHOD ? STORAGE_MEMBER : STORAGE_NONE;
    if (!old) return semantic_declare(name, expected, type, storage, line, definition);
    if (old->kind != expected || !type_equal(old->type, type))
        semantic_error(line, "definition/prototype of '%s' has an incompatible signature", name);
    if (definition && old->defined) semantic_error(line, "function '%s' has more than one definition", name);
    if (definition) old->defined = 1;
    return old;
}

Symbol *semantic_declare_method(Type *owner, const char *name, Type *type,
                                int line, int definition)
{
    Symbol *method;
    if (!owner || owner->kind != TYPE_CLASS)
        semantic_error(line, "qualified method definition requires a class type");
    method = symtab_lookup_in_scope((Scope *)owner->members, name);
    if (!method || method->kind != SYMBOL_METHOD)
        semantic_error(line, "method '%s::%s' has no prototype in the class", owner->name, name);
    if (!type_equal(method->type, type))
        semantic_error(line, "definition of '%s::%s' does not match its prototype", owner->name, name);
    if (definition && method->defined)
        semantic_error(line, "method '%s::%s' has more than one definition", owner->name, name);
    if (definition) method->defined = 1;
    return method;
}

static void verify_prototype(Symbol *symbol, void *context)
{
    (void)context;
    if ((symbol->kind == SYMBOL_FUNCTION || symbol->kind == SYMBOL_METHOD) &&
        !symbol->defined)
        semantic_error(symbol->line, "%s '%s' has a prototype but no full definition",
                       symbol->kind == SYMBOL_METHOD ? "method" : "function", symbol->name);
}

void semantic_verify_all_prototypes(void)
{
    symtab_visit_symbols(verify_prototype, NULL);
}

static int member_accessible(const Symbol *member, const Type *declaring)
{
    if (!member || !declaring) return 0;
    if (member->access == ACCESS_PUBLIC) return 1;
    if (!function_class) return 0;
    if (member->access == ACCESS_PRIVATE) return function_class == declaring;
    return type_is_derived_from(function_class, declaring);
}

AstNode *semantic_identifier(const char *name, int line)
{
    Symbol *symbol = symtab_lookup(name);
    AstNode *node;
    Type *declaring = NULL;
    if (!symbol && function_class) {
        symbol = symtab_lookup_member(function_class, name, &declaring);
        if (symbol && !member_accessible(symbol, declaring))
            semantic_error(line, "member '%s' is %s in class '%s'",
                           name, access_kind_name(symbol->access), declaring->name);
    }
    if (!symbol) semantic_error(line, "use of undeclared identifier '%s'", name);
    if (symbol->kind == SYMBOL_TYPE && symbol->type->kind != TYPE_ENUM)
        semantic_error(line, "type name '%s' used as a value", name);
    node = ast_text(AST_IDENTIFIER, name, symbol->type, line);
    node->symbol = symbol;
    node->is_lvalue = symbol->kind == SYMBOL_VARIABLE || symbol->kind == SYMBOL_PARAMETER ||
                      symbol->kind == SYMBOL_FIELD;
    node->is_constant = symbol->kind == SYMBOL_CONSTANT || symbol->kind == SYMBOL_ENUM_VALUE;
    if (node->is_constant) {
        node->constant_kind = symbol->constant_kind;
        node->int_value = symbol->int_value;
        node->float_value = symbol->float_value;
        if (symbol->constant_text) {
            node->constant_text = strdup(symbol->constant_text);
            if (!node->constant_text) { fprintf(stderr, "Out of memory\n"); exit(EXIT_FAILURE); }
        }
    }
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
        if (type_is_numeric(left->type) && type_is_numeric(right->type)) {
            Type *common = type_common_numeric(left->type, right->type);
            left = ast_cast(left, common, line);
            right = ast_cast(right, common, line);
        }
        result = type_int();
    }
    {
        AstNode *node = ast_binary(op, left, right, result, line);
        if (left->is_constant && right->is_constant &&
            (left->constant_kind == CONST_VALUE_INT || left->constant_kind == CONST_VALUE_FLOAT) &&
            (right->constant_kind == CONST_VALUE_INT || right->constant_kind == CONST_VALUE_FLOAT)) {
            double a = constant_number(left), b = constant_number(right);
            if (!strcmp(op, "+")) set_numeric_constant(node, result, a + b);
            else if (!strcmp(op, "-")) set_numeric_constant(node, result, a - b);
            else if (!strcmp(op, "*")) set_numeric_constant(node, result, a * b);
            else if (!strcmp(op, "/")) {
                if (b == 0.0) semantic_error(line, "division by zero in constant expression");
                set_numeric_constant(node, result, a / b);
            } else if (!strcmp(op, "%")) {
                if (right->int_value == 0) semantic_error(line, "division by zero in constant expression");
                set_integer_constant(node, left->int_value % right->int_value);
            } else if (!strcmp(op, "&&")) set_integer_constant(node, a != 0.0 && b != 0.0);
            else if (!strcmp(op, "||")) set_integer_constant(node, a != 0.0 || b != 0.0);
            else if (!strcmp(op, "==")) set_integer_constant(node, a == b);
            else if (!strcmp(op, "!=")) set_integer_constant(node, a != b);
            else if (!strcmp(op, "<")) set_integer_constant(node, a < b);
            else if (!strcmp(op, "<=")) set_integer_constant(node, a <= b);
            else if (!strcmp(op, ">")) set_integer_constant(node, a > b);
            else if (!strcmp(op, ">=")) set_integer_constant(node, a >= b);
        }
        return node;
    }
}

AstNode *semantic_unary(const char *op, AstNode *operand, int line)
{
    Type *result = operand->type;
    if (!strcmp(op, "+") || !strcmp(op, "-")) {
        if (!type_is_numeric(operand->type)) semantic_error(line, "unary '%s' requires numeric operand", op);
    } else if (!strcmp(op, "!")) {
        if (!type_is_scalar(operand->type)) semantic_error(line, "logical negation requires scalar operand");
        result = type_int();
    } else if (!strcmp(op, "++") || !strcmp(op, "--") ||
               !strcmp(op, "post++") || !strcmp(op, "post--")) {
        if (!operand->is_lvalue || !type_is_numeric(operand->type))
            semantic_error(line, "operator '%s' requires a numeric lvalue", op);
    } else if (!strcmp(op, "sizeof")) result = type_int();
    {
        AstNode *node = ast_unary(op, operand, result, line);
        if (!strcmp(op, "sizeof")) {
            set_integer_constant(node, operand->type ? (long long)operand->type->size : 0);
        } else if (operand->is_constant && type_is_numeric(operand->type)) {
            double value = constant_number(operand);
            if (!strcmp(op, "+")) set_numeric_constant(node, result, value);
            else if (!strcmp(op, "-")) set_numeric_constant(node, result, -value);
            else if (!strcmp(op, "!")) set_integer_constant(node, value == 0.0);
        }
        return node;
    }
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
    Type *declaring = NULL;
    Symbol *member;
    AstNode *node;
    if (!object->type || (object->type->kind != TYPE_CLASS &&
        object->type->kind != TYPE_UNION && object->type->kind != TYPE_ENUM))
        semantic_error(line, "member access requires class, union or enum value");
    member = symtab_lookup_member(object->type, name, &declaring);
    if (!member) semantic_error(line, "type '%s' has no member '%s'", object->type->name, name);
    if (object->type->kind != TYPE_ENUM && !member_accessible(member, declaring))
        semantic_error(line, "member '%s' is %s in class '%s'",
                       name, access_kind_name(member->access), declaring->name);
    node = ast_new(AST_MEMBER, line, name); node->type = member->type;
    node->symbol = member;
    node->is_lvalue = member->kind == SYMBOL_FIELD;
    node->is_constant = member->kind == SYMBOL_ENUM_VALUE || member->kind == SYMBOL_CONSTANT;
    if (node->is_constant) {
        node->constant_kind = member->constant_kind;
        node->int_value = member->int_value;
        node->float_value = member->float_value;
    }
    node->child[0] = object;
    return node;
}

AstNode *semantic_this(int line)
{
    AstNode *node;
    if (!function_class) semantic_error(line, "this used outside a class method");
    node = ast_new(AST_IDENTIFIER, line, "this");
    node->type = function_class;
    node->is_lvalue = 1;
    return node;
}

AstNode *semantic_list_literal(AstNode *elements, int line)
{
    AstNode *element;
    Type *base = NULL;
    for (element = elements; element; element = element->next) {
        if (!base) base = element->type;
        else if (type_is_numeric(base) && type_is_numeric(element->type))
            base = type_common_numeric(base, element->type);
        else if (!type_assignable(base, element->type))
            semantic_error(line, "list elements have incompatible types");
    }
    if (base) {
        for (element = elements; element; element = element->next)
            if (type_is_numeric(base) && type_is_numeric(element->type) &&
                !type_equal(base, element->type)) element->type = base;
    }
    {
        AstNode *node = ast_new(AST_LIST, line, NULL);
        node->type = type_list(base ? base : type_error());
        node->child[0] = elements;
        return node;
    }
}

AstNode *semantic_list_function(const char *name, AstNode *argument, int line)
{
    Type *current;
    size_t length, index;
    AstNode *node;
    if (!name || !argument) semantic_error(line, "invalid list function");
    length = strlen(name);
    if (length < 3 || toupper((unsigned char)name[0]) != 'C' ||
        toupper((unsigned char)name[length - 1]) != 'R')
        semantic_error(line, "invalid list function '%s'", name);
    current = argument->type;
    for (index = length - 1; index-- > 1;) {
        char operation = (char)toupper((unsigned char)name[index]);
        if (!current || current->kind != TYPE_LIST)
            semantic_error(line, "%s requires a list at operation '%c'", name, operation);
        if (operation == 'A') current = current->base;
        else if (operation == 'D') { /* CDR preserves the current list type. */ }
        else semantic_error(line, "invalid operation '%c' in list function '%s'", operation, name);
    }
    node = ast_unary(name, argument, current, line);
    node->is_lvalue = argument->is_lvalue;
    return node;
}

AstNode *semantic_length(AstNode *argument, int line)
{
    AstNode *node;
    if (!argument || !argument->type ||
        (argument->type->kind != TYPE_STRING && argument->type->kind != TYPE_ARRAY &&
         argument->type->kind != TYPE_LIST))
        semantic_error(line, "length requires a string, array or list");
    node = ast_unary("length", argument, type_int(), line);
    if (argument->type->kind == TYPE_ARRAY && argument->type->array_length >= 0)
        set_integer_constant(node, argument->type->array_length);
    else if (argument->kind == AST_STRING && argument->text)
        set_integer_constant(node, (long long)strlen(argument->text));
    return node;
}

AstNode *semantic_condition(AstNode *expression, int line)
{
    if (!expression || !type_is_scalar(expression->type)) semantic_error(line, "condition must have scalar type");
    return expression;
}

void semantic_begin_function(Type *return_type) { function_result = return_type; function_class = NULL; }
void semantic_begin_method(Type *owner, Type *return_type) { function_result = return_type; function_class = owner; }
void semantic_end_function(void) { function_result = NULL; function_class = NULL; }

static int guarantees_return(const AstNode *node)
{
    const AstNode *cursor;
    for (cursor = node; cursor; cursor = cursor->next) {
        if (cursor->kind == AST_RETURN) return 1;
        if (cursor->kind == AST_BLOCK && guarantees_return(cursor->child[0])) return 1;
        if (cursor->kind == AST_IF && cursor->child[2] &&
            guarantees_return(cursor->child[1]) && guarantees_return(cursor->child[2])) return 1;
    }
    return 0;
}

void semantic_check_function_body(const AstNode *body, int line)
{
    if (function_result && function_result->kind != TYPE_VOID && !guarantees_return(body))
        semantic_error(line, "non-void function may finish without returning a value");
}

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

AstNode *semantic_string_literal(const char *text, int line)
{
    size_t logical_length = 0;
    const unsigned char *cursor = (const unsigned char *)(text ? text : "");
    while (*cursor) {
        if (*cursor == '\\' && cursor[1] != '\0') cursor++;
        cursor++;
        logical_length++;
    }
    if (logical_length > 255)
        semantic_error(line, "string literal has %zu characters; maximum is 255", logical_length);
    return ast_text(AST_STRING, text, type_string(), line);
}

static int initializer_is_constant(const AstNode *value)
{
    const AstNode *item;
    if (!value) return 0;
    if (value->kind != AST_INIT_LIST) return value->is_constant;
    for (item = value->child[0]; item; item = item->next)
        if (!initializer_is_constant(item)) return 0;
    return 1;
}

void semantic_store_constant(Symbol *symbol, const AstNode *value, int line)
{
    char buffer[96];
    if (!symbol || !value) semantic_error(line, "constant requires an initializer");
    if (value->kind == AST_INIT_LIST) {
        if (!initializer_is_constant(value))
            semantic_error(line, "initializer list for constant '%s' is not constant", symbol->name);
        symtab_set_constant_value(symbol, CONST_VALUE_AGGREGATE, 0, 0.0, NULL);
        return;
    }
    if (!value->is_constant || value->constant_kind == CONST_VALUE_NONE)
        semantic_error(line, "initializer of constant '%s' is not a constant expression", symbol->name);
    switch (value->constant_kind) {
    case CONST_VALUE_INT:
        snprintf(buffer, sizeof(buffer), "%lld", value->int_value);
        symtab_set_constant_value(symbol, CONST_VALUE_INT, value->int_value, 0.0, buffer);
        break;
    case CONST_VALUE_FLOAT:
        snprintf(buffer, sizeof(buffer), "%.17g", value->float_value);
        symtab_set_constant_value(symbol, CONST_VALUE_FLOAT, 0, value->float_value, buffer);
        break;
    case CONST_VALUE_CHAR:
    case CONST_VALUE_STRING:
        symtab_set_constant_value(symbol, value->constant_kind, 0, 0.0,
                                  value->constant_text ? value->constant_text : value->text);
        break;
    default:
        semantic_error(line, "unsupported constant initializer for '%s'", symbol->name);
    }
}

void semantic_store_initial_value(Symbol *symbol, const AstNode *value)
{
    char buffer[96];
    if (!symbol || !value || !initializer_is_constant(value)) return;
    if (value->kind == AST_INIT_LIST || value->kind == AST_LIST) {
        symtab_set_constant_value(symbol, CONST_VALUE_AGGREGATE, 0, 0.0, NULL);
        return;
    }
    switch (value->constant_kind) {
    case CONST_VALUE_INT:
        snprintf(buffer, sizeof(buffer), "%lld", value->int_value);
        symtab_set_constant_value(symbol, CONST_VALUE_INT, value->int_value, 0.0, buffer);
        break;
    case CONST_VALUE_FLOAT:
        snprintf(buffer, sizeof(buffer), "%.17g", value->float_value);
        symtab_set_constant_value(symbol, CONST_VALUE_FLOAT, 0, value->float_value, buffer);
        break;
    case CONST_VALUE_CHAR:
    case CONST_VALUE_STRING:
        symtab_set_constant_value(symbol, value->constant_kind, 0, 0.0,
                                  value->constant_text ? value->constant_text : value->text);
        break;
    default:
        break;
    }
}

long long semantic_require_integer_constant(const AstNode *value,
                                            const char *context, int line)
{
    if (!value || !value->is_constant || value->constant_kind != CONST_VALUE_INT)
        semantic_error(line, "%s requires an integer constant expression", context);
    return value->int_value;
}

static size_t initializer_count(const AstNode *list)
{
    const AstNode *item;
    size_t count = 0;
    for (item = list ? list->child[0] : NULL; item; item = item->next) count++;
    return count;
}

static void validate_initializer(Type *type, AstNode *initializer,
                                 const char *name, int line);

static size_t nested_array_capacity(const Type *type)
{
    size_t capacity = 1;
    while (type && type->kind == TYPE_ARRAY) {
        if (type->array_length <= 0) return 0;
        capacity *= (size_t)type->array_length;
        type = type->base;
    }
    return capacity;
}

static Type *array_leaf_type(Type *type)
{
    while (type && type->kind == TYPE_ARRAY) type = type->base;
    return type;
}

static void validate_aggregate_members(Type *type, AstNode **item,
                                       const char *name, int line)
{
    Symbol *member;
    if (type->parent) validate_aggregate_members(type->parent, item, name, line);
    for (member = symtab_first_member(type); member && *item;
         member = symtab_next_declared(member)) {
        if (member->kind != SYMBOL_FIELD) continue;
        validate_initializer(member->type, *item, name, line);
        *item = (*item)->next;
    }
}

static void validate_initializer(Type *type, AstNode *initializer,
                                 const char *name, int line)
{
    if (!type || !initializer) return;
    if (type->kind == TYPE_ARRAY) {
        AstNode *item;
        size_t count;
        if (initializer->kind != AST_INIT_LIST)
            semantic_error(line, "array '%s' requires an initializer list", name);
        count = initializer_count(initializer);
        if (type->base && type->base->kind == TYPE_ARRAY &&
            initializer->child[0] && initializer->child[0]->kind != AST_INIT_LIST) {
            size_t inner_capacity = nested_array_capacity(type->base);
            size_t total_capacity;
            Type *leaf = array_leaf_type(type);
            if (!inner_capacity)
                semantic_error(line, "cannot determine all dimensions of array '%s'", name);
            if (type->array_length < 0) {
                type->array_length = (long long)((count + inner_capacity - 1) / inner_capacity);
                type->size = type->base->size * (size_t)type->array_length;
            }
            total_capacity = inner_capacity * (size_t)type->array_length;
            if (count > total_capacity)
                semantic_error(line, "too many initializers for array '%s': %zu for capacity %zu",
                               name, count, total_capacity);
            for (item = initializer->child[0]; item; item = item->next)
                validate_initializer(leaf, item, name, line);
            return;
        }
        if (type->array_length < 0) {
            if (count == 0) semantic_error(line, "cannot infer the omitted dimension of array '%s'", name);
            type->array_length = (long long)count;
            type->size = type->base ? type->base->size * count : 0;
        } else if (count > (size_t)type->array_length) {
            semantic_error(line, "too many initializers for array '%s': %zu for dimension %lld",
                           name, count, type->array_length);
        }
        for (item = initializer->child[0]; item; item = item->next)
            validate_initializer(type->base, item, name, line);
        return;
    }
    if (type->kind == TYPE_CLASS) {
        AstNode *item;
        if (initializer->kind != AST_INIT_LIST)
            semantic_error(line, "class object '%s' requires an initializer list", name);
        item = initializer->child[0];
        validate_aggregate_members(type, &item, name, line);
        if (item) semantic_error(line, "too many initializers for class object '%s'", name);
        return;
    }
    if (type->kind == TYPE_UNION) {
        Symbol *member;
        AstNode *item;
        if (initializer->kind != AST_INIT_LIST)
            semantic_error(line, "union object '%s' requires an initializer list", name);
        item = initializer->child[0];
        if (item && item->next)
            semantic_error(line, "union object '%s' accepts at most one initializer", name);
        member = symtab_first_member(type);
        while (member && member->kind != SYMBOL_FIELD) member = symtab_next_declared(member);
        if (item && !member) semantic_error(line, "union type '%s' has no field to initialize", type->name);
        if (item) validate_initializer(member->type, item, name, line);
        return;
    }
    if (initializer->kind == AST_INIT_LIST)
        semantic_error(line, "scalar '%s' cannot be initialized by a list", name);
    if (!type_assignable(type, initializer->type))
        semantic_error(line, "initializer of '%s' has incompatible type", name);
}

void semantic_validate_initializer(Type *type, AstNode *initializer,
                                   const char *name, int line)
{
    if (!initializer) {
        if (type && type->kind == TYPE_ARRAY && type->array_length < 0)
            semantic_error(line, "array '%s' has an omitted dimension but no initializer", name);
        return;
    }
    validate_initializer(type, initializer, name, line);
}
