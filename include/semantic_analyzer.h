#include "error_types.h"
#include "symbol_array.h"
#include "hash_table.h"

int define_synonym(SynonymArray *synonym_arr, char *token, int value);
int semantic_analysis(TokenArray *token_arr, SymbolArray *symbol_arr, SynonymArray *synonym_arr, int debug_flag);
