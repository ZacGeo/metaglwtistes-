%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    // Επικοινωνία με τον Lexer
    extern int yylex();
    extern int yylineno;
    extern char* yytext;

    // Δήλωση της συνάρτησης σφαλμάτων
    void yyerror(const char *s);
%}

/* Ορισμός των τύπων δεδομένων που μπορεί να κρατάει ο Parser */
%union {
    int ival;
    float fval;
    char* sval;
}

/* Ορισμός των απλών Tokens (Λέξεις κλειδιά και Σύμβολα) */
%token INT FLOAT
%token PLUS MINUS ASSIGN SEMI LPAREN RPAREN

/* Ορισμός των Tokens που κουβαλάνε τιμές (Αριθμοί και Μεταβλητές) */
%token <ival> ICONST
%token <fval> FCONST
%token <sval> ID

/* Προτεραιότητα πράξεων (τα αριστερά προς τα δεξιά) */
%left PLUS MINUS

%%
/* ====== ΚΑΝΟΝΕΣ ΤΗΣ ΓΡΑΜΜΑΤΙΚΗΣ ====== */

program:
    statements
    ;

statements:
    statements statement
    | /* Κενός κανόνας, επιτρέπει να διαβαστεί ένα άδειο αρχείο */
    ;

statement:
    declaration SEMI
    | assignment SEMI
    | expression SEMI
    ;

/* Δήλωση μεταβλητής (π.χ. int x;) */
declaration:
    type ID
    ;

type:
    INT
    | FLOAT
    ;

/* Ανάθεση τιμής (π.χ. x = 5;) */
assignment:
    ID ASSIGN expression
    ;

/* Αριθμητικές Πράξεις */
expression:
    expression PLUS expression
    | expression MINUS expression
    | LPAREN expression RPAREN
    | ID
    | ICONST
    | FCONST
    ;

%%
/* ====== ΚΩΔΙΚΑΣ C ====== */

/* Συνάρτηση που τυπώνει το μήνυμα όταν υπάρχει συντακτικό λάθος */
void yyerror(const char *s) {
    fprintf(stderr, "Συντακτικό σφάλμα στη γραμμή %d: %s (βρέθηκε: '%s')\n", yylineno, s, yytext);
}

/* Η κεντρική συνάρτηση που εκκινεί την ανάλυση */
int main(void) {
    if (yyparse() == 0) {
        printf("\nΗ συντακτική ανάλυση ολοκληρώθηκε με επιτυχία! Δεν βρέθηκαν σφάλματα.\n");
    }
    return 0;
}