#include "../include/hash_table.h"
#include "../include/symbol_array.h"

enum ParserStringState
{
  PARSE_NUMBER,
  PARSE_SYMBOL,
  PARSE_NONE,
  PARSE_UNDEFINED
};

enum Line
{
  PARSE_EMPTY_LINE,
  PARSE_INSTRUCTION_LINE,
  PARSE_DIRECTIVE_LINE,
};

int get_token_type(char *token);
int token_is_reserved(InstructionNode *instruction_array, DirectiveNode *directive_array, char *token);
int token_is_end_of_line(char *token);

int parse_directive(InstructionNode *instruction_array, DirectiveNode *directive_array, char ***directive_ptr, DirectiveNode *directive, int debug_flag);

int parse_instruction(InstructionNode *instruction_array, DirectiveNode *directive_array, char **instruction_ptr, InstructionNode *instruction, int debug_flag);

int parse_line(char **line_start, int token_count, InstructionNode *instruction_array, DirectiveNode *directive_array, int debug_flag);
int parse_code(TokenArray *token_arr, SymbolArray *symbol_arr, InstructionNode *instr_arr, DirectiveNode *direc_arr, int debug_flag);
