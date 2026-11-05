#include "include/hash_tables.h"

typedef struct
{
  char *code;
  HashTable *symbol_table;  
  HashTable *use_table;
} Program;


// tab uso, tab simbolos (sem definições), 
char *two_pass(char *code, HashTable *symbol_table);

