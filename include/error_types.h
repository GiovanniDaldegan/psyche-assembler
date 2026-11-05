typedef struct _err
{
  int err_type;
  int err_code;
  char *err_msg;
} Error;

enum ErrorType
{
  LEXIC_ERROR,
  SYNTAX_ERROR,
  SEMANTIC_ERROR,
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

  LABEL_TOO_LONG, // rótulo tem mais de 30 caracteres

  UNKNOWN_ERROR, // erro desconhecido
};

enum SemanticError
{
  LABEL_UNDEFINED, // rótulo não definido
  LABEL_REDEFINED, // definição repetida de rótulo
  MACRO_UNDEFINED, // macro não definida
  MACRO_REDEFINED, // definição repetida de macro
};
