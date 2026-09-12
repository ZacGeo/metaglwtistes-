/* Διαχείριση λεκτικών και συντακτικών σφαλμάτων. */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diagnostics.h"

#define MAX_SYNTAX_ERRORS 5 /* Όριο πριν από τη διακοπή. */

/* Μετρητές σφαλμάτων. */
static int  lexical_errors = 0;
static int  syntax_errors  = 0;

/* Αποθηκευμένες γραμμές του πηγαίου αρχείου. */
static char **source_lines   = NULL;
static int    source_count   = 0;

/* Φορτώνει το πηγαίο αρχείο ανά γραμμή. */
int diagnostics_load_source(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    /* Μετρά τις γραμμές. */
    int   lines = 0;
    int   c;
    while ((c = fgetc(f)) != EOF)
        if (c == '\n') lines++;
    lines++;   /* Τελευταία γραμμή χωρίς \n. */
    rewind(f);

    source_lines = calloc((size_t)lines, sizeof(char *));
    if (!source_lines) { fclose(f); return -1; }

    char  buf[4096];
    int   idx = 0;
    while (fgets(buf, sizeof(buf), f) && idx < lines) {
        /* Αφαιρεί τους χαρακτήρες αλλαγής γραμμής. */
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';
        if (len > 0 && buf[len-1] == '\r') buf[--len] = '\0';
        source_lines[idx++] = strdup(buf);
    }
    source_count = idx;
    fclose(f);
    return 0;
}

/* Αποδεσμεύει τις αποθηκευμένες γραμμές. */
void diagnostics_free_source(void)
{
    for (int i = 0; i < source_count; i++)
        free(source_lines[i]);
    free(source_lines);
    source_lines = NULL;
    source_count = 0;
}

/* Εκτυπώνει τη γραμμή όπου εμφανίστηκε το σφάλμα. */
static void print_source_line(int line)
{
    int idx = line - 1;
    if (idx >= 0 && idx < source_count && source_lines[idx])
        fprintf(stderr, "    %s\n", source_lines[idx]);
}

/* Καταγράφει λεκτικό σφάλμα. */
void diagnostics_lexical_error(int line, const char *message)
{
    lexical_errors++;
    fprintf(stderr, "Lexical error (line %d): %s\n", line, message);
    print_source_line(line);
}

/* Καταγράφει συντακτικό σφάλμα. */
void diagnostics_syntax_error(int line, const char *message)
{
    syntax_errors++;
    fprintf(stderr, "Syntax error (line %d): %s\n", line, message);
    print_source_line(line);
    if (syntax_errors >= MAX_SYNTAX_ERRORS)
        fprintf(stderr, "Too many errors — stopping analysis.\n");
}

/* Επιστρέφουν τα πλήθη σφαλμάτων. */
int diagnostics_lexical_count(void) { return lexical_errors; }
int diagnostics_syntax_count(void)  { return syntax_errors;  }

/* Ελέγχει αν συμπληρώθηκε το όριο σφαλμάτων. */
int diagnostics_too_many_syntax_errors(void)
{
    return syntax_errors >= MAX_SYNTAX_ERRORS;
}
