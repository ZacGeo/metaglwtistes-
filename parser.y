%{
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diagnostics.h"
#include "symtab.h"
#include "ast.h"
#include "semantic.h"
#include "types.h"
#include "parser_types.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
void lexer_set_trace(int enabled);

void yyerror(const char *message);

static char *pending_function_name = NULL;

typedef struct PendingParameter {
    char *name;
    Type *type;
    PassMode mode;
    struct PendingParameter *next;
} PendingParameter;

static PendingParameter *pending_parameters = NULL;
static PendingParameter *pending_parameters_tail = NULL;
static Type *pending_function_return_type = NULL;

static Type *declaration_base_type = NULL;
static SymbolKind declaration_symbol_kind = SYMBOL_VARIABLE;

static void declare_identifier(char *name);
static void discard_text(char *text);
static void enter_named_scope(const char *kind, const char *name);
static void remember_function(char *name);
static void enter_function_scope(void);
static void finish_function_scope(void);
static void remember_parameter(char *name, Type *type, PassMode mode);
static void declare_pending_parameters(void);
static void clear_pending_parameters(void);
static void register_pending_function(int definition);
static int implicit_reference(Type *type);
static DimList *dim_new(long long length);
static DimList *dim_append(DimList *list, DimList *item);
static Type *apply_declarator(Type *base, int is_list, DimList *dimensions, int line);
static void dim_free(DimList *dimensions);
static DeclItem *decl_new(char *name, int is_list, DimList *dimensions, int line);
static DeclItem *decl_append(DeclItem *list, DeclItem *item);
static void process_declarations(Type *base, DeclItem *items, SymbolKind kind,
                                 StorageClass storage);
%}

%error-verbose
%start program

%union {
    long long ival;
    double fval;
    char *sval;
    Type *type;
    AstNode *node;
    DimList *dims;
    DeclItem *decl;
}

/* Keywords. */
%token TYPEDEF CHAR INT FLOAT STRING CONST CLASS
%token PRIVATE PROTECTED PUBLIC VOID STATIC UNION ENUM LIST
%token CONTINUE BREAK IF ELSE WHILE FOR RETURN
%token LENGTH CIN COUT MAIN THIS SIZEOP

/* Operators. */
%token <sval> OROP ANDOP EQUOP RELOP ADDOP MULOP NOTOP INCDEC
%token METH INP OUT REFER

/* Separators. */
%token LPAREN RPAREN SEMI DOT COMMA ASSIGN COLON
%token LBRACK RBRACK LBRACE RBRACE

/* Tokens carrying values through yylval. */
%token <ival> ICONST
%token <fval> FCONST
%token <sval> CCONST SCONST ID LISTFUNC

%type <type> typename standard_type
%type <node> expression or_expression and_expression equality_expression
%type <node> relational_expression additive_expression multiplicative_expression
%type <node> unary_expression postfix_expression variable general_expression assignment
%type <node> expression_list constant listexpression init_value init_values initializer
%type <node> optexpr statement statements expression_statement if_statement
%type <node> while_statement for_statement return_statement io_statement
%type <node> in_list in_item out_list out_item comp_statement decl_statements
%type <decl> variabledef variabledefs init_variabledef init_variabledefs
%type <node> function_body
%type <ival> listspec decltype
%type <dims> dims dim
%type <sval> func_class

%destructor { free($$); } <sval>

/* Resolve the classic dangling-else ambiguity. */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

program
    : global_declarations main_function
    ;

global_declarations
    : global_declarations global_declaration
    | /* empty */
    ;

global_declaration
    : typedef_declaration
    | const_declaration
    | enum_declaration
    | class_declaration
    | union_declaration
    | global_var_declaration
    | func_declaration
    | error SEMI
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

typedef_declaration
    : TYPEDEF typename listspec ID dims SEMI
      {
          Type *alias = apply_declarator($2, (int)$3, $5, yylineno);
          semantic_declare($4, SYMBOL_TYPE, alias, STORAGE_NONE, yylineno, 1);
          dim_free($5);
          discard_text($4);
      }
    ;

typename
    : standard_type
    | ID
      {
          $$ = semantic_resolve_type($1, yylineno);
          discard_text($1);
      }
    ;

standard_type
    : CHAR   { $$ = type_char(); }
    | INT    { $$ = type_int(); }
    | FLOAT  { $$ = type_float(); }
    | STRING { $$ = type_string(); }
    | VOID   { $$ = type_void(); }
    ;

listspec
    : LIST { $$ = 1; }
    | /* empty */ { $$ = 0; }
    ;

dims
    : dims dim { $$ = dim_append($1, $2); }
    | /* empty */ { $$ = NULL; }
    ;

dim
    : LBRACK ICONST RBRACK { $$ = dim_new($2); }
    | LBRACK RBRACK { $$ = dim_new(-1); }
    ;

const_declaration
    : CONST typename
      { declaration_base_type = $2; declaration_symbol_kind = SYMBOL_CONSTANT; }
      constdefs SEMI
    ;

constdefs
    : constdefs COMMA constdef
    | constdef
    ;

constdef
    : ID dims ASSIGN init_value
      {
          Type *type = apply_declarator(declaration_base_type, 0, $2, yylineno);
          if (!type_assignable(type, $4->type))
              semantic_error(yylineno, "initializer of constant '%s' has incompatible type", $1);
          semantic_declare($1, SYMBOL_CONSTANT, type, STORAGE_NONE, yylineno, 1);
          dim_free($2);
          ast_free($4);
          discard_text($1);
      }
    ;

init_value
    : expression
    | LBRACE init_values RBRACE { $$ = $2; }
    ;

init_values
    : init_values COMMA init_value { $$ = ast_append($1, $3); }
    | init_value { $$ = $1; }
    ;

/* The ambiguous expression grammar from cpp.pdf is split into precedence
   levels. The accepted operators and operands remain those of CPP. */
expression
    : or_expression { $$ = $1; }
    ;

or_expression
    : or_expression OROP and_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | and_expression { $$ = $1; }
    ;

and_expression
    : and_expression ANDOP equality_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | equality_expression { $$ = $1; }
    ;

equality_expression
    : equality_expression EQUOP relational_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | relational_expression { $$ = $1; }
    ;

relational_expression
    : relational_expression RELOP additive_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | additive_expression { $$ = $1; }
    ;

additive_expression
    : additive_expression ADDOP multiplicative_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | multiplicative_expression { $$ = $1; }
    ;

multiplicative_expression
    : multiplicative_expression MULOP unary_expression
      { $$ = semantic_binary($2, $1, $3, yylineno); free($2); }
    | unary_expression { $$ = $1; }
    ;

unary_expression
    : NOTOP unary_expression
      { $$ = semantic_unary($1, $2, yylineno); free($1); }
    | ADDOP unary_expression
      { $$ = semantic_unary($1, $2, yylineno); free($1); }
    | SIZEOP unary_expression { $$ = semantic_unary("sizeof", $2, yylineno); }
    | INCDEC variable
      { $$ = semantic_unary($1, $2, yylineno); free($1); }
    | postfix_expression { $$ = $1; }
    ;

postfix_expression
    : variable INCDEC
      { $$ = semantic_unary($2, $1, yylineno); free($2); }
    | variable { $$ = $1; }
    | variable LPAREN expression_list RPAREN { $$ = semantic_call($1, $3, yylineno); }
    | LENGTH LPAREN general_expression RPAREN
      { $$ = ast_unary("length", $3, type_int(), yylineno); }
    | constant { $$ = $1; }
    | LPAREN general_expression RPAREN { $$ = $2; }
    | LPAREN standard_type RPAREN
      { $$ = ast_new(AST_EMPTY, yylineno, "type-expression"); $$->type = $2; }
    | listexpression { $$ = $1; }
    ;

variable
    : variable LBRACK general_expression RBRACK
      { $$ = semantic_index($1, $3, yylineno); }
    | variable DOT ID
      {
          $$ = semantic_member($1, $3, yylineno);
          discard_text($3);
      }
    | LISTFUNC LPAREN general_expression RPAREN
      {
          $$ = ast_unary($1, $3, $3->type, yylineno);
          $$->is_lvalue = 1;
          discard_text($1);
      }
    | decltype ID
      {
          $$ = semantic_identifier($2, yylineno);
          discard_text($2);
      }
    | THIS { $$ = semantic_identifier("this", yylineno); }
    ;

general_expression
    : general_expression COMMA assignment { $$ = ast_append($1, $3); }
    | assignment { $$ = $1; }
    ;

assignment
    : variable ASSIGN assignment { $$ = semantic_assignment($1, $3, yylineno); }
    | expression { $$ = $1; }
    ;

expression_list
    : general_expression { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

constant
    : CCONST
      {
          $$ = ast_text(AST_CHAR, $1, type_char(), yylineno);
          discard_text($1);
      }
    | ICONST { $$ = ast_int($1, yylineno); }
    | FCONST { $$ = ast_float($1, yylineno); }
    | SCONST
      {
          $$ = ast_text(AST_STRING, $1, type_string(), yylineno);
          discard_text($1);
      }
    ;

listexpression
    : LBRACK expression_list RBRACK
      {
          AstNode *element;
          Type *base = NULL;
          for (element = $2; element; element = element->next) {
              if (!base) base = element->type;
              else if (!type_assignable(base, element->type))
                  semantic_error(yylineno, "list elements have incompatible types");
          }
          $$ = ast_new(AST_LIST, yylineno, NULL);
          $$->type = type_list(base ? base : type_error());
          $$->child[0] = $2;
      }
    ;

enum_declaration
    : ENUM ID
      {
          Type *type = type_named(TYPE_ENUM, $2);
          semantic_declare($2, SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("enum", $2);
          symtab_set_scope_type(symtab_current_scope(), type);
          declaration_symbol_kind = SYMBOL_ENUM_VALUE;
          declaration_base_type = type;
          discard_text($2);
      }
      enum_body SEMI
      {
          symtab_leave_scope();
      }
    ;

enum_body
    : LBRACE id_list RBRACE
    | LBRACE error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

id_list
    : id_list COMMA ID initializer
      {
          declare_identifier($3);
          ast_free($4);
      }
    | ID initializer
      {
          declare_identifier($1);
          ast_free($2);
      }
    ;

initializer
    : ASSIGN init_value { $$ = $2; }
    | /* empty */ { $$ = NULL; }
    ;

class_declaration
    : CLASS ID
      {
          Type *type = type_named(TYPE_CLASS, $2);
          semantic_declare($2, SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("class", $2);
          symtab_set_scope_type(symtab_current_scope(), type);
          discard_text($2);
      }
      class_body SEMI
      {
          symtab_leave_scope();
      }
    ;

class_body
    : parent LBRACE members_methods RBRACE
    | parent LBRACE error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

parent
    : COLON ID
      {
          discard_text($2);
      }
    | /* empty */
    ;

members_methods
    : members_methods access member_or_method
    | access member_or_method
    ;

access
    : PRIVATE COLON
    | PROTECTED COLON
    | PUBLIC COLON
    | /* empty */
    ;

member_or_method
    : member
    | method
    ;

member
    : var_declaration
    | anonymous_union
    ;

var_declaration
    : typename variabledefs SEMI
      { process_declarations($1, $2, SYMBOL_FIELD, STORAGE_MEMBER); }
    ;

variabledefs
    : variabledefs COMMA variabledef { $$ = decl_append($1, $3); }
    | variabledef { $$ = $1; }
    ;

variabledef
    : listspec ID dims
      {
          $$ = decl_new($2, (int)$1, $3, yylineno);
      }
    ;

/* An anonymous union nested in a class exposes its fields in the surrounding
   class scope, so no additional symbol-table scope is opened here. */
anonymous_union
    : UNION union_body SEMI
    ;

union_body
    : LBRACE fields RBRACE
    | LBRACE error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

fields
    : fields field
    | field
    ;

field
    : var_declaration
    ;

method
    : short_func_declaration
    ;

short_func_declaration
    : short_par_func_header SEMI
      {
          register_pending_function(0);
          free(pending_function_name);
          pending_function_name = NULL;
      }
    | nopar_func_header SEMI
      {
          register_pending_function(0);
          free(pending_function_name);
          pending_function_name = NULL;
      }
    ;

short_par_func_header
    : func_header_start LPAREN parameter_types RPAREN
    ;

func_header_start
    : typename listspec ID
      {
          pending_function_return_type = $2 ? type_list($1) : $1;
          remember_function($3);
          discard_text($3);
      }
    ;

parameter_types
    : parameter_types COMMA prototype_parameter
    | prototype_parameter
    ;

prototype_parameter
    : typename REFER
      { remember_parameter(NULL, $1, PASS_REFERENCE); }
    | typename listspec dims
      {
          Type *type = apply_declarator($1, (int)$2, $3, yylineno);
          remember_parameter(NULL, type,
              implicit_reference(type) ? PASS_REFERENCE : PASS_VALUE);
          dim_free($3);
      }
    ;

nopar_func_header
    : func_header_start LPAREN RPAREN
    ;

union_declaration
    : UNION ID
      {
          Type *type = type_named(TYPE_UNION, $2);
          semantic_declare($2, SYMBOL_TYPE, type, STORAGE_NONE, yylineno, 1);
          enter_named_scope("union", $2);
          symtab_set_scope_type(symtab_current_scope(), type);
          discard_text($2);
      }
      union_body SEMI
      {
          symtab_leave_scope();
      }
    ;

global_var_declaration
    : typename init_variabledefs SEMI
      { process_declarations($1, $2, SYMBOL_VARIABLE, STORAGE_GLOBAL); }
    ;

init_variabledefs
    : init_variabledefs COMMA init_variabledef { $$ = decl_append($1, $3); }
    | init_variabledef { $$ = $1; }
    ;

init_variabledef
    : variabledef initializer
      { $$ = $1; $$->initializer = $2; }
    ;

func_declaration
    : short_func_declaration
    | full_func_declaration
    ;

full_func_declaration
    : full_par_func_header function_body
      {
          printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
          ast_print(stdout, $2, 0);
          ast_free($2);
          finish_function_scope();
      }
    | nopar_class_func_header
      {
          register_pending_function(1);
          enter_function_scope();
      }
      function_body
      {
          printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
          ast_print(stdout, $3, 0);
          ast_free($3);
          finish_function_scope();
      }
    | nopar_func_header
      {
          register_pending_function(1);
          enter_function_scope();
      }
      function_body
      {
          printf("\n=== ABSTRACT SYNTAX TREE: %s ===\n", pending_function_name);
          ast_print(stdout, $3, 0);
          ast_free($3);
          finish_function_scope();
      }
    ;

full_par_func_header
    : class_func_header_start LPAREN parameter_list RPAREN
      {
          register_pending_function(1);
          enter_function_scope();
          declare_pending_parameters();
      }
    | func_header_start LPAREN parameter_list RPAREN
      {
          register_pending_function(1);
          enter_function_scope();
          declare_pending_parameters();
      }
    ;

class_func_header_start
    : typename listspec func_class ID
      {
          size_t length = strlen($3) + strlen($4) + 1;
          char *qualified = malloc(length);
          if (!qualified) {
              fprintf(stderr, "Out of memory\n");
              exit(EXIT_FAILURE);
          }
          snprintf(qualified, length, "%s%s", $3, $4);
          pending_function_return_type = $2 ? type_list($1) : $1;
          remember_function(qualified);
          free(qualified);
          discard_text($3);
          discard_text($4);
      }
    ;

func_class
    : ID METH
      {
          size_t length = strlen($1) + 3;
          $$ = malloc(length);
          if (!$$) {
              fprintf(stderr, "Out of memory\n");
              exit(EXIT_FAILURE);
          }
          snprintf($$, length, "%s::", $1);
          discard_text($1);
      }
    ;

parameter_list
    : parameter_list COMMA named_parameter
    | named_parameter
    ;

named_parameter
    : typename REFER ID
      {
          remember_parameter($3, $1, PASS_REFERENCE);
      }
    | typename listspec ID dims
      {
          Type *type = apply_declarator($1, (int)$2, $4, yylineno);
          remember_parameter($3, type,
              implicit_reference(type) ? PASS_REFERENCE : PASS_VALUE);
          dim_free($4);
      }
    ;

nopar_class_func_header
    : class_func_header_start LPAREN RPAREN
    ;

function_body
    : LBRACE decl_statements RBRACE
      { $$ = ast_new(AST_BLOCK, yylineno, NULL); $$->child[0] = $2; }
    | LBRACE error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          $$ = ast_new(AST_BLOCK, yylineno, "syntax-error");
      }
    ;

decl_statements
    : declarations statements { $$ = $2; }
    | declarations { $$ = NULL; }
    | statements { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

declarations
    : declarations local_declaration
    | local_declaration
    ;

local_declaration
    : decltype typename variabledefs SEMI
      {
          process_declarations($2, $3, SYMBOL_VARIABLE,
              $1 ? STORAGE_STATIC_LOCAL : STORAGE_STACK);
      }
    | decltype typename error SEMI
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    | decltype typename variabledefs error
      {
          /* Common error-production rule: a missing semicolon after a
             local declaration. The unexpected lookahead is retained so
             parsing can continue with the next declaration/statement. */
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

decltype
    : STATIC { $$ = 1; }
    | /* empty */ { $$ = 0; }
    ;

statements
    : statements statement { $$ = ast_append($1, $2); }
    | statement { $$ = $1; }
    ;

statement
    : expression_statement { $$ = $1; }
    | if_statement { $$ = $1; }
    | while_statement { $$ = $1; }
    | for_statement { $$ = $1; }
    | return_statement { $$ = $1; }
    | io_statement { $$ = $1; }
    | comp_statement { $$ = $1; }
    | CONTINUE SEMI { $$ = semantic_loop_jump(AST_CONTINUE, yylineno); }
    | BREAK SEMI { $$ = semantic_loop_jump(AST_BREAK, yylineno); }
    | SEMI { $$ = ast_new(AST_EMPTY, yylineno, NULL); }
    | error SEMI
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          $$ = ast_new(AST_EMPTY, yylineno, "syntax-error");
      }
    ;

expression_statement
    : general_expression SEMI
      { $$ = $1; }
    ;

if_statement
    : IF LPAREN general_expression RPAREN statement %prec LOWER_THAN_ELSE
      {
          $$ = ast_new(AST_IF, yylineno, NULL);
          $$->child[0] = semantic_condition($3, yylineno);
          $$->child[1] = $5;
      }
    | IF LPAREN general_expression RPAREN statement ELSE statement
      {
          $$ = ast_new(AST_IF, yylineno, NULL);
          $$->child[0] = semantic_condition($3, yylineno);
          $$->child[1] = $5;
          $$->child[2] = $7;
      }
    ;

while_statement
    : WHILE LPAREN general_expression RPAREN
      { semantic_enter_loop(); }
      statement
      {
          semantic_leave_loop();
          $$ = ast_new(AST_WHILE, yylineno, NULL);
          $$->child[0] = semantic_condition($3, yylineno);
          $$->child[1] = $6;
      }
    ;

for_statement
    : FOR LPAREN optexpr SEMI optexpr SEMI optexpr RPAREN
      { semantic_enter_loop(); }
      statement
      {
          semantic_leave_loop();
          $$ = ast_new(AST_FOR, yylineno, NULL);
          $$->child[0] = $3;
          $$->child[1] = $5 ? semantic_condition($5, yylineno) : NULL;
          $$->child[2] = $7;
          $$->child[3] = $10;
      }
    ;

optexpr
    : general_expression { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

return_statement
    : RETURN optexpr SEMI { $$ = semantic_return($2, yylineno); }
    ;

io_statement
    : CIN INP in_list SEMI
      { $$ = ast_new(AST_INPUT, yylineno, NULL); $$->child[0] = $3; }
    | COUT OUT out_list SEMI
      { $$ = ast_new(AST_OUTPUT, yylineno, NULL); $$->child[0] = $3; }
    ;

in_list
    : in_list INP in_item { $$ = ast_append($1, $3); }
    | in_item { $$ = $1; }
    ;

in_item
    : variable
      {
          if (!$1->is_lvalue) semantic_error(yylineno, "cin item is not an lvalue");
          $$ = $1;
      }
    ;

out_list
    : out_list OUT out_item { $$ = ast_append($1, $3); }
    | out_item { $$ = $1; }
    ;

out_item
    : general_expression { $$ = $1; }
    ;

compound_open
    : LBRACE
      {
          symtab_enter_scope("compound block");
      }
    ;

comp_statement
    : compound_open decl_statements RBRACE
      {
          symtab_leave_scope();
          $$ = ast_new(AST_BLOCK, yylineno, NULL);
          $$->child[0] = $2;
      }
    | compound_open error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          symtab_leave_scope();
          $$ = ast_new(AST_BLOCK, yylineno, "syntax-error");
      }
    ;

main_function
    : main_header
      {
          semantic_declare_function("main", type_function(type_int(), NULL), yylineno, 1);
          symtab_enter_scope("main");
          semantic_begin_function(type_int());
      }
      function_body
      {
          puts("\n=== ABSTRACT SYNTAX TREE: main ===");
          ast_print(stdout, $3, 0);
          ast_free($3);
          semantic_end_function();
          symtab_leave_scope();
      }
    ;

main_header
    : INT MAIN LPAREN RPAREN
    ;

%%

static void declare_identifier(char *name)
{
    if (name != NULL) {
        semantic_declare(name, declaration_symbol_kind,
                         declaration_base_type ? declaration_base_type : type_error(),
                         STORAGE_NONE, yylineno, 1);
        free(name);
    }
}

static void discard_text(char *text)
{
    free(text);
}

static void enter_named_scope(const char *kind, const char *name)
{
    size_t size = strlen(kind) + strlen(name) + 3;
    char *label = malloc(size);

    if (label == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }

    snprintf(label, size, "%s %s", kind, name);
    symtab_enter_scope(label);
    free(label);
}

static void remember_function(char *name)
{
    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;

    if (name != NULL) {
        pending_function_name = strdup(name);
        if (pending_function_name == NULL) {
            fprintf(stderr, "Out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

static void enter_function_scope(void)
{
    enter_named_scope("function",
                      pending_function_name != NULL ? pending_function_name : "<anonymous>");
    semantic_begin_function(pending_function_return_type
                            ? pending_function_return_type : type_error());
}

static void finish_function_scope(void)
{
    semantic_end_function();
    symtab_leave_scope();
    free(pending_function_name);
    pending_function_name = NULL;
}

static void remember_parameter(char *name, Type *type, PassMode mode)
{
    PendingParameter *parameter;

    parameter = malloc(sizeof(*parameter));
    if (parameter == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(name);
        exit(EXIT_FAILURE);
    }

    parameter->name = name;
    parameter->type = type;
    parameter->mode = mode;
    parameter->next = NULL;

    if (pending_parameters_tail == NULL) {
        pending_parameters = parameter;
    } else {
        pending_parameters_tail->next = parameter;
    }
    pending_parameters_tail = parameter;
}

static void declare_pending_parameters(void)
{
    PendingParameter *parameter = pending_parameters;

    while (parameter != NULL) {
        if (parameter->name != NULL) {
            StorageClass storage = parameter->mode == PASS_REFERENCE
                ? STORAGE_PARAMETER_REFERENCE : STORAGE_PARAMETER_VALUE;
            semantic_declare(parameter->name, SYMBOL_PARAMETER, parameter->type,
                             storage, yylineno, 1);
        }
        parameter = parameter->next;
    }

    clear_pending_parameters();
}

static void clear_pending_parameters(void)
{
    PendingParameter *parameter = pending_parameters;

    while (parameter != NULL) {
        PendingParameter *next = parameter->next;
        free(parameter->name);
        free(parameter);
        parameter = next;
    }

    pending_parameters = NULL;
    pending_parameters_tail = NULL;
}

static void register_pending_function(int definition)
{
    PendingParameter *pending;
    Parameter *parameters = NULL;
    Type *function_type;

    for (pending = pending_parameters; pending; pending = pending->next) {
        parameters = parameter_append(parameters,
            parameter_new(pending->name, pending->type, pending->mode));
    }

    function_type = type_function(
        pending_function_return_type ? pending_function_return_type : type_error(),
        parameters);
    parameter_free(parameters);

    semantic_declare_function(
        pending_function_name ? pending_function_name : "<anonymous>",
        function_type, yylineno, definition);
}

static int implicit_reference(Type *type)
{
    return type && (type->kind == TYPE_STRING || type->kind == TYPE_ARRAY ||
                    type->kind == TYPE_LIST || type->kind == TYPE_CLASS ||
                    type->kind == TYPE_UNION);
}

static DimList *dim_new(long long length)
{
    DimList *dimension = calloc(1, sizeof(*dimension));
    if (!dimension) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    dimension->length = length;
    return dimension;
}

static DimList *dim_append(DimList *list, DimList *item)
{
    DimList *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

static Type *apply_dimensions_recursive(Type *base, DimList *dimensions)
{
    Type *inner;
    if (!dimensions) return base;
    inner = apply_dimensions_recursive(base, dimensions->next);
    return type_array(inner, dimensions->length);
}

static Type *apply_declarator(Type *base, int is_list,
                              DimList *dimensions, int line)
{
    Type *type;
    if (is_list && dimensions) {
        semantic_error(line,
            "a list marker and array dimensions cannot coexist in one declarator");
    }
    type = is_list ? type_list(base) : base;
    return apply_dimensions_recursive(type, dimensions);
}

static void dim_free(DimList *dimensions)
{
    while (dimensions) {
        DimList *next = dimensions->next;
        free(dimensions);
        dimensions = next;
    }
}

static DeclItem *decl_new(char *name, int is_list,
                          DimList *dimensions, int line)
{
    DeclItem *item = calloc(1, sizeof(*item));
    if (!item) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    item->name = name;
    item->is_list = is_list;
    item->dimensions = dimensions;
    item->line = line;
    return item;
}

static DeclItem *decl_append(DeclItem *list, DeclItem *item)
{
    DeclItem *tail;
    if (!list) return item;
    for (tail = list; tail->next; tail = tail->next) {}
    tail->next = item;
    return list;
}

static void process_declarations(Type *base, DeclItem *items,
                                 SymbolKind kind, StorageClass storage)
{
    while (items) {
        DeclItem *next = items->next;
        Type *type = apply_declarator(base, items->is_list,
                                      items->dimensions, items->line);

        if (items->initializer) {
            if (type->kind == TYPE_LIST) {
                semantic_error(items->line,
                    "list variable '%s' cannot be initialized", items->name);
            }
            if (!type_assignable(type, items->initializer->type)) {
                semantic_error(items->line,
                    "initializer of '%s' has incompatible type", items->name);
            }
        }

        semantic_declare(items->name, kind, type, storage, items->line, 1);
        ast_free(items->initializer);
        dim_free(items->dimensions);
        free(items->name);
        free(items);
        items = next;
    }
}

void yyerror(const char *message)
{
    if (!diagnostics_too_many_syntax_errors()) {
        diagnostics_syntax_error(yylineno, message);
    }
}

static void print_usage(const char *program_name)
{
    fprintf(stderr,
            "Usage: %s [-q|--quiet-tokens] <source.cpp>\n"
            "  -q, --quiet-tokens   do not print every token returned by the lexer\n",
            program_name);
}

int main(int argc, char **argv)
{
    const char *filename = NULL;
    int trace_tokens = 1;
    int index;
    int parse_result;
    int success;

    for (index = 1; index < argc; index++) {
        if (strcmp(argv[index], "-q") == 0 ||
            strcmp(argv[index], "--quiet-tokens") == 0) {
            trace_tokens = 0;
        } else if (filename == NULL) {
            filename = argv[index];
        } else {
            print_usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (filename == NULL) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (diagnostics_load_source(filename) != 0) {
        fprintf(stderr, "Cannot preload source file '%s' for diagnostics.\n", filename);
        return EXIT_FAILURE;
    }

    yyin = fopen(filename, "r");
    if (yyin == NULL) {
        perror(filename);
        diagnostics_free_source();
        return EXIT_FAILURE;
    }

    yylineno = 1;
    yyrestart(yyin);
    lexer_set_trace(trace_tokens);
    types_init();
    symtab_init();
    semantic_init();

    parse_result = yyparse();

    if (symtab_current_scope()) symtab_print_scope(symtab_current_scope());
    semantic_verify_all_prototypes();
    symtab_finish();

    success = parse_result == 0 &&
              diagnostics_lexical_count() == 0 &&
              diagnostics_syntax_count() == 0 &&
              semantic_error_count() == 0;

    printf("\n=== ANALYSIS SUMMARY ===\n");
    printf("Lexical errors : %d\n", diagnostics_lexical_count());
    printf("Syntax errors  : %d\n", diagnostics_syntax_count());
    printf("Semantic errors: %d\n", semantic_error_count());
    printf("Result         : %s\n", success ? "ACCEPTED" : "REJECTED");

    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;
    fclose(yyin);
    diagnostics_free_source();
    semantic_finish();
    types_finish();

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
