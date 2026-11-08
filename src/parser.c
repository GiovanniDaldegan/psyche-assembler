#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/hash_tables.h"

int get_token_type(char *token)
{
  if (!token)
    return -1;

  // token_type in ParserStringState
  int token_type = PARSE_NONE;

  for (int i = 0; i < strlen(token); i++)
  {
    switch (token_type)
    {
    case PARSE_NONE:
      if (is_alpha_regular(token[i]) || is_underscore(token[i]))
        token_type = PARSE_SYMBOL;
      else if (is_digit(token[i]))
        token_type = PARSE_NUMBER;
      // erro doido: caractere inválido no parsing
      else if (!is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return -1;
      }
      break;

    case PARSE_SYMBOL:
      // erro doido: caractere inválido no parsing
      if (!is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return -1;
      }
      break;

    case PARSE_NUMBER:
      if (!is_valid(token[i]) || is_alpha_regular(token[i]))
        return -1;
      break;

    default:
      return -1;
    }
  }

  return token_type;
}

int parse_line(char **line_start,
               InstructionNode **instruction_arr,
               DirectiveNode **directive_arr,
               int token_count)
{
}

int parse_code()
{
  InstructionNode instruction_arr[] = {
      {"", "ADD", 1, 2, 1, (void *)NULL},
      {"", "SUB", 2, 2, 1, (void *)NULL},
      {"", "MUL", 3, 2, 1, (void *)NULL},
      {"", "DIV", 4, 2, 1, (void *)NULL},
      {"", "JMP", 5, 2, 1, (void *)NULL},
      {"", "JMPN", 6, 2, 1, (void *)NULL},
      {"", "JMPP", 7, 2, 1, (void *)NULL},
      {"", "JMPZ", 8, 2, 1, (void *)NULL},
      {"", "COPY", 9, 3, 2, (void *)NULL},
      {"", "LOAD", 10, 2, 1, (void *)NULL},
      {"", "STORE", 11, 2, 1, (void *)NULL},
      {"", "INPUT", 12, 2, 1, (void *)NULL},
      {"", "OUTPUT", 13, 2, 1, (void *)NULL},
      {"", "STOP", 14, 1, 0, (void *)NULL},
  };

  DirectiveNode directive_arr[] = {
      {"", "SPACE", 0, (void *)NULL},
      {"", "CONST", 1, (void *)NULL},
      {"", "EQU", 2, (void *)NULL},
      {"", "IF", 1, (void *)NULL},
      {"", "ORG", 1, (void *)NULL},
      {"", "MACRO", -1, (void *)NULL},
      {"", "ENDMACRO", 0, (void *)NULL},
      {"", "BEGIN", 0, (void *)NULL},
      {"", "END", 0, (void *)NULL},
      {"", "PUBLIC", 1, (void *)NULL},
      {"", "EXTERN", 1, (void *)NULL},
      {"", "+", 2, (void *)NULL},
      {"", "SECTION", 1, (void *)NULL},
  };
}

