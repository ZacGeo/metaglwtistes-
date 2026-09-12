#define _POSIX_C_SOURCE 200809L
#include "ir.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Κεντρικές λίστες της ενδιάμεσης αναπαράστασης. */
static AstNode *global_declarations;
static IrFunction *first_function;
static IrFunction *last_function;

/* Τερματίζει αν αποτύχει δέσμευση μνήμης. */
static void die(void)
{
    fputs("Out of memory\n", stderr);
    exit(EXIT_FAILURE);
}

/* Αρχικοποιεί την ενδιάμεση αναπαράσταση. */
void ir_init(void)
{
    global_declarations = NULL;
    first_function = NULL;
    last_function = NULL;
}

/* Προσθέτει καθολικές δηλώσεις. */
void ir_add_global(AstNode *declarations)
{
    global_declarations = ast_append(global_declarations, declarations);
}

/* Προσθέτει μία συνάρτηση στην IR. */
void ir_add_function(const char *name, Type *return_type, Type *owner_type,
                     Symbol *symbol, Scope *scope, AstNode *body)
{
    IrFunction *function = calloc(1, sizeof(*function));
    if (!function) die();
    function->name = strdup(name ? name : "<anonymous>");
    if (!function->name) die();
    function->return_type = return_type;
    function->owner_type = owner_type;
    function->symbol = symbol;
    function->scope = scope;
    function->body = body;
    if (last_function) last_function->next = function;
    else first_function = function;
    last_function = function;
}

/* Επιστρέφει τις καθολικές δηλώσεις. */
AstNode *ir_globals(void) { return global_declarations; }

/* Επιστρέφει τη λίστα συναρτήσεων. */
IrFunction *ir_functions(void) { return first_function; }

/* Εκτυπώνει την ενδιάμεση αναπαράσταση. */
void ir_print(FILE *stream)
{
    IrFunction *function;
    if (global_declarations) {
        fputs("\n=== INTERMEDIATE CODE: globals ===\n", stream);
        ast_print(stream, global_declarations, 0);
    }
    for (function = first_function; function; function = function->next) {
        fprintf(stream, "\n=== INTERMEDIATE CODE: %s ===\n", function->name);
        ast_print(stream, function->body, 0);
    }
}

/* Αποδεσμεύει όλη την ενδιάμεση αναπαράσταση. */
void ir_finish(void)
{
    IrFunction *function = first_function;
    ast_free(global_declarations);
    global_declarations = NULL;
    while (function) {
        IrFunction *next = function->next;
        ast_free(function->body);
        free(function->name);
        free(function);
        function = next;
    }
    first_function = last_function = NULL;
}
