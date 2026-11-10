#include "../include/hash_table.h"

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

int parse_directive(char **directive_ptr, DirectiveNode *directive);
int parse_instruction(InstructionNode *instruction_array, DirectiveNode *directive_array, char **instruction_ptr, InstructionNode *instruction);

int parse_line(char **line_start, int token_count, InstructionNode *instruction_array, DirectiveNode *directive_array);
int parse_code(TokenArray *token_arr, SymbolArray *symbol_arr, InstructionNode *instr_arr, DirectiveNode *direc_arr);
