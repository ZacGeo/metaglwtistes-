# Πίνακας δοκιμών Σταδίου 2

## Έγκυρα προγράμματα — αναμενόμενο `ACCEPTED`

| Αρχείο | Τι ελέγχει |
|---|---|
| `valid_scalar.cpp` | σταθερές, συναρτήσεις, κλήσεις, βρόχος, AST και offsets |
| `valid_constants_enum_arrays.cpp` | constant folding, enum, πίνακες, παραλειπόμενη διάσταση, string |
| `valid_lists.cpp` | list literals, `CAR`, `CDR`, `CADR`, `CDDR`, `length` |
| `valid_classes_methods_union.cpp` | κληρονομικότητα, access, `this`, μέθοδοι, named/anonymous union |
| `valid_flat_and_aggregate_initializers.cpp` | flat πολυδιάστατα arrays και aggregate initializer lists |
| `valid_reference_and_control_flow.cpp` | references, prototypes, `if/else`, `while`, `continue`, returns |

## Άκυρα προγράμματα — αναμενόμενο σημασιολογικό σφάλμα

| Αρχείο | Αναμενόμενος έλεγχος |
|---|---|
| `error_undeclared.cpp` | χρήση αδήλωτου αναγνωριστικού |
| `error_duplicate_declaration.cpp` | διπλή δήλωση στην ίδια εμβέλεια |
| `error_type.cpp` | ασύμβατη ανάθεση τύπων |
| `error_wrong_argument_count.cpp` | λάθος πλήθος ορισμάτων |
| `error_reference_argument.cpp` | μη-lvalue όρισμα σε παράμετρο αναφοράς |
| `error_break.cpp` | `break` έξω από βρόχο |
| `error_void_return_value.cpp` | επιστροφή τιμής από `void` |
| `error_nonvoid_missing_return.cpp` | non-void συνάρτηση χωρίς εγγυημένο `return` |
| `error_constant_nonconstant.cpp` | μη σταθερός initializer σταθεράς |
| `error_enum_nonconstant.cpp` | μη ακέραια constant expression σε enum |
| `error_array_index_type.cpp` | μη ακέραιος δείκτης πίνακα |
| `error_array_omitted_without_initializer.cpp` | άγνωστη διάσταση χωρίς initializer |
| `error_array_too_many_initializers.cpp` | περισσότεροι initializers από το μέγεθος |
| `error_string_too_long.cpp` | string μεγαλύτερο από 255 χαρακτήρες |
| `error_heterogeneous_list.cpp` | ετερογενής λίστα |
| `error_car_nonlist.cpp` | `CAR` σε μη-list τύπο |
| `error_cadr_nonlist.cpp` | σύνθετη list function σε μη-list τύπο |
| `error_length_scalar.cpp` | `length` σε scalar |
| `error_private_access.cpp` | εξωτερική πρόσβαση σε private member |
| `error_protected_access.cpp` | εξωτερική πρόσβαση σε protected member |
| `error_inherited_private_access.cpp` | private base member από derived method |
| `error_this_outside_method.cpp` | `this` έξω από μέθοδο |
| `error_method_signature.cpp` | ορισμός μεθόδου με λάθος υπογραφή |
| `error_method_without_prototype.cpp` | ορισμός μεθόδου χωρίς class prototype |
| `error_missing_prototype_definition.cpp` | prototype χωρίς τελικό definition |

## Έλεγχος παραγόμενων αναφορών

Η εντολή Bison δημιουργεί το `parser.output`. Αναζήτησε conflicts σε PowerShell:

```powershell
Select-String -Path .\parser.output -Pattern 'conflict|useless'
```

Η σωστή έξοδος είναι κενή. Η κενή έξοδος εδώ σημαίνει ότι δεν βρέθηκε κανένα
conflict ή useless rule — δεν είναι σφάλμα της εντολής.
