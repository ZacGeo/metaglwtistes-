%{
    #include <stdio.h> 
    #include <stlib.h>

    
    extern int yylex(); //The line `extern int yylex();` is declaring a function named `yylex` that returns an integer. The `extern` keyword indicates that the function is defined in another source file and will be linked during the compilation process. This declaration allows the current source file to use the `yylex` function without needing to know its implementation details
    extern int yylineno; 
    extern char* yytext;  

    void yyerror(const char *s);
%}

//_______.Λέξεις κλειδιά._______

%token 
%token
%token

