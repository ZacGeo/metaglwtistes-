#ifndef CPP_CODEGEN_H
#define CPP_CODEGEN_H

/* Παραγωγή του τελικού MIPS assembly από την IR και το AST. */

#include "ir.h"

/* Generate the final MIPS assembly after successful semantic analysis. */
int codegen_mips(const char *output_path);

#endif
