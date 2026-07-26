/* ============================================================
   symtab.c  —  Υλοποίηση Πίνακα Συμβόλων για CPP
   ============================================================ */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* ── Hash table size ── */
#define HASH_SIZE 256

/* ── Εγγραφή συμβόλου ── */
typedef struct Symbol {
    char          *name;
    int            depth;
    struct Symbol *next;   /* αλυσίδα hash bucket */
} Symbol;

/* ── Εμβέλεια ── */
typedef struct Scope {
    char          *label;              /* περιγραφή π.χ. "function add" */
    int            depth;
    Symbol        *table[HASH_SIZE];
    struct Scope  *parent;
} Scope;

static Scope *current = NULL;

/* ── hash (djb2) ── */
static unsigned int hash(const char *s)
{
    unsigned int h = 5381;
    while (*s) h = ((h << 5) + h) ^ (unsigned char)(*s++);
    return h % HASH_SIZE;
}

/* ============================================================
   Αρχικοποίηση
   ============================================================ */
void symtab_init(void)
{
    current = NULL;
    symtab_enter_scope("global");
}

/* ============================================================
   Εμβέλειες
   ============================================================ */
void symtab_enter_scope(const char *label)
{
    Scope *s = calloc(1, sizeof(Scope));
    if (!s) { fprintf(stderr, "Out of memory\n"); exit(1); }
    s->label  = label ? strdup(label) : strdup("?");
    s->depth  = current ? current->depth + 1 : 0;
    s->parent = current;
    current   = s;
}

static void print_scope(Scope *s)
{
    printf("\nScope [depth=%d]: %s\n", s->depth, s->label);

    int found = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        Symbol *sym = s->table[i];
        while (sym) {
            printf("  %-30s  (depth %d)\n", sym->name, sym->depth);
            found++;
            sym = sym->next;
        }
    }
    if (!found)
        printf("  (empty scope)\n");
    printf("\n");
}

static void free_scope(Scope *s)
{
    for (int i = 0; i < HASH_SIZE; i++) {
        Symbol *sym = s->table[i];
        while (sym) {
            Symbol *next = sym->next;
            free(sym->name);
            free(sym);
            sym = next;
        }
    }
    free(s->label);
    free(s);
}

void symtab_leave_scope(void)
{
    if (!current) return;
    print_scope(current);
    Scope *old = current;
    current    = current->parent;
    free_scope(old);
}

void symtab_finish(void)
{
    /* Κλείνει όσες εμβέλειες έχουν μείνει ανοιχτές
       (π.χ. σε περίπτωση abort λόγω σφαλμάτων) */
    while (current)
        symtab_leave_scope();
}

/* ============================================================
   Δήλωση / Αναζήτηση
   ============================================================ */
int symtab_declare(const char *name)
{
    if (!current || !name) return -1;

    unsigned int h = hash(name);

    /* Έλεγχος διπλής δήλωσης στην ΙΔΙΑ εμβέλεια */
    Symbol *s = current->table[h];
    while (s) {
        if (strcmp(s->name, name) == 0) {
            fprintf(stderr,
                "Semantic error: '%s' already declared in scope '%s'\n",
                name, current->label);
            return -1;
        }
        s = s->next;
    }

    /* Εισαγωγή */
    Symbol *sym = malloc(sizeof(Symbol));
    if (!sym) { fprintf(stderr, "Out of memory\n"); exit(1); }
    sym->name  = strdup(name);
    sym->depth = current->depth;
    sym->next  = current->table[h];
    current->table[h] = sym;
    return 0;
}

int symtab_lookup(const char *name)
{
    Scope *sc = current;
    while (sc) {
        unsigned int h = hash(name);
        Symbol *s = sc->table[h];
        while (s) {
            if (strcmp(s->name, name) == 0) return 1;
            s = s->next;
        }
        sc = sc->parent;
    }
    return 0;
}