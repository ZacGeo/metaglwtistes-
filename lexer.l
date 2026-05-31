%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h> 
    #include "parser.tab.h"  // Εισάγει τα tokens που ορίστηκαν στο parser.y
%}

%option noyywrap

DIGIT      [0-9]
LETTER     [a-zA-Z]

ID         {LETTER}({LETTER}|{DIGIT}|_)*
ICONST     {DIGIT}+
FCONST     {DIGIT}+"."{DIGIT}+

%%

"int"              { return INT; }
"float"            { return FLOAT; }

"+"                { return PLUS; }
"-"                { return MINUS; }

"="                { return ASSIGN; }
";"                { return SEMI; }

"("                { return LPAREN; }
")"                { return RPAREN; }

{FCONST}           { yylval.fval = atof(yytext); return FCONST; }
{ICONST}           { yylval.ival = atoi(yytext); return ICONST; }
{ID}               { yylval.sval = strdup(yytext); return ID; }

[ \t\n]            ; /* Αγνόησε τα κενά και τις αλλαγές γραμμής */

.                  { printf("Άγνωστος χαρακτήρας: %s\n", yytext); }

%%