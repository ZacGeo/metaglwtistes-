#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

/* ============================================================
   diagnostics.h  —  Διαχείριση σφαλμάτων μεταγλωττιστή CPP
   ============================================================ */

/* Φόρτωσε το source file για εκτύπωση γραμμών στα μηνύματα */
int  diagnostics_load_source(const char *filename);
void diagnostics_free_source(void);

/* Αναφορά σφαλμάτων */
void diagnostics_lexical_error(int line, const char *message);
void diagnostics_syntax_error(int line, const char *message);

/* Μετρητές */
int  diagnostics_lexical_count(void);
int  diagnostics_syntax_count(void);

/* Επιστρέφει 1 αν έχουμε ήδη >= 5 συντακτικά σφάλματα */
int  diagnostics_too_many_syntax_errors(void);

#endif /* DIAGNOSTICS_H */
