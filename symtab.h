#ifndef CPP_SYMTAB_H
#define CPP_SYMTAB_H

#include "types.h"

typedef enum SymbolKind {
    SYMBOL_TYPE, SYMBOL_CONSTANT, SYMBOL_VARIABLE, SYMBOL_FUNCTION,
    SYMBOL_PARAMETER, SYMBOL_FIELD, SYMBOL_METHOD, SYMBOL_ENUM_VALUE
} SymbolKind;

typedef enum StorageClass {
    STORAGE_NONE, STORAGE_GLOBAL, STORAGE_STACK, STORAGE_STATIC_LOCAL,
    STORAGE_PARAMETER_VALUE, STORAGE_PARAMETER_REFERENCE, STORAGE_MEMBER
} StorageClass;

typedef enum AccessKind {
    ACCESS_PRIVATE,
    ACCESS_PROTECTED,
    ACCESS_PUBLIC
} AccessKind;

typedef struct Symbol {
    char *name;
    SymbolKind kind;
    Type *type;
    StorageClass storage;
    long offset;
    size_t size;
    int line;
    int defined;
    ConstantValueKind constant_kind;
    long long int_value;
    double float_value;
    char *constant_text;
    AccessKind access;
    Type *owner_type;
    unsigned anonymous_union_member : 1;
    struct Symbol *next;
    struct Symbol *order_next;
} Symbol;

typedef struct Scope Scope;

void symtab_init(void);
void symtab_finish(void);
Scope *symtab_enter_scope(const char *label);
void symtab_leave_scope(void);
Scope *symtab_current_scope(void);
const char *symtab_current_label(void);
int symtab_current_depth(void);
Type *symtab_current_owner_type(void);
void symtab_set_current_access(AccessKind access);
AccessKind symtab_current_access(void);

Symbol *symtab_declare_full(const char *name, SymbolKind kind, Type *type,
                            StorageClass storage, int line, int defined);
Symbol *symtab_declare_parent_full(const char *name, SymbolKind kind, Type *type,
                                   StorageClass storage, int line, int defined);
void symtab_set_constant_value(Symbol *symbol, ConstantValueKind kind,
                               long long int_value, double float_value,
                               const char *text);
/* Compatibility entry point used by unchanged Stage-1 declaration actions. */
int symtab_declare(const char *name);
Symbol *symtab_lookup(const char *name);
Symbol *symtab_lookup_current(const char *name);
Symbol *symtab_lookup_in_scope(Scope *scope, const char *name);
void symtab_set_scope_type(Scope *scope, Type *type);
void symtab_set_class_parent(Type *derived, Type *base, int line);
Symbol *symtab_lookup_member(Type *type, const char *name, Type **declaring_type);
Symbol *symtab_first_member(Type *type);
Symbol *symtab_next_declared(Symbol *symbol);
void symtab_begin_anonymous_union(void);
void symtab_end_anonymous_union(void);
void symtab_visit_symbols(void (*visitor)(Symbol *, void *), void *context);
void symtab_print_scope(const Scope *scope);
void symtab_print_all(void);

const char *symbol_kind_name(SymbolKind kind);
const char *storage_class_name(StorageClass storage);
const char *access_kind_name(AccessKind access);

#endif
