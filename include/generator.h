#include "../include/hash_tables.h"

typedef struct _prog
{
  char *code;
  HashTable *symbol_table;
  HashTable *extern_table;
  HashTable *macro_name_table;
  char *macro_def_table;
} Program;


// tab uso, tab simbolos (sem definições), 
char *two_pass(char *code, HashTable *symbol_table);

