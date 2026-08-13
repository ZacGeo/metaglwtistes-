%{
#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "diagnostics.h"
#include "parser_types.h"
#include "parser.tab.h"

/* Some Flex skeletons include stdio before the POSIX feature macro. */
extern int fileno(FILE *stream);

static int trace_tokens = 1;
static char *string_buffer = NULL;
static size_t string_length = 0;
static size_t string_capacity = 0;
static int string_start_line = 1;
static int comment_start_line = 1;

static void *xrealloc(void *ptr, size_t size);
static char *xstrdup(const char *text);
static void string_reset(void);
static void string_append(const char *text, size_t length);
static int emit_token(int token, const char *name, const char *lexeme);
static int emit_text_token(int token, const char *name, const char *lexeme);
static double parse_based_float(const char *text, int base);

void lexer_set_trace(int enabled)
{
    trace_tokens = enabled;
}

#define STOP_AFTER_FIVE_ERRORS()                   \
    do {                                           \
        if (diagnostics_too_many_syntax_errors())  \
            return 0;                              \
    } while (0)
%}

%option noyywrap
%option yylineno
%option caseless
%option nodefault
%option nounput
%option noinput

%x COMMENT STRING_STATE

DIGIT           [0-9]
NZDIGIT         [1-9]
OCTDIGIT        [0-7]
BINDIGIT        [01]
HEXDIGIT        [0-9A-F]
LETTER          [A-Z]
ALNUM           [A-Z0-9]

DECINT          (0|{NZDIGIT}{DIGIT}*)
HEXINT          0X[1-9A-F]{HEXDIGIT}*
OCTINT          0O[1-7]{OCTDIGIT}*
BININT          0B1{BINDIGIT}*

EXPNUM          (0|{NZDIGIT}{DIGIT}*)
EXPONENT        E[+-]?{EXPNUM}
DECFRAC         (0|{DIGIT}*{NZDIGIT}{DIGIT}*)
HEXFRAC         (0|{HEXDIGIT}*[1-9A-F]{HEXDIGIT}*)
OCTFRAC         (0|{OCTDIGIT}*[1-7]{OCTDIGIT}*)
BINFRAC         (0|{BINDIGIT}*1{BINDIGIT}*)

DECFLOAT        (({DECINT})?\.{DECFRAC})({EXPONENT})?
DECEXPFLOAT     {DECINT}{EXPONENT}
HEXFLOAT        0X(0|[1-9A-F]{HEXDIGIT}*)?\.{HEXFRAC}
OCTFLOAT        0O(0|[1-7]{OCTDIGIT}*)?\.{OCTFRAC}
BINFLOAT        0B(0|1{BINDIGIT}*)?\.{BINFRAC}

/* Leading underscores are allowed. Elsewhere an underscore must be followed
   by an alphanumeric character. Thus trailing/consecutive inner '_' fail. */
VALID_ID        _*{LETTER}({ALNUM}|_{ALNUM})*

/* One A followed by zero or more D, OR no A and one or more D, between C/R. */
LIST_FUNCTION   C(AD*|D+)R

%%

<INITIAL>"/*"               {
                                STOP_AFTER_FIVE_ERRORS();
                                comment_start_line = yylineno;
                                BEGIN(COMMENT);
                              }
<COMMENT>\*+\/               { BEGIN(INITIAL); }
<COMMENT>\n                  { /* yylineno is updated automatically. */ }
<COMMENT>[^*\n]+             { /* Ignore block-comment text. */ }
<COMMENT>\*+                 { /* Ignore one or more '*'. */ }
<COMMENT><<EOF>>             {
                                diagnostics_lexical_error(comment_start_line,
                                    "unterminated block comment");
                                if (trace_tokens) printf("[EOF]\n");
                                BEGIN(INITIAL);
                                return 0;
                              }

<INITIAL>"//"[^\n]*          {
                                STOP_AFTER_FIVE_ERRORS();
                                /* Ignore line comment. */
                              }

 /* Keywords must precede ID because Flex resolves equal-length matches by the
   first rule. Matching is case-insensitive, but yytext preserves its spelling. */
<INITIAL>typedef             { STOP_AFTER_FIVE_ERRORS(); return emit_token(TYPEDEF, "TYPEDEF", yytext); }
<INITIAL>char                { STOP_AFTER_FIVE_ERRORS(); return emit_token(CHAR, "CHAR", yytext); }
<INITIAL>int                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(INT, "INT", yytext); }
<INITIAL>float               { STOP_AFTER_FIVE_ERRORS(); return emit_token(FLOAT, "FLOAT", yytext); }
<INITIAL>string              { STOP_AFTER_FIVE_ERRORS(); return emit_token(STRING, "STRING", yytext); }
<INITIAL>const               { STOP_AFTER_FIVE_ERRORS(); return emit_token(CONST, "CONST", yytext); }
<INITIAL>class               { STOP_AFTER_FIVE_ERRORS(); return emit_token(CLASS, "CLASS", yytext); }
<INITIAL>private             { STOP_AFTER_FIVE_ERRORS(); return emit_token(PRIVATE, "PRIVATE", yytext); }
<INITIAL>protected           { STOP_AFTER_FIVE_ERRORS(); return emit_token(PROTECTED, "PROTECTED", yytext); }
<INITIAL>public              { STOP_AFTER_FIVE_ERRORS(); return emit_token(PUBLIC, "PUBLIC", yytext); }
<INITIAL>void                { STOP_AFTER_FIVE_ERRORS(); return emit_token(VOID, "VOID", yytext); }
<INITIAL>static              { STOP_AFTER_FIVE_ERRORS(); return emit_token(STATIC, "STATIC", yytext); }
<INITIAL>union               { STOP_AFTER_FIVE_ERRORS(); return emit_token(UNION, "UNION", yytext); }
<INITIAL>enum                { STOP_AFTER_FIVE_ERRORS(); return emit_token(ENUM, "ENUM", yytext); }
<INITIAL>list                { STOP_AFTER_FIVE_ERRORS(); return emit_token(LIST, "LIST", yytext); }
<INITIAL>continue            { STOP_AFTER_FIVE_ERRORS(); return emit_token(CONTINUE, "CONTINUE", yytext); }
<INITIAL>break               { STOP_AFTER_FIVE_ERRORS(); return emit_token(BREAK, "BREAK", yytext); }
<INITIAL>if                  { STOP_AFTER_FIVE_ERRORS(); return emit_token(IF, "IF", yytext); }
<INITIAL>else                { STOP_AFTER_FIVE_ERRORS(); return emit_token(ELSE, "ELSE", yytext); }
<INITIAL>while               { STOP_AFTER_FIVE_ERRORS(); return emit_token(WHILE, "WHILE", yytext); }
<INITIAL>for                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(FOR, "FOR", yytext); }
<INITIAL>return              { STOP_AFTER_FIVE_ERRORS(); return emit_token(RETURN, "RETURN", yytext); }
<INITIAL>length              { STOP_AFTER_FIVE_ERRORS(); return emit_token(LENGTH, "LENGTH", yytext); }
<INITIAL>cin                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(CIN, "CIN", yytext); }
<INITIAL>cout                { STOP_AFTER_FIVE_ERRORS(); return emit_token(COUT, "COUT", yytext); }
<INITIAL>main                { STOP_AFTER_FIVE_ERRORS(); return emit_token(MAIN, "MAIN", yytext); }
<INITIAL>this                { STOP_AFTER_FIVE_ERRORS(); return emit_token(THIS, "THIS", yytext); }
<INITIAL>sizeof              { STOP_AFTER_FIVE_ERRORS(); return emit_token(SIZEOP, "SIZEOP", yytext); }

<INITIAL>{LIST_FUNCTION}     {
                                STOP_AFTER_FIVE_ERRORS();
                                yylval.sval = xstrdup(yytext);
                                return emit_token(LISTFUNC, "LISTFUNC", yytext);
                              }

 /* Floating rules must precede integer rules. */
<INITIAL>({DECFLOAT}|{DECEXPFLOAT}|{HEXFLOAT}|{OCTFLOAT}|{BINFLOAT}) {
                                STOP_AFTER_FIVE_ERRORS();
                                if (yytext[0] == '0' && (yytext[1] == 'x' || yytext[1] == 'X'))
                                    yylval.fval = parse_based_float(yytext + 2, 16);
                                else if (yytext[0] == '0' && (yytext[1] == 'o' || yytext[1] == 'O'))
                                    yylval.fval = parse_based_float(yytext + 2, 8);
                                else if (yytext[0] == '0' && (yytext[1] == 'b' || yytext[1] == 'B'))
                                    yylval.fval = parse_based_float(yytext + 2, 2);
                                else
                                    yylval.fval = strtod(yytext, NULL);

                                if (trace_tokens)
                                    printf("[FCONST] line=%d lexeme=%s value=%.17g\n",
                                           yylineno, yytext, yylval.fval);
                                return FCONST;
                              }

<INITIAL>({DECINT}|{HEXINT}|{OCTINT}|{BININT}) {
                                int base = 10;
                                const char *digits = yytext;

                                STOP_AFTER_FIVE_ERRORS();
                                if (yytext[0] == '0' && (yytext[1] == 'x' || yytext[1] == 'X')) {
                                    base = 16;
                                    digits = yytext + 2;
                                } else if (yytext[0] == '0' && (yytext[1] == 'o' || yytext[1] == 'O')) {
                                    base = 8;
                                    digits = yytext + 2;
                                } else if (yytext[0] == '0' && (yytext[1] == 'b' || yytext[1] == 'B')) {
                                    base = 2;
                                    digits = yytext + 2;
                                }

                                errno = 0;
                                yylval.ival = strtoll(digits, NULL, base);
                                if (errno == ERANGE) {
                                    diagnostics_lexical_error(yylineno,
                                        "integer constant is outside the supported range");
                                }

                                if (trace_tokens)
                                    printf("[ICONST] line=%d lexeme=%s value=%lld\n",
                                           yylineno, yytext, yylval.ival);
                                return ICONST;
                              }

 /* The language is case-insensitive except inside CCONST/SCONST. Because the
   scanner itself is caseless, validate special character escapes explicitly. */
<INITIAL>"'"\\."'"          {
                                STOP_AFTER_FIVE_ERRORS();
                                if (strchr("nftrbv", yytext[2]) != NULL) {
                                    yylval.sval = xstrdup(yytext);
                                    return emit_token(CCONST, "CCONST", yytext);
                                }
                                diagnostics_lexical_error(yylineno,
                                    "invalid escape in character constant; constant discarded");
                              }

<INITIAL>"'"[ -~]"'"        {
                                STOP_AFTER_FIVE_ERRORS();
                                yylval.sval = xstrdup(yytext);
                                return emit_token(CCONST, "CCONST", yytext);
                              }

<INITIAL>"'"[^'\n]*"'"      {
                                diagnostics_lexical_error(yylineno,
                                    "malformed character constant; constant discarded");
                              }

<INITIAL>"'"[^'\n]*\n       {
                                diagnostics_lexical_error(yylineno - 1,
                                    "unterminated character constant; line discarded");
                              }

<INITIAL>"'"[^'\n]*          {
                                diagnostics_lexical_error(yylineno,
                                    "unterminated character constant at end of file");
                              }

<INITIAL>\"                 {
                                STOP_AFTER_FIVE_ERRORS();
                                string_reset();
                                string_start_line = yylineno;
                                BEGIN(STRING_STATE);
                              }

<STRING_STATE>\"            {
                                yylval.sval = xstrdup(string_buffer != NULL ? string_buffer : "");
                                if (trace_tokens)
                                    printf("[SCONST] line=%d value=\"%s\"\n",
                                           string_start_line, yylval.sval);
                                BEGIN(INITIAL);
                                return SCONST;
                              }

<STRING_STATE>\\\n           {
                                /* Line continuation: neither character belongs
                                   to the returned string. */
                              }
<STRING_STATE>\\[^\n]       { string_append(yytext, (size_t)yyleng); }
<STRING_STATE>\\             {
                                diagnostics_lexical_error(string_start_line,
                                    "unterminated string constant after backslash");
                                BEGIN(INITIAL);
                                return 0;
                              }
<STRING_STATE>[^\\\"\n]+   { string_append(yytext, (size_t)yyleng); }
<STRING_STATE>\n             {
                                diagnostics_lexical_error(string_start_line,
                                    "unterminated string constant before end of line");
                                BEGIN(INITIAL);
                              }
<STRING_STATE><<EOF>>        {
                                diagnostics_lexical_error(string_start_line,
                                    "unterminated string constant at end of file");
                                if (trace_tokens) printf("[EOF]\n");
                                BEGIN(INITIAL);
                                return 0;
                              }

<INITIAL>{VALID_ID}          {
                                STOP_AFTER_FIVE_ERRORS();
                                yylval.sval = xstrdup(yytext);
                                return emit_token(ID, "ID", yytext);
                              }

 /* Long operators before their one-character prefixes. */
<INITIAL>"||"                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(OROP, "OROP", yytext); }
<INITIAL>"&&"                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(ANDOP, "ANDOP", yytext); }
<INITIAL>"=="                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(EQUOP, "EQUOP", yytext); }
<INITIAL>"!="                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(EQUOP, "EQUOP", yytext); }
<INITIAL>">="                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(RELOP, "RELOP", yytext); }
<INITIAL>"<="                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(RELOP, "RELOP", yytext); }
<INITIAL>"++"                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(INCDEC, "INCDEC", yytext); }
<INITIAL>"--"                { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(INCDEC, "INCDEC", yytext); }
<INITIAL>"::"                { STOP_AFTER_FIVE_ERRORS(); return emit_token(METH, "METH", yytext); }
<INITIAL>">>"                { STOP_AFTER_FIVE_ERRORS(); return emit_token(INP, "INP", yytext); }
<INITIAL>"<<"                { STOP_AFTER_FIVE_ERRORS(); return emit_token(OUT, "OUT", yytext); }
<INITIAL>">"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(RELOP, "RELOP", yytext); }
<INITIAL>"<"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(RELOP, "RELOP", yytext); }
<INITIAL>"+"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(ADDOP, "ADDOP", yytext); }
<INITIAL>"-"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(ADDOP, "ADDOP", yytext); }
<INITIAL>"*"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(MULOP, "MULOP", yytext); }
<INITIAL>"/"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(MULOP, "MULOP", yytext); }
<INITIAL>"%"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(MULOP, "MULOP", yytext); }
<INITIAL>"!"                 { STOP_AFTER_FIVE_ERRORS(); return emit_text_token(NOTOP, "NOTOP", yytext); }
<INITIAL>"("                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(LPAREN, "LPAREN", yytext); }
<INITIAL>")"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(RPAREN, "RPAREN", yytext); }
<INITIAL>";"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(SEMI, "SEMI", yytext); }
<INITIAL>"."                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(DOT, "DOT", yytext); }
<INITIAL>","                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(COMMA, "COMMA", yytext); }
<INITIAL>"="                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(ASSIGN, "ASSIGN", yytext); }
<INITIAL>":"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(COLON, "COLON", yytext); }
<INITIAL>"["                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(LBRACK, "LBRACK", yytext); }
<INITIAL>"]"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(RBRACK, "RBRACK", yytext); }
<INITIAL>"&"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(REFER, "REFER", yytext); }
<INITIAL>"{"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(LBRACE, "LBRACE", yytext); }
<INITIAL>"}"                 { STOP_AFTER_FIVE_ERRORS(); return emit_token(RBRACE, "RBRACE", yytext); }

<INITIAL>[ \t\r]+            { STOP_AFTER_FIVE_ERRORS(); }
<INITIAL>\n                  { STOP_AFTER_FIVE_ERRORS(); }

<INITIAL>.                   {
                                char message[128];
                                snprintf(message, sizeof(message),
                                         "unknown character '%s'; character discarded",
                                         yytext);
                                diagnostics_lexical_error(yylineno, message);
                              }

<INITIAL><<EOF>>             {
                                if (trace_tokens) printf("[EOF]\n");
                                return 0;
                              }

%%

static void *xrealloc(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

static char *xstrdup(const char *text)
{
    size_t length = strlen(text) + 1;
    char *copy = xrealloc(NULL, length);
    memcpy(copy, text, length);
    return copy;
}

static void string_reset(void)
{
    string_length = 0;
    if (string_buffer == NULL) {
        string_capacity = 64;
        string_buffer = xrealloc(NULL, string_capacity);
    }
    string_buffer[0] = '\0';
}

static void string_append(const char *text, size_t length)
{
    size_t required = string_length + length + 1;

    if (string_buffer == NULL) {
        string_reset();
    }

    if (required > string_capacity) {
        while (string_capacity < required) {
            string_capacity *= 2;
        }
        string_buffer = xrealloc(string_buffer, string_capacity);
    }

    memcpy(string_buffer + string_length, text, length);
    string_length += length;
    string_buffer[string_length] = '\0';
}

static int emit_token(int token, const char *name, const char *lexeme)
{
    if (trace_tokens) {
        printf("[%s] line=%d lexeme=%s\n", name, yylineno, lexeme);
    }
    return token;
}

static int emit_text_token(int token, const char *name, const char *lexeme)
{
    yylval.sval = xstrdup(lexeme);
    return emit_token(token, name, lexeme);
}

static int digit_value(int character)
{
    if (character >= '0' && character <= '9') {
        return character - '0';
    }
    if (character >= 'a' && character <= 'f') {
        return character - 'a' + 10;
    }
    return character - 'A' + 10;
}

static double parse_based_float(const char *text, int base)
{
    const char *cursor = text;
    double value = 0.0;
    double factor;

    while (*cursor != '\0' && *cursor != '.') {
        value = value * base + digit_value((unsigned char)*cursor);
        cursor++;
    }

    if (*cursor == '.') {
        cursor++;
    }

    factor = 1.0 / base;
    while (*cursor != '\0') {
        value += digit_value((unsigned char)*cursor) * factor;
        factor /= base;
        cursor++;
    }

    return value;
}
