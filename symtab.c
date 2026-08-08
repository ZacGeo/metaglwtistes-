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
    Symbol *table[HASH_SIZE];
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
    symtab_print_scope(current_scope);
    current_scope = current_scope->parent;
}

Scope *symtab_current_scope(void) { return current_scope; }
const char *symtab_current_label(void) { return current_scope ? current_scope->label : "<none>"; }
int symtab_current_depth(void) { return current_scope ? current_scope->depth : -1; }

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
    symbol->line = line; symbol->defined = defined;
    symbol->size = type ? type->size : 0;
    if (kind == SYMBOL_FIELD && current_scope->owner_type &&
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
    return symbol;
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

void symtab_print_scope(const Scope *scope)
{
    int bucket, count = 0;
    printf("\n=== SYMBOLS / DATA SPACE: %s (depth=%d, local-bytes=%ld) ===\n",
           scope->label, scope->depth, scope->total_size);
    printf("%-20s %-12s %-20s %-14s %8s %8s\n",
           "name", "kind", "type", "storage", "offset", "size");
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol;
        for (symbol = scope->table[bucket]; symbol; symbol = symbol->next) {
            printf("%-20s %-12s ", symbol->name, symbol_kind_name(symbol->kind));
            type_print(stdout, symbol->type);
            printf("%-*s %-14s %8ld %8zu\n", 20, "", storage_class_name(symbol->storage),
                   symbol->offset, symbol->size);
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

static void free_scope(Scope *scope)
{
    int bucket;
    Scope *child = scope->first_child;
    while (child) { Scope *next = child->next_sibling; free_scope(child); child = next; }
    for (bucket = 0; bucket < HASH_SIZE; bucket++) {
        Symbol *symbol = scope->table[bucket];
        while (symbol) { Symbol *next = symbol->next; free(symbol->name); free(symbol); symbol = next; }
    }
    free(scope->label); free(scope);
}

void symtab_finish(void)
{
    while (current_scope) current_scope = current_scope->parent;
    free_scope(root_scope);
    root_scope = NULL;
}
