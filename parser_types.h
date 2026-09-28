#ifndef CPP_PARSER_TYPES_H
#define CPP_PARSER_TYPES_H

#include "ast.h"
#include "types.h"

/* Μία διάσταση array declaration, με -1 για omitted dimension. */
typedef struct DimList {
    long long length;
    struct DimList *next;
} DimList;

/* Προσωρινή περιγραφή declarator πριν δημιουργηθεί το σύμβολο. */
typedef struct DeclItem {
    char *name;
    int is_list;
    int line;
    DimList *dimensions;
    AstNode *initializer;
    struct DeclItem *next;
} DeclItem;

#endif
