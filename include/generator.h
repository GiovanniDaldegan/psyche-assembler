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

int inital_pass(TokenArray *token_array, SynonymArray *synonym_array, SymbolArray *def_table, SymbolArray *use_table, TokenArray *program_array, int debug_flag);

int two_pass(TokenArray *token_array, InstructionNode *instruction_array, DirectiveNode *directive_array, SymbolArray *sybmol_array, char *generated_code, int debug_flag);
