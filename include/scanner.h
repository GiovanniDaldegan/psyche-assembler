#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/error_types.h"
#include "../include/token_array.h"
#include "../include/symbol_array.h"
#include "../include/hash_table.h"

enum ScannerStringState
{
  SCAN_NUMBER,
  SCAN_LABEL,
  SCAN_IGNORE,
  SCAN_NONE,
  SCAN_UNDEFINED
};

int token_is_valid(char *token);

int scan_line(char *line, TokenArray *token_arr, SymbolArray *symbol_table, InstructionNode *instr_array, DirectiveNode * directive_array);
int scan_file(char *i_filename, TokenArray *token_arr, SymbolArray *symbol_table, InstructionNode *instr_array, DirectiveNode * directive_array);
