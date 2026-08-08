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

typedef struct Symbol {
    char *name;
    SymbolKind kind;
    Type *type;
    StorageClass storage;
    long offset;
    size_t size;
    int line;
    int defined;
    struct Symbol *next;
} Symbol;

typedef struct Scope Scope;

void symtab_init(void);
void symtab_finish(void);
Scope *symtab_enter_scope(const char *label);
void symtab_leave_scope(void);
Scope *symtab_current_scope(void);
const char *symtab_current_label(void);
int symtab_current_depth(void);

Symbol *symtab_declare_full(const char *name, SymbolKind kind, Type *type,
                            StorageClass storage, int line, int defined);
/* Compatibility entry point used by unchanged Stage-1 declaration actions. */
int symtab_declare(const char *name);
Symbol *symtab_lookup(const char *name);
Symbol *symtab_lookup_current(const char *name);
Symbol *symtab_lookup_in_scope(Scope *scope, const char *name);
void symtab_set_scope_type(Scope *scope, Type *type);
void symtab_print_scope(const Scope *scope);
void symtab_print_all(void);

const char *symbol_kind_name(SymbolKind kind);
const char *storage_class_name(StorageClass storage);

#endif
