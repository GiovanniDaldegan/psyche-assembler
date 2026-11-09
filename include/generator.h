#include "../include/hash_table.h"

typedef struct _prog
{
  char *code;
  HashTable *symbol_table;
  HashTable *extern_table;
  HashTable *macro_name_table;
  char *macro_def_table;
} Program;

// MacroNode *define_macros(char *code, HashTable macro_name_table, char *macro_def_table);
// char *expand_macros(char *code, HashTable *macro_table);

// tab uso, tab simbolos (sem definições),
char *two_pass(char *code, HashTable *symbol_table, HashTable *extern_table);
