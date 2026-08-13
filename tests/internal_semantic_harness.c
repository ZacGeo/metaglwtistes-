#include <assert.h>

#include "../ast.h"
#include "../semantic.h"
#include "../symtab.h"
#include "../types.h"

int main(void)
{
    AstNode *sum;
    AstNode *items;
    AstNode *initializer;
    AstNode *identifier;
    Symbol *constant;
    Type *inferred_array;

    types_init();
    symtab_init();
    semantic_init();

    sum = semantic_binary("+", ast_int(2, 1), ast_int(3, 1), 1);
    assert(sum->is_constant);
    assert(sum->constant_kind == CONST_VALUE_INT);
    assert(sum->int_value == 5);

    constant = semantic_declare("limit", SYMBOL_CONSTANT, type_int(),
                                STORAGE_NONE, 1, 1);
    semantic_store_constant(constant, sum, 1);
    identifier = semantic_identifier("limit", 2);
    assert(identifier->is_constant);
    assert(identifier->int_value == 5);

    items = ast_append(ast_int(10, 3), ast_int(20, 3));
    initializer = ast_init_list(items, 3);
    inferred_array = type_array(type_int(), -1);
    semantic_validate_initializer(inferred_array, initializer, "values", 3);
    assert(inferred_array->array_length == 2);
    assert(inferred_array->size == 8);

    ast_free(identifier);
    ast_free(sum);
    ast_free(initializer);
    symtab_finish();
    semantic_finish();
    types_finish();
    return 0;
}
