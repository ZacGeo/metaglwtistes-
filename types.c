#define _POSIX_C_SOURCE 200809L
#include "types.h"

#include <stdlib.h>
#include <string.h>

typedef struct TypeCell {
    Type *type;
    struct TypeCell *next;
} TypeCell;

static Type primitive_error, primitive_void, primitive_int;
static Type primitive_float, primitive_char, primitive_string;
static TypeCell *allocated_types;

static void die(void)
{
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
}

static Type *remember(Type *type)
{
    TypeCell *cell = malloc(sizeof(*cell));
    if (!cell) die();
    cell->type = type;
    cell->next = allocated_types;
    allocated_types = cell;
    return type;
}

static Type *new_type(TypeKind kind, const char *name)
{
    Type *type = calloc(1, sizeof(*type));
    if (!type) die();
    type->kind = kind;
    type->name = name ? strdup(name) : NULL;
    type->array_length = -1;
    return remember(type);
}

void types_init(void)
{
    primitive_error = (Type){ TYPE_ERROR, "<error>", NULL, -1, 0, 1, NULL, 0, NULL };
    primitive_void = (Type){ TYPE_VOID, "void", NULL, -1, 0, 1, NULL, 0, NULL };
    primitive_int = (Type){ TYPE_INT, "int", NULL, -1, 4, 4, NULL, 0, NULL };
    primitive_float = (Type){ TYPE_FLOAT, "float", NULL, -1, 4, 4, NULL, 0, NULL };
    primitive_char = (Type){ TYPE_CHAR, "char", NULL, -1, 1, 1, NULL, 0, NULL };
    primitive_string = (Type){ TYPE_STRING, "string", &primitive_char, 256, 256, 1, NULL, 0, NULL };
    allocated_types = NULL;
}

void types_finish(void)
{
    while (allocated_types) {
        TypeCell *next = allocated_types->next;
        free(allocated_types->type->name);
        parameter_free(allocated_types->type->parameters);
        free(allocated_types->type);
        free(allocated_types);
        allocated_types = next;
    }
}

Type *type_error(void) { return &primitive_error; }
Type *type_void(void) { return &primitive_void; }
Type *type_int(void) { return &primitive_int; }
Type *type_float(void) { return &primitive_float; }
Type *type_char(void) { return &primitive_char; }
Type *type_string(void) { return &primitive_string; }

Type *type_named(TypeKind kind, const char *name)
{
    Type *type = new_type(kind, name);
    if (kind == TYPE_ENUM) {
        type->size = 4;
        type->alignment = 4;
    }
    return type;
}

Type *type_array(Type *element, long long length)
{
    Type *type = new_type(TYPE_ARRAY, NULL);
    type->base = element;
    type->array_length = length;
    type->alignment = element ? element->alignment : 1;
    type->size = (element && length > 0) ? element->size * (size_t)length : 0;
    return type;
}

Type *type_list(Type *element)
{
    Type *type = new_type(TYPE_LIST, NULL);
    type->base = element;
    type->size = sizeof(void *);
    type->alignment = sizeof(void *);
    return type;
}

Type *type_function(Type *result, Parameter *parameters)
{
    Type *type = new_type(TYPE_FUNCTION, NULL);
    Parameter *p;
    type->base = result;
    type->parameters = parameter_clone(parameters);
    for (p = type->parameters; p; p = p->next) type->parameter_count++;
    type->alignment = 1;
    return type;
}

Parameter *parameter_new(const char *name, Type *type, PassMode mode)
{
    Parameter *parameter = calloc(1, sizeof(*parameter));
    if (!parameter) die();
    parameter->name = name ? strdup(name) : NULL;
    parameter->type = type;
    parameter->mode = mode;
    return parameter;
}

Parameter *parameter_append(Parameter *list, Parameter *item)
{
    Parameter *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

Parameter *parameter_clone(const Parameter *list)
{
    Parameter *copy = NULL;
    while (list) {
        copy = parameter_append(copy, parameter_new(list->name, list->type, list->mode));
        list = list->next;
    }
    return copy;
}

void parameter_free(Parameter *list)
{
    while (list) {
        Parameter *next = list->next;
        free(list->name);
        free(list);
        list = next;
    }
}

int type_is_numeric(const Type *type)
{
    return type && (type->kind == TYPE_INT || type->kind == TYPE_FLOAT);
}

int type_is_scalar(const Type *type)
{
    return type && (type_is_numeric(type) || type->kind == TYPE_CHAR ||
                    type->kind == TYPE_STRING || type->kind == TYPE_ENUM);
}

int type_equal(const Type *left, const Type *right)
{
    const Parameter *a, *b;
    if (left == right) return 1;
    if (!left || !right || left->kind != right->kind) return 0;
    switch (left->kind) {
    case TYPE_ARRAY:
        return left->array_length == right->array_length && type_equal(left->base, right->base);
    case TYPE_LIST:
        return type_equal(left->base, right->base);
    case TYPE_ENUM:
    case TYPE_CLASS:
    case TYPE_UNION:
        return left->name && right->name && strcmp(left->name, right->name) == 0;
    case TYPE_FUNCTION:
        if (!type_equal(left->base, right->base) || left->parameter_count != right->parameter_count)
            return 0;
        a = left->parameters; b = right->parameters;
        while (a && b) {
            if (a->mode != b->mode || !type_equal(a->type, b->type)) return 0;
            a = a->next; b = b->next;
        }
        return 1;
    default:
        return 1;
    }
}

int type_assignable(const Type *destination, const Type *source)
{
    if (!destination || !source) return 0;
    if (type_equal(destination, source)) return 1;
    if (type_is_numeric(destination) && type_is_numeric(source)) return 1;
    if (destination->kind == TYPE_LIST && source->kind == TYPE_LIST)
        return type_assignable(destination->base, source->base);
    return 0;
}

Type *type_common_numeric(Type *left, Type *right)
{
    if (!type_is_numeric(left) || !type_is_numeric(right)) return type_error();
    return left->kind == TYPE_FLOAT || right->kind == TYPE_FLOAT ? type_float() : type_int();
}

const char *type_kind_name(TypeKind kind)
{
    static const char *names[] = { "error", "void", "int", "float", "char", "string",
        "enum", "array", "list", "class", "union", "function" };
    return names[(int)kind];
}

void type_print(FILE *stream, const Type *type)
{
    const Parameter *parameter;
    if (!type) { fputs("<no-type>", stream); return; }
    if (type->name) { fputs(type->name, stream); return; }
    switch (type->kind) {
    case TYPE_ARRAY:
        type_print(stream, type->base);
        fprintf(stream, "[%lld]", type->array_length);
        break;
    case TYPE_LIST:
        type_print(stream, type->base);
        fputs(" list", stream);
        break;
    case TYPE_FUNCTION:
        type_print(stream, type->base);
        fputs(" (", stream);
        for (parameter = type->parameters; parameter; parameter = parameter->next) {
            if (parameter != type->parameters) fputs(", ", stream);
            type_print(stream, parameter->type);
            if (parameter->mode == PASS_REFERENCE) fputc('&', stream);
        }
        fputc(')', stream);
        break;
    default:
        fputs(type_kind_name(type->kind), stream);
    }
}
