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

int get_numeric_value(SynonymArray *synonym_arr, char *token);
int insert_in_section(TokenArray **section_ptr, char *token);
char *concat_sections(char *sec_text, char *sec_data);

int inital_pass(TokenArray *token_arr, SymbolArray *symbol_array, SynonymArray *synonym_arr, SymbolArray *def_table, SymbolArray *use_table, TokenArray *text_arr, TokenArray *data_arr);

int two_pass(TokenArray *token_arr, InstructionNode *instruction_arr, DirectiveNode *directive_arr, SymbolArray *sybmol_arr, char *generated_code);
