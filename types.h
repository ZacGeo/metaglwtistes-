#ifndef CPP_TYPES_H
#define CPP_TYPES_H

#include <stddef.h>
#include <stdio.h>

typedef enum TypeKind {
    TYPE_ERROR,
    TYPE_VOID,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_ENUM,
    TYPE_ARRAY,
    TYPE_LIST,
    TYPE_CLASS,
    TYPE_UNION,
    TYPE_FUNCTION
} TypeKind;

typedef enum PassMode {
    PASS_VALUE,
    PASS_REFERENCE
} PassMode;

typedef struct Type Type;

typedef struct Parameter {
    char *name;
    Type *type;
    PassMode mode;
    struct Parameter *next;
} Parameter;

struct Type {
    TypeKind kind;
    char *name;
    Type *base;
    long long array_length; /* -1 means an omitted first dimension. */
    size_t size;
    size_t alignment;
    Parameter *parameters;
    size_t parameter_count;
    void *members;          /* Filled by the symbol-table module. */
};

void types_init(void);
void types_finish(void);
Type *type_error(void);
Type *type_void(void);
Type *type_int(void);
Type *type_float(void);
Type *type_char(void);
Type *type_string(void);
Type *type_named(TypeKind kind, const char *name);
Type *type_array(Type *element, long long length);
Type *type_list(Type *element);
Type *type_function(Type *result, Parameter *parameters);

Parameter *parameter_new(const char *name, Type *type, PassMode mode);
Parameter *parameter_append(Parameter *list, Parameter *item);
Parameter *parameter_clone(const Parameter *list);
void parameter_free(Parameter *list);

int type_is_numeric(const Type *type);
int type_is_scalar(const Type *type);
int type_equal(const Type *left, const Type *right);
int type_assignable(const Type *destination, const Type *source);
Type *type_common_numeric(Type *left, Type *right);
const char *type_kind_name(TypeKind kind);
void type_print(FILE *stream, const Type *type);

#endif
