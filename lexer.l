%option noyywrap
%option caseless

%{
#include <stdio.h>
%}

DIGIT      [0-9]
LETTER     [a-zA-Z]

ID         {LETTER}({LETTER}|{DIGIT}|_)*

ICONST     {DIGIT}+

FCONST     {DIGIT}+"."{DIGIT}+

%%

"int"              { printf("INT\n"); }
"float"            { printf("FLOAT\n"); }

"+"                { printf("PLUS\n"); }
"-"                { printf("MINUS\n"); }

"="                { printf("ASSIGN\n"); }

";"                { printf("SEMI\n"); }

"("                { printf("LPAREN\n"); }
")"                { printf("RPAREN\n"); }

{FCONST}           { printf("FLOAT CONST: %s\n", yytext); }

{ICONST}           { printf("INT CONST: %s\n", yytext); }

{ID}               { printf("ID: %s\n", yytext); }

[ \t\n]            ;

.                  { printf("UNKNOWN: %s\n", yytext); }

%%

int main()
{
    yylex();
    return 0;
}