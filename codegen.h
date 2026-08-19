#ifndef CPP_CODEGEN_H
#define CPP_CODEGEN_H

#include "ir.h"

/* Generate the final MIPS assembly after successful semantic analysis. */
int codegen_mips(const char *output_path);

#endif
