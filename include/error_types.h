typedef struct _err
{
  int err_type;
  int err_code;
  char *err_msg;
} Error;

/*
enum ErrorType
{
  OK,
  LEXIC_ERROR,
  SYNTAX_ERROR,
  SEMANTIC_ERROR,
};
*/

enum LexicError
{
  LEXIC_OK,
  INVALID_CHAR,   // caractere inválido
  INVALID_SYMBOL, // nome inválido de rótulo

  INVALID_DECLARATION, // declaração inválida de rótulo

  FILE_ERROR,
};

enum SyntaxError
{
  SYNTAX_OK,
  INVALID_LINE, // linha invalidamente iniciada

  EXPRESSION_INVALID, // espressão inválida

  OPCODE_UNDEFINED,        // opcode inválido
  INSTRUCTION_FAIL,        // falha de parsing da instrução
  INSTRUCTION_FEW_ARGS,    // número inferior ao esperado para instrução
  INSTRUCTION_MANY_ARGS,   // número superior ao esperado para instrução
  INSTRUCTION_INVALID_ARG, // número superior ao esperado para instrução

  DIRECTIVE_UNDEFINED,   // diretiva não encontrada
  DIRECTIVE_FEW_ARGS,    // número inferior de argumentos para diretiva
  DIRECTIVE_MANY_ARGS,   // número superior de argumentos para diretiva
  DIRECTIVE_INVALID_ARG, // argumento inválido para diretiva

  MACRO_FEW_ARGS,    // número inferior de argumentos para a diretiva
  MACRO_MANY_ARGS,   // número superior de argumentos para a diretiva
  MACRO_INVALID_ARG, // número superior de argumentos para a diretiva

  LABEL_TOO_LONG, // rótulo tem mais de 30 caracteres

  SYNTAX_UNDEFINED_ERROR, // erro indefinido
};

enum SemanticError
{
  SEMANTIC_OK,

  LABEL_UNDEFINED,   // rótulo não definido
  LABEL_REDEFINED,   // definição repetida de rótulo
  LABEL_OUT_SECTION, // definição de rótulo fora de seção
  LABEL_IS_SYNONYM,  // rótulo também é definido como sinônimo numérico com EQU

  MACRO_UNDEFINED, // macro não definida
  MACRO_REDEFINED, // definição repetida de macro

  SYNONYM_IN_SECTION, // sinônimo definido em seção
  SYNONYM_REDEFINED, // sinônimo definido em seção

  MISSING_TEXT, // seção de texto ausente

  SEMANTIC_UNDEFINED_ERROR // erro indefinido
};

/*
void print_error(Error *err)
{
  char *err_type;
  char *err_message;

  switch (err->err_type)
  {

  case LEXIC_ERROR:
    err_type = "léxico";

    switch (err->err_code)
    {
    case INVALID_CHAR:
      err_message = "";
      break;

    case INVALID_LABEL:
      err_message = "";
      break;

    default:
      break;
    }
    break;

  case SYNTAX_ERROR:
    err_type = "sintático";

    switch (err->err_code)
    {
    case DIRECTIVE_UNDEFINED:
      err_message = "";
      break;

    case OPCODE_INVALID:
      err_message = "";
      break;

    case INSTRUCTION_WRONG_ARG_COUNT:
      err_message = "";
      break;

    case DIRECTIVE_INVALID:
      err_message = "";
      break;

    case DIRECTIVE_WRONG_ARG_COUNT:
      err_message = "";
      break;

    case MACRO_WRONG_ARG_COUNT:
      err_message = "";
      break;

    case LABEL_TOO_LONG:
      err_message = "";
      break;

    case UNDEFINED_ERROR:
      err_message = "";
      break;

    default:
      break;
    }
    break;

  case SEMANTIC_ERROR:
    err_type = "semântico";

    switch (err->err_code)
    {
    case LABEL_UNDEFINED:
      err_message = "";
      break;

    case LABEL_REDEFINED:
      err_message = "";
      break;

    case MACRO_UNDEFINED:
      err_message = "";
      break;

    case MACRO_REDEFINED:
      err_message = "";
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  printf("[Erro %s]: %s", err_type, err_message);
}
*/
