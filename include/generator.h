#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash_table.h"
#include "token_array.h"
#include "symbol_array.h"

typedef struct _prog
{
  char *code;
  HashTable *symbol_table;
  HashTable *extern_table;
  HashTable *macro_name_table;
  char *macro_def_table;
} Program;

int two_pass(TokenArray *token_arr, InstructionNode *instruction_arr, DirectiveNode *directive_arr, SymbolArray *sybmol_arr, char* generated_code);
