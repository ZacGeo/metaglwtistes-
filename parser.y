%{
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diagnostics.h"
#include "symtab.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
extern void yyrestart(FILE *input_file);
void lexer_set_trace(int enabled);

void yyerror(const char *message);

static char *pending_function_name = NULL;

typedef struct PendingParameter {
    char *name;
    struct PendingParameter *next;
} PendingParameter;

static PendingParameter *pending_parameters = NULL;
static PendingParameter *pending_parameters_tail = NULL;

static void declare_identifier(char *name);
static void discard_text(char *text);
static void enter_named_scope(const char *kind, const char *name);
static void remember_function(char *name);
static void enter_function_scope(void);
static void finish_function_scope(void);
static void remember_parameter(char *name);
static void declare_pending_parameters(void);
static void clear_pending_parameters(void);
%}

%error-verbose
%start program

%union {
    long long ival;
    double fval;
    char *sval;
}

/* Keywords. */
%token TYPEDEF CHAR INT FLOAT STRING CONST CLASS
%token PRIVATE PROTECTED PUBLIC VOID STATIC UNION ENUM LIST
%token CONTINUE BREAK IF ELSE WHILE FOR RETURN
%token LENGTH CIN COUT MAIN THIS SIZEOP

/* Operators. */
%token OROP ANDOP EQUOP RELOP ADDOP MULOP NOTOP INCDEC
%token METH INP OUT REFER

/* Separators. */
%token LPAREN RPAREN SEMI DOT COMMA ASSIGN COLON
%token LBRACK RBRACK LBRACE RBRACE

/* Tokens carrying values through yylval. */
%token <ival> ICONST
%token <fval> FCONST
%token <sval> CCONST SCONST ID LISTFUNC

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
          declare_identifier($4);
      }
    ;

typename
    : standard_type
    | ID
      {
          discard_text($1);
      }
    ;

standard_type
    : CHAR
    | INT
    | FLOAT
    | STRING
    | VOID
    ;

listspec
    : LIST
    | /* empty */
    ;

dims
    : dims dim
    | /* empty */
    ;

dim
    : LBRACK ICONST RBRACK
    | LBRACK RBRACK
    ;

const_declaration
    : CONST typename constdefs SEMI
    ;

constdefs
    : constdefs COMMA constdef
    | constdef
    ;

constdef
    : ID dims ASSIGN init_value
      {
          declare_identifier($1);
      }
    ;

init_value
    : expression
    | LBRACE init_values RBRACE
    ;

init_values
    : init_values COMMA init_value
    | init_value
    ;

/* The ambiguous expression grammar from cpp.pdf is split into precedence
   levels. The accepted operators and operands remain those of CPP. */
expression
    : or_expression
    ;

or_expression
    : or_expression OROP and_expression
    | and_expression
    ;

and_expression
    : and_expression ANDOP equality_expression
    | equality_expression
    ;

equality_expression
    : equality_expression EQUOP relational_expression
    | relational_expression
    ;

relational_expression
    : relational_expression RELOP additive_expression
    | additive_expression
    ;

additive_expression
    : additive_expression ADDOP multiplicative_expression
    | multiplicative_expression
    ;

multiplicative_expression
    : multiplicative_expression MULOP unary_expression
    | unary_expression
    ;

unary_expression
    : NOTOP unary_expression
    | ADDOP unary_expression
    | SIZEOP unary_expression
    | INCDEC variable
    | postfix_expression
    ;

postfix_expression
    : variable INCDEC
    | variable
    | variable LPAREN expression_list RPAREN
    | LENGTH LPAREN general_expression RPAREN
    | constant
    | LPAREN general_expression RPAREN
    | LPAREN standard_type RPAREN
    | listexpression
    ;

variable
    : variable LBRACK general_expression RBRACK
    | variable DOT ID
      {
          discard_text($3);
      }
    | LISTFUNC LPAREN general_expression RPAREN
      {
          discard_text($1);
      }
    | decltype ID
      {
          discard_text($2);
      }
    | THIS
    ;

general_expression
    : general_expression COMMA assignment
    | assignment
    ;

assignment
    : variable ASSIGN assignment
    | expression
    ;

expression_list
    : general_expression
    | /* empty */
    ;

constant
    : CCONST
      {
          discard_text($1);
      }
    | ICONST
    | FCONST
    | SCONST
      {
          discard_text($1);
      }
    ;

listexpression
    : LBRACK expression_list RBRACK
    ;

enum_declaration
    : ENUM ID
      {
          symtab_declare($2);
          enter_named_scope("enum", $2);
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
      }
    | ID initializer
      {
          declare_identifier($1);
      }
    ;

initializer
    : ASSIGN init_value
    | /* empty */
    ;

class_declaration
    : CLASS ID
      {
          symtab_declare($2);
          enter_named_scope("class", $2);
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
    ;

variabledefs
    : variabledefs COMMA variabledef
    | variabledef
    ;

variabledef
    : listspec ID dims
      {
          declare_identifier($2);
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
          free(pending_function_name);
          pending_function_name = NULL;
      }
    | nopar_func_header SEMI
      {
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
          symtab_declare($3);
          remember_function($3);
          discard_text($3);
      }
    ;

parameter_types
    : parameter_types COMMA typename pass_list_dims
    | typename pass_list_dims
    ;

pass_list_dims
    : REFER
    | listspec dims
    ;

nopar_func_header
    : func_header_start LPAREN RPAREN
    ;

union_declaration
    : UNION ID
      {
          symtab_declare($2);
          enter_named_scope("union", $2);
          discard_text($2);
      }
      union_body SEMI
      {
          symtab_leave_scope();
      }
    ;

global_var_declaration
    : typename init_variabledefs SEMI
    ;

init_variabledefs
    : init_variabledefs COMMA init_variabledef
    | init_variabledef
    ;

init_variabledef
    : variabledef initializer
    ;

func_declaration
    : short_func_declaration
    | full_func_declaration
    ;

full_func_declaration
    : full_par_func_header function_body
      {
          finish_function_scope();
      }
    | nopar_class_func_header
      {
          enter_function_scope();
      }
      function_body
      {
          finish_function_scope();
      }
    | nopar_func_header
      {
          enter_function_scope();
      }
      function_body
      {
          finish_function_scope();
      }
    ;

full_par_func_header
    : class_func_header_start LPAREN parameter_list RPAREN
      {
          enter_function_scope();
          declare_pending_parameters();
      }
    | func_header_start LPAREN parameter_list RPAREN
      {
          enter_function_scope();
          declare_pending_parameters();
      }
    ;

class_func_header_start
    : typename listspec func_class ID
      {
          symtab_declare($4);
          remember_function($4);
          discard_text($4);
      }
    ;

func_class
    : ID METH
      {
          discard_text($1);
      }
    ;

parameter_list
    : parameter_list COMMA typename pass_variabledef
    | typename pass_variabledef
    ;

pass_variabledef
    : parameter_variabledef
    | REFER ID
      {
          remember_parameter($2);
      }
    ;

parameter_variabledef
    : listspec ID dims
      {
          remember_parameter($2);
      }
    ;

nopar_class_func_header
    : class_func_header_start LPAREN RPAREN
    ;

function_body
    : LBRACE decl_statements RBRACE
    | LBRACE error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

decl_statements
    : declarations statements
    | declarations
    | statements
    | /* empty */
    ;

declarations
    : declarations local_declaration
    | local_declaration
    ;

local_declaration
    : decltype typename variabledefs SEMI
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
    : STATIC
    | /* empty */
    ;

statements
    : statements statement
    | statement
    ;

statement
    : expression_statement
    | if_statement
    | while_statement
    | for_statement
    | return_statement
    | io_statement
    | comp_statement
    | CONTINUE SEMI
    | BREAK SEMI
    | SEMI
    | error SEMI
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
      }
    ;

expression_statement
    : general_expression SEMI
    ;

if_statement
    : IF LPAREN general_expression RPAREN statement %prec LOWER_THAN_ELSE
    | IF LPAREN general_expression RPAREN statement ELSE statement
    ;

while_statement
    : WHILE LPAREN general_expression RPAREN statement
    ;

for_statement
    : FOR LPAREN optexpr SEMI optexpr SEMI optexpr RPAREN statement
    ;

optexpr
    : general_expression
    | /* empty */
    ;

return_statement
    : RETURN optexpr SEMI
    ;

io_statement
    : CIN INP in_list SEMI
    | COUT OUT out_list SEMI
    ;

in_list
    : in_list INP in_item
    | in_item
    ;

in_item
    : variable
    ;

out_list
    : out_list OUT out_item
    | out_item
    ;

out_item
    : general_expression
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
      }
    | compound_open error RBRACE
      {
          yyerrok;
          if (diagnostics_too_many_syntax_errors()) YYABORT;
          symtab_leave_scope();
      }
    ;

main_function
    : main_header
      {
          symtab_enter_scope("main");
      }
      function_body
      {
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
        symtab_declare(name);
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
}

static void finish_function_scope(void)
{
    symtab_leave_scope();
    free(pending_function_name);
    pending_function_name = NULL;
}

static void remember_parameter(char *name)
{
    PendingParameter *parameter;

    if (name == NULL) {
        return;
    }

    parameter = malloc(sizeof(*parameter));
    if (parameter == NULL) {
        fprintf(stderr, "Out of memory\n");
        free(name);
        exit(EXIT_FAILURE);
    }

    parameter->name = name;
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
        symtab_declare(parameter->name);
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
    symtab_init();

    parse_result = yyparse();

    /* Also closes/prints any still-open scopes after an aborted parse. */
    symtab_finish();

    success = parse_result == 0 &&
              diagnostics_lexical_count() == 0 &&
              diagnostics_syntax_count() == 0;

    printf("\n=== ANALYSIS SUMMARY ===\n");
    printf("Lexical errors : %d\n", diagnostics_lexical_count());
    printf("Syntax errors  : %d\n", diagnostics_syntax_count());
    printf("Result         : %s\n", success ? "ACCEPTED" : "REJECTED");

    clear_pending_parameters();
    free(pending_function_name);
    pending_function_name = NULL;
    fclose(yyin);
    diagnostics_free_source();

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
