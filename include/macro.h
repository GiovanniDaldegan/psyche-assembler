#include "../include/hash_table.h"

// macro não pode ser recursiva; definição de macro deve preceder uso
MacroNode *define_macro(char *macro);
int matching_args(MacroNode *macro, char **args);
char *expand_macro(MacroNode *macro, char **args);
