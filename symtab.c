#define _POSIX_C_SOURCE 200809L
#include "symtab.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 257

struct Scope {
    char *label;
    int depth;
    long next_offset;
    long total_size;
    Type *owner_type;
    AccessKind current_access;
    int anonymous_union_active;
    long anonymous_union_base;
    long anonymous_union_size;
    size_t anonymous_union_alignment;
    Symbol *table[HASH_SIZE];
    Symbol *first_symbol;
    Symbol *last_symbol;
    Scope *parent;
    Scope *first_child;
    Scope *last_child;
    Scope *next_sibling;
};

static Scope *root_scope;
static Scope *current_scope;
static long global_static_offset;

static void die(void) { fprintf(stderr, "Out of memory\n"); exit(EXIT_FAILURE); }

static unsigned hash_name(const char *name)
{
    unsigned hash = 5381;
    while (*name) hash = ((hash << 5) + hash) ^ (unsigned char)tolower((unsigned char)*name++);
    return hash % HASH_SIZE;
}

static int same_name(const char *left, const char *right)
{
    while (*left && *right) {
        if (tolower((unsigned char)*left++) != tolower((unsigned char)*right++)) return 0;
    }
    return *left == *right;
}

static long align_up(long value, size_t alignment)
{
    long a = alignment ? (long)alignment : 1;
    return (value + a - 1) / a * a;
}

void symtab_init(void)
{
    root_scope = current_scope = NULL;
    global_static_offset = 0;
    symtab_enter_scope("global");
}

Scope *symtab_enter_scope(const char *label)
{
    Scope *scope = calloc(1, sizeof(*scope));
    if (!scope) die();
    scope->label = strdup(label ? label : "?");
    if (!scope->label) die();
    scope->parent = current_scope;
    scope->depth = current_scope ? current_scope->depth + 1 : 0;
    scope->current_access = ACCESS_PRIVATE;
    if (current_scope) {
        if (current_scope->last_child) current_scope->last_child->next_sibling = scope;
        else current_scope->first_child = scope;
        current_scope->last_child = scope;
    } else root_scope = scope;
    current_scope = scope;
    return scope;
}

void symtab_leave_scope(void)
{
    if (!current_scope) return;
    if (current_scope->owner_type && current_scope->owner_type->alignment > 0) {
        current_scope->total_size = align_up(current_scope->total_size,
                                             current_scope->owner_type->alignment);
        current_scope->owner_type->size = (size_t)current_scope->total_size;
    }
    symtab_print_scope(current_scope);
    current_scope = current_scope->parent;
}

Scope *symtab_current_scope(void) { return current_scope; }
Scope *symtab_root_scope(void) { return root_scope; }
long symtab_global_size(void) { return global_static_offset; }
long symtab_scope_total_size(const Scope *scope) { return scope ? scope->total_size : 0; }
const char *symtab_scope_label(const Scope *scope) { return scope ? scope->label : "<none>"; }
Symbol *symtab_scope_first_symbol(const Scope *scope) { return scope ? scope->first_symbol : NULL; }
const char *symtab_current_label(void) { return current_scope ? current_scope->label : "<none>"; }
int symtab_current_depth(void) { return current_scope ? current_scope->depth : -1; }
Type *symtab_current_owner_type(void) { return current_scope ? current_scope->owner_type : NULL; }
void symtab_set_current_access(AccessKind access) { if (current_scope) current_scope->current_access = access; }
AccessKind symtab_current_access(void) { return current_scope ? current_scope->current_access : ACCESS_PUBLIC; }

Symbol *symtab_lookup_in_scope(Scope *scope, const char *name)
{
    Symbol *symbol;
    if (!scope || !name) return NULL;
    for (symbol = scope->table[hash_name(name)]; symbol; symbol = symbol->next)
        if (same_name(symbol->name, name)) return symbol;
    return NULL;
}

Symbol *symtab_lookup_current(const char *name)
{
    return symtab_lookup_in_scope(current_scope, name);
}

Symbol *symtab_lookup(const char *name)
{
    Scope *scope;
    for (scope = current_scope; scope; scope = scope->parent) {
        Symbol *symbol = symtab_lookup_in_scope(scope, name);
        if (symbol) return symbol;
    }
    return NULL;
}

Symbol *symtab_declare_full(const char *name, SymbolKind kind, Type *type,
                            StorageClass storage, int line, int defined)
{
    unsigned bucket;
    Symbol *symbol;
    long *cursor;
    if (!current_scope || !name || symtab_lookup_current(name)) return NULL;
    symbol = calloc(1, sizeof(*symbol));
    if (!symbol) die();
    symbol->name = strdup(name);
    if (!symbol->name) die();
    symbol->kind = kind; symbol->type = type; symbol->storage = storage;
    symbol->scope = current_scope;
    symbol->line = line; symbol->defined = defined;
    symbol->access = current_scope->current_access;
    symbol->owner_type = current_scope->owner_type;
    symbol->size = type ? type->size : 0;
    if (kind == SYMBOL_FIELD && current_scope->anonymous_union_active) {
        long end;
        symbol->offset = current_scope->anonymous_union_base;
        symbol->anonymous_union_member = 1;
        if (symbol->type && symbol->type->alignment > current_scope->anonymous_union_alignment)
            current_scope->anonymous_union_alignment = symbol->type->alignment;
        end = (long)symbol->size;
        if (end > current_scope->anonymous_union_size) current_scope->anonymous_union_size = end;
        cursor = NULL;
    } else if (kind == SYMBOL_FIELD && current_scope->owner_type &&
        current_scope->owner_type->kind == TYPE_UNION) {
        symbol->offset = 0;
        if ((long)symbol->size > current_scope->total_size) current_scope->total_size = (long)symbol->size;
        if (symbol->type && symbol->type->alignment > current_scope->owner_type->alignment)
            current_scope->owner_type->alignment = symbol->type->alignment;
        current_scope->owner_type->size = (size_t)current_scope->total_size;
        cursor = NULL;
    } else if (storage == STORAGE_GLOBAL || storage == STORAGE_STATIC_LOCAL) cursor = &global_static_offset;
    else cursor = &current_scope->next_offset;
    if (cursor && storage != STORAGE_NONE && symbol->size > 0) {
        *cursor = align_up(*cursor, type ? type->alignment : 1);
        symbol->offset = *cursor;
        *cursor += (long)symbol->size;
        if (storage != STORAGE_GLOBAL && storage != STORAGE_STATIC_LOCAL)
            current_scope->total_size = current_scope->next_offset;
        if (kind == SYMBOL_FIELD && current_scope->owner_type) {
            current_scope->owner_type->size = (size_t)current_scope->total_size;
            if (type && type->alignment > current_scope->owner_type->alignment)
                current_scope->owner_type->alignment = type->alignment;
        }
    }
    bucket = hash_name(name);
    symbol->next = current_scope->table[bucket];
    current_scope->table[bucket] = symbol;
    if (current_scope->last_symbol) current_scope->last_symbol->order_next = symbol;
    else current_scope->first_symbol = symbol;
    current_scope->last_symbol = symbol;
    return symbol;
}

Symbol *symtab_declare_parent_full(const char *name, SymbolKind kind, Type *type,
                                   StorageClass storage, int line, int defined)
{
    Scope *saved = current_scope;
    Symbol *symbol;
    if (!saved || !saved->parent) return NULL;
    current_scope = saved->parent;
    symbol = symtab_declare_full(name, kind, type, storage, line, defined);
    current_scope = saved;
    return symbol;
}

void symtab_set_constant_value(Symbol *symbol, ConstantValueKind kind,
                               long long int_value, double float_value,
                               const char *text)
{
    if (!symbol) return;
    free(symbol->constant_text);
    symbol->constant_text = text ? strdup(text) : NULL;
    if (text && !symbol->constant_text) die();
    symbol->constant_kind = kind;
    symbol->int_value = int_value;
    symbol->float_value = float_value;
}

int symtab_declare(const char *name)
{
    return symtab_declare_full(name, SYMBOL_VARIABLE, type_error(), STORAGE_NONE, 0, 1) ? 0 : -1;
}

void symtab_set_scope_type(Scope *scope, Type *type)
{
    if (scope) {
        scope->owner_type = type;
        if (type) type->members = scope;
    }
}

void symtab_set_class_parent(Type *derived, Type *base, int line)
{
    (void)line;
    if (!current_scope || !derived || !base) return;
    type_set_parent(derived, base);
    current_scope->next_offset = align_up((long)base->size,
                                          base->alignment ? base->alignment : 1);
    current_scope->total_size = current_scope->next_offset;
    if (base->alignment > derived->alignment) derived->alignment = base->alignment;
    derived->size = (size_t)current_scope->total_size;
}

Symbol *symtab_lookup_member(Type *type, const char *name, Type **declaring_type)
{
    Type *cursor;
    if (declaring_type) *declaring_type = NULL;
    for (cursor = type; cursor; cursor = cursor->parent) {
        Symbol *symbol = symtab_lookup_in_scope((Scope *)cursor->members, name);
        if (symbol) {
            if (declaring_type) *declaring_type = cursor;
            return symbol;
        }
    }
    return NULL;
}

Symbol *symtab_first_member(Type *type)
{
    Scope *scope = type ? (Scope *)type->members : NULL;
    return scope ? scope->first_symbol : NULL;
}

Symbol *symtab_next_declared(Symbol *symbol)
{
    return symbol ? symbol->order_next : NULL;
}

void symtab_begin_anonymous_union(void)
{
    if (!current_scope || current_scope->anonymous_union_active) return;
    current_scope->anonymous_union_active = 1;
    current_scope->anonymous_union_base = current_scope->next_offset;
    current_scope->anonymous_union_size = 0;
    current_scope->anonymous_union_alignment = 1;
}

void symtab_end_anonymous_union(void)
{
    long base;
    int bucket;
    if (!current_scope || !current_scope->anonymous_union_active) return;
    base = align_up(current_scope->anonymous_union_base,
                    current_scope->anonymous_union_alignment);
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol;
        for (symbol = current_scope->table[bucket]; symbol; symbol = symbol->next) {
            if (symbol->anonymous_union_member &&
                symbol->offset == current_scope->anonymous_union_base) {
                symbol->offset = base;
                symbol->anonymous_union_member = 0;
            }
        }
    }
    current_scope->next_offset = base + align_up(current_scope->anonymous_union_size,
                                                  current_scope->anonymous_union_alignment);
    current_scope->total_size = current_scope->next_offset;
    if (current_scope->owner_type) {
        if (current_scope->anonymous_union_alignment > current_scope->owner_type->alignment)
            current_scope->owner_type->alignment = current_scope->anonymous_union_alignment;
        current_scope->owner_type->size = (size_t)current_scope->total_size;
    }
    current_scope->anonymous_union_active = 0;
}

const char *symbol_kind_name(SymbolKind kind)
{
    static const char *names[] = { "type", "constant", "variable", "function", "parameter",
        "field", "method", "enum-value" };
    return names[(int)kind];
}

const char *storage_class_name(StorageClass storage)
{
    static const char *names[] = { "none", "global", "stack", "static-local", "param-value", "param-ref", "member" };
    return names[(int)storage];
}

const char *access_kind_name(AccessKind access)
{
    static const char *names[] = { "private", "protected", "public" };
    return names[(int)access];
}

void symtab_print_scope(const Scope *scope)
{
    int bucket, count = 0;
    printf("\n=== SYMBOLS / DATA SPACE: %s (depth=%d, local-bytes=%ld) ===\n",
           scope->label, scope->depth, scope->total_size);
    printf("%-20s %-12s %-20s %-14s %-10s %-11s %8s %8s  %s\n",
           "name", "kind", "type", "storage", "access", "state", "offset", "size", "value");
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol;
        for (symbol = scope->table[bucket]; symbol; symbol = symbol->next) {
            printf("%-20s %-12s ", symbol->name, symbol_kind_name(symbol->kind));
            type_print(stdout, symbol->type);
            printf("%-*s %-14s %-10s %-11s %8ld %8zu  ", 20, "", storage_class_name(symbol->storage),
                   symbol->owner_type ? access_kind_name(symbol->access) : "-",
                   (symbol->kind == SYMBOL_FUNCTION || symbol->kind == SYMBOL_METHOD)
                       ? (symbol->defined ? "definition" : "prototype") : "-",
                   symbol->offset, symbol->size);
            switch (symbol->constant_kind) {
            case CONST_VALUE_INT: fprintf(stdout, "%lld", symbol->int_value); break;
            case CONST_VALUE_FLOAT: fprintf(stdout, "%.17g", symbol->float_value); break;
            case CONST_VALUE_CHAR:
            case CONST_VALUE_STRING: fprintf(stdout, "%s", symbol->constant_text ? symbol->constant_text : ""); break;
            case CONST_VALUE_AGGREGATE: fputs("<initializer-list>", stdout); break;
            default: fputc('-', stdout); break;
            }
            fputc('\n', stdout);
            count++;
        }
    }
    if (!count) puts("(empty)");
}

static void print_tree(const Scope *scope)
{
    const Scope *child;
    if (!scope) return;
    symtab_print_scope(scope);
    for (child = scope->first_child; child; child = child->next_sibling) print_tree(child);
}

void symtab_print_all(void) { print_tree(root_scope); }

static void visit_tree(Scope *scope, void (*visitor)(Symbol *, void *), void *context)
{
    int bucket;
    Scope *child;
    if (!scope || !visitor) return;
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol;
        for (symbol = scope->table[bucket]; symbol; symbol = symbol->next)
            visitor(symbol, context);
    }
    for (child = scope->first_child; child; child = child->next_sibling)
        visit_tree(child, visitor, context);
}

void symtab_visit_symbols(void (*visitor)(Symbol *, void *), void *context)
{
    visit_tree(root_scope, visitor, context);
}

static void free_scope(Scope *scope)
{
    int bucket;
    Scope *child = scope->first_child;
    while (child) { Scope *next = child->next_sibling; free_scope(child); child = next; }
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol = scope->table[bucket];
        while (symbol) { Symbol *next = symbol->next; free(symbol->name); free(symbol->constant_text); free(symbol); symbol = next; }
    }
    free(scope->label); free(scope);
}

void symtab_finish(void)
{
    while (current_scope) current_scope = current_scope->parent;
    free_scope(root_scope);
    root_scope = NULL;
}
