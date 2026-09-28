#ifndef CPP_REGALLOC_H
#define CPP_REGALLOC_H

#include "ir.h"

/* Πλήθος προσωρινών MIPS registers που εξετάζει ο allocator. */
#define MIPS_TEMP_REGISTER_COUNT 8

/* Συνοπτικά αποτελέσματα register coloring και spilling. */
typedef struct RegisterAllocation {
    int temporary_count;
    int colored_count;
    int spilled_count;
    long spill_bytes;
} RegisterAllocation;

/* Εκτέλεση allocation για μία συνάρτηση και ονόματα registers ανά color. */
RegisterAllocation regalloc_function(IrFunction *function, long first_spill_offset);
const char *regalloc_integer_name(int color);
const char *regalloc_float_name(int color);

#endif
