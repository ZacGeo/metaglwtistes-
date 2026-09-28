#ifndef CPP_IR_H
#define CPP_IR_H

#include "ast.h"
#include "symtab.h"

/* Μία συνάρτηση της ενδιάμεσης αναπαράστασης μαζί με το scope της. */
typedef struct IrFunction {
    char *name;
    Type *return_type;
    Type *owner_type;
    Symbol *symbol;
    Scope *scope;
    AstNode *body;
    struct IrFunction *next;
} IrFunction;

/* Δημιουργία και ενημέρωση των global λιστών της IR. */
void ir_init(void);
void ir_add_global(AstNode *declarations);
void ir_add_function(const char *name, Type *return_type, Type *owner_type,
                     Symbol *symbol, Scope *scope, AstNode *body);
AstNode *ir_globals(void);
IrFunction *ir_functions(void);

/* Debug εκτύπωση και αποδέσμευση της IR. */
void ir_print(FILE *stream);
void ir_finish(void);

#endif
