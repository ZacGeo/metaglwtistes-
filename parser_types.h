#ifndef CPP_PARSER_TYPES_H
#define CPP_PARSER_TYPES_H

#include "ast.h"
#include "types.h"

typedef struct DimList {
    long long length;
    struct DimList *next;
} DimList;

typedef struct DeclItem {
    char *name;
    int is_list;
    int line;
    DimList *dimensions;
    AstNode *initializer;
    struct DeclItem *next;
} DeclItem;

#endif
