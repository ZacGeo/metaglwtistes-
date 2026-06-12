%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "parser.tab.h"

    /* Τρέχων αριθμός γραμμής και περιεχόμενο γραμμής για μηνύματα σφάλματος */
    int   yyline = 1;
    char  yylinebuf[4096] = {0};
    int   yylinebuf_pos   = 0;

    /* Βοηθητική: καταγράφει κάθε χαρακτήρα στο buffer τρέχουσας γραμμής */
    static void track_char(char c) {
        if (c == '\n') {
            yylinebuf[yylinebuf_pos] = '\0';
            yylinebuf_pos = 0;
            yyline++;
        } else if (yylinebuf_pos < (int)sizeof(yylinebuf) - 1) {
            yylinebuf[yylinebuf_pos++] = c;
            yylinebuf[yylinebuf_pos]   = '\0';
        }
    }

    /* Καλείται για κάθε token — ενημερώνει το line buffer */
    static void track_token(void) {
        for (int i = 0; yytext[i]; i++) track_char(yytext[i]);
    }

    static void lex_error(const char *msg) {
        fprintf(stderr, "Λεκτικό σφάλμα (γραμμή %d): %s\n  >> %s\n",
                yyline, msg, yylinebuf);
    }
%}

%option noyywrap
%option yylineno

/* ── Βασικοί ορισμοί ── */
DIGIT       [0-9]
NZDIGIT     [1-9]
OCTDIGIT    [0-7]
BINDIGIT    [01]
HEXDIGIT    [0-9A-Fa-f]
LETTER      [a-zA-Z]
ALNUM       [a-zA-Z0-9]

/* ── Αναγνωριστικό (ID) ──
   Κανόνες CPP:
   - προαιρετικό '_', ακολουθούμενο από αλφαβητικό
   - μετά: αλφαριθμητικοί ή '_'
   - δεν τελειώνει με '_'
   - δεν έχει διαδοχικά '__' εκτός αρχής
   Απλοποιημένο regex που καλύπτει τη συντριπτική πλειοψηφία:
*/
ID          (_*{LETTER})({ALNUM}|(_(?!_)))*

/* ── Ακέραιες σταθερές ── */
ICONST_DEC  (0|{NZDIGIT}{DIGIT}*)
ICONST_HEX  0[Xx]({NZDIGIT}{HEXDIGIT}*|[A-Fa-f]{HEXDIGIT}*)
ICONST_OCT  0[Oo][1-7]{OCTDIGIT}*
ICONST_BIN  0[Bb]1{BINDIGIT}*
ICONST      {ICONST_DEC}|{ICONST_HEX}|{ICONST_OCT}|{ICONST_BIN}

/* ── Εκθέτης ── */
EXP         [Ee][+-]?{DIGIT}+

/* ── Πραγματικές σταθερές ──
   Μορφές: digits.digits[E...] | .digits[E...] | ICONST E...
   Επίσης hex/oct/bin χωρίς εκθέτη
*/
FCONST_DE   ({DIGIT}*\.{DIGIT}+({EXP})?)
FCONST_EO   (({ICONST_DEC}){EXP})
FCONST_HF   (0[Xx]{HEXDIGIT}*\.{HEXDIGIT}+)
FCONST_OF   (0[Oo]{OCTDIGIT}*\.{OCTDIGIT}+)
FCONST_BF   (0[Bb]{BINDIGIT}*\.{BINDIGIT}+)
FCONST      {FCONST_DE}|{FCONST_EO}|{FCONST_HF}|{FCONST_OF}|{FCONST_BF}

%%

 /* ════════════════════════════════════════════
    ΣΧΟΛΙΑ
    ════════════════════════════════════════════ */
"/*"        {
                int c;
                while (1) {
                    c = input();
                    if (c == EOF) { lex_error("Μη κλειστό σχόλιο /*"); break; }
                    if (c == '\n') yyline++;
                    if (c == '*') {
                        c = input();
                        if (c == '/') break;
                        if (c == '\n') yyline++;
                    }
                }
            }

"//"[^\n]*  { /* Σχόλιο μέχρι τέλος γραμμής — αγνόησε */ }

 /* ════════════════════════════════════════════
    ΛΕΞΕΙΣ-ΚΛΕΙΔΙΑ
    ════════════════════════════════════════════ */
[Tt][Yy][Pp][Ee][Dd][Ee][Ff]           { track_token(); return TYPEDEF; }
[Cc][Hh][Aa][Rr]                        { track_token(); return CHAR; }
[Ii][Nn][Tt]                            { track_token(); return INT; }
[Ff][Ll][Oo][Aa][Tt]                    { track_token(); return FLOAT; }
[Ss][Tt][Rr][Ii][Nn][Gg]               { track_token(); return STRING; }
[Cc][Oo][Nn][Ss][Tt]                    { track_token(); return CONST; }
[Cc][Ll][Aa][Ss][Ss]                    { track_token(); return CLASS; }
[Pp][Rr][Ii][Vv][Aa][Tt][Ee]           { track_token(); return PRIVATE; }
[Pp][Rr][Oo][Tt][Ee][Cc][Tt][Ee][Dd]   { track_token(); return PROTECTED; }
[Pp][Uu][Bb][Ll][Ii][Cc]               { track_token(); return PUBLIC; }
[Vv][Oo][Ii][Dd]                        { track_token(); return VOID; }
[Ss][Tt][Aa][Tt][Ii][Cc]               { track_token(); return STATIC; }
[Uu][Nn][Ii][Oo][Nn]                    { track_token(); return UNION; }
[Ee][Nn][Uu][Mm]                        { track_token(); return ENUM; }
[Ll][Ii][Ss][Tt]                        { track_token(); return LIST; }
[Cc][Oo][Nn][Tt][Ii][Nn][Uu][Ee]       { track_token(); return CONTINUE; }
[Bb][Rr][Ee][Aa][Kk]                    { track_token(); return BREAK; }
[Ii][Ff]                                { track_token(); return IF; }
[Ee][Ll][Ss][Ee]                        { track_token(); return ELSE; }
[Ww][Hh][Ii][Ll][Ee]                    { track_token(); return WHILE; }
[Ff][Oo][Rr]                            { track_token(); return FOR; }
[Rr][Ee][Tt][Uu][Rr][Nn]               { track_token(); return RETURN; }
[Ll][Ee][Nn][Gg][Tt][Hh]               { track_token(); return LENGTH; }
[Cc][Ii][Nn]                            { track_token(); return CIN; }
[Cc][Oo][Uu][Tt]                        { track_token(); return COUT; }
[Mm][Aa][Ii][Nn]                        { track_token(); return MAIN; }
[Tt][Hh][Ii][Ss]                        { track_token(); return THIS; }
[Ss][Ii][Zz][Ee][Oo][Ff]               { track_token(); return SIZEOP; }

 /* ════════════════════════════════════════════
    LISTFUNC: C(A|D+)R  π.χ. CAR CDR CADR CADDR
    ════════════════════════════════════════════ */
[Cc]([Aa][Dd]*|[Dd]+)[Rr]  {
    track_token();
    yylval.sval = strdup(yytext);
    return LISTFUNC;
}

 /* ════════════════════════════════════════════
    ΠΡΑΓΜΑΤΙΚΕΣ ΣΤΑΘΕΡΕΣ (πριν τις ακέραιες!)
    ════════════════════════════════════════════ */
{FCONST}    {
    track_token();
    yylval.fval = atof(yytext);
    printf("[FCONST: %s -> %f]\n", yytext, yylval.fval);
    return FCONST;
}

 /* ════════════════════════════════════════════
    ΑΚΕΡΑΙΕΣ ΣΤΑΘΕΡΕΣ
    ════════════════════════════════════════════ */
{ICONST}    {
    track_token();
    /* Μετατροπή ανάλογα με βάση */
    if (yytext[0]=='0' && (yytext[1]=='X'||yytext[1]=='x'))
        yylval.ival = (int)strtol(yytext+2, NULL, 16);
    else if (yytext[0]=='0' && (yytext[1]=='O'||yytext[1]=='o'))
        yylval.ival = (int)strtol(yytext+2, NULL, 8);
    else if (yytext[0]=='0' && (yytext[1]=='B'||yytext[1]=='b'))
        yylval.ival = (int)strtol(yytext+2, NULL, 2);
    else
        yylval.ival = atoi(yytext);
    printf("[ICONST: %s -> %d]\n", yytext, yylval.ival);
    return ICONST;
}

 /* ════════════════════════════════════════════
    ΧΑΡΑΚΤΗΡΕΣ (CCONST)
    Αποδεκτά: 'a'  '$'  ' '  '''  '\n' '\t' κλπ.
    ════════════════════════════════════════════ */
\'([^\\\']|\\[nftrbv\\\'\"]) {
    track_token();
    /* Ανάγνωση κλεισίματος ' */
    int c = input();
    if (c != '\'') {
        lex_error("Μη έγκυρη σταθερά χαρακτήρα (λείπει το ')");
        unput(c);
    }
    /* Αποθήκευση string */
    yylval.sval = strdup(yytext);
    printf("[CCONST: %s]\n", yytext);
    return CCONST;
}

 /* ════════════════════════════════════════════
    ΟΡΜΑΘΟΙ ΧΑΡΑΚΤΗΡΩΝ (SCONST)
    Υποστηρίζει escape sequences και συνέχεια
    γραμμής με '\' στο τέλος
    ════════════════════════════════════════════ */
\"  {
    track_token();
    char buf[4096];
    int  pos = 0;
    int  c;
    while (1) {
        c = input();
        if (c == EOF) { lex_error("Μη κλειστός ορμαθός χαρακτήρων"); break; }
        if (c == '"')  break;
        if (c == '\\') {
            int next = input();
            if (next == '\n') { yyline++; continue; } /* συνέχεια γραμμής */
            if (pos < (int)sizeof(buf)-2) { buf[pos++]='\\'; buf[pos++]=next; }
        } else {
            if (c == '\n') yyline++;
            if (pos < (int)sizeof(buf)-1) buf[pos++] = c;
        }
    }
    buf[pos] = '\0';
    yylval.sval = strdup(buf);
    printf("[SCONST: \"%s\"]\n", buf);
    return SCONST;
}

 /* ════════════════════════════════════════════
    ΑΝΑΓΝΩΡΙΣΤΙΚΑ
    ════════════════════════════════════════════ */
{ID}        {
    track_token();
    yylval.sval = strdup(yytext);
    printf("[ID: %s]\n", yytext);
    return ID;
}

 /* ════════════════════════════════════════════
    ΤΕΛΕΣΤΕΣ ΠΟΛΛΑΠΛΩΝ ΧΑΡΑΚΤΗΡΩΝ (πρώτα!)
    ════════════════════════════════════════════ */
"||"    { track_token(); return OROP;   }
"&&"    { track_token(); return ANDOP;  }
"=="    { track_token(); return EQUOP;  }
"!="    { track_token(); return EQUOP;  }
">="    { track_token(); return RELOP;  }
"<="    { track_token(); return RELOP;  }
"++"    { track_token(); return INCDEC; }
"--"    { track_token(); return INCDEC; }
"::"    { track_token(); return METH;   }
">>"    { track_token(); return INP;    }
"<<"    { track_token(); return OUT;    }

 /* ════════════════════════════════════════════
    ΤΕΛΕΣΤΕΣ ΜΟΝΟΥ ΧΑΡΑΚΤΗΡΑ
    ════════════════════════════════════════════ */
">"     { track_token(); return RELOP;  }
"<"     { track_token(); return RELOP;  }
"+"     { track_token(); return ADDOP;  }
"-"     { track_token(); return ADDOP;  }
"*"     { track_token(); return MULOP;  }
"/"     { track_token(); return MULOP;  }
"%"     { track_token(); return MULOP;  }
"!"     { track_token(); return NOTOP;  }

 /* ════════════════════════════════════════════
    ΔΙΑΧΩΡΙΣΤΙΚΑ
    ════════════════════════════════════════════ */
"("     { track_token(); return LPAREN; }
")"     { track_token(); return RPAREN; }
";"     { track_token(); return SEMI;   }
"."     { track_token(); return DOT;    }
","     { track_token(); return COMMA;  }
"="     { track_token(); return ASSIGN; }
":"     { track_token(); return COLON;  }
"["     { track_token(); return LBRACK; }
"]"     { track_token(); return RBRACK; }
"&"     { track_token(); return REFER;  }
"{"     { track_token(); return LBRACE; }
"}"     { track_token(); return RBRACE; }

 /* ════════════════════════════════════════════
    ΚΕΝΑ / ΑΛΛΑΓΕΣ ΓΡΑΜΜΗΣ
    ════════════════════════════════════════════ */
[ \t\r]+    { track_token(); /* αγνόησε */ }
\n          { yyline++; yylinebuf_pos = 0; yylinebuf[0] = '\0'; }

 /* ════════════════════════════════════════════
    ΣΦΑΛΜΑΤΑ
    ════════════════════════════════════════════ */
.   {
    track_token();
    char msg[64];
    snprintf(msg, sizeof(msg), "Άγνωστος χαρακτήρας '%s'", yytext);
    lex_error(msg);
    /* Ανάνηψη: συνεχίζουμε σβήνοντας τον χαρακτήρα */
}

%%
