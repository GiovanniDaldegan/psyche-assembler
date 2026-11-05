/* Element types */

enum CharType
{
  DIGIT,
  LETTER,
  UNDESCORE,
  SPACE,
  LINEBREAK
};

#include "include/hash_tables.h"

/**
 * Component states
 */

enum ScannerState
{
  SCAN_NUMBER,
  SCAN_SYMBOL,
  SCAN_IGNORE,
  SCAN_NONE,
  SCAN_UNDEFINED
};

enum ParserState
{
  PARSE_NUMBER,
  PARSE_LABEL,
  PARSE_IGNORE,
  PARSE_NONE,
  PARSE_UNDEFINED
};

/**
 * Errors
 */

typedef struct
{
  int err_code;
  char *err_type;
  char *err_msg;
} Error;

enum ErrorType
{
  LexicErr,
  SyntaxErr,
  SemanticErr
};

enum LexicError
{
  INVALID_CHAR,  // caractere inválido
  INVALID_LABEL, // nome inválido de rótulo
};

enum SyntaxError
{
  DIRECTIVE_UNDEFINED, // diretiva inexistente

  OPCODE_INVALID,              // opcode inválido
  INSTRUCTION_WRONG_ARG_COUNT, // número inválido de argumentos para instrução
  DIRECTIVE_INVALID,           // diretiva inválida
  DIRECTIVE_WRONG_ARG_COUNT,   // número inválido de argumentos para diretiva
  MACRO_WRONG_ARG_COUNT,       // número inválido de argumentos para a diretiva

  UNKNOWN_ERROR, // erro desconhecido
};

enum SemanticError
{
  LABEL_UNDEFINED, // rótulo não definido
  LABEL_REDEFINED, // definição repetida de rótulo
  MACRO_UNDEFINED, // macro não definida
  MACRO_REDEFINED, // definição repetida de macro
};
