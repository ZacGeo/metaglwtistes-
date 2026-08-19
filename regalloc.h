#ifndef CPP_REGALLOC_H
#define CPP_REGALLOC_H

#include "ir.h"

#define MIPS_TEMP_REGISTER_COUNT 8

typedef struct RegisterAllocation {
    int temporary_count;
    int colored_count;
    int spilled_count;
    long spill_bytes;
} RegisterAllocation;

RegisterAllocation regalloc_function(IrFunction *function, long first_spill_offset);
const char *regalloc_integer_name(int color);
const char *regalloc_float_name(int color);

#endif
