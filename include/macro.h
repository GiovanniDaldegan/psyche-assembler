#include "../include/hash_tables.h"

MacroNode *search_macro(HashTable *macro_table, char *macro_name);
int matching_args(MacroNode *macro, char **args);
char *expand_macro(MacroNode *macro, char **args);
