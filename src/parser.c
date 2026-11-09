#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/hash_table.h"

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
      if (char_is_alpha_regular(token[i]) || char_is_underscore(token[i]))
        token_type = PARSE_SYMBOL;
      else if (char_is_digit(token[i]))
        token_type = PARSE_NUMBER;
      // erro doido: caractere inválido no parsing
      else if (!char_is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return -1;
      }
      break;

    case PARSE_SYMBOL:
      // erro doido: caractere inválido no parsing
      if (!char_is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return -1;
      }
      break;

    case PARSE_NUMBER:
      if (!char_is_valid(token[i]) || char_is_alpha_regular(token[i]))
        return -1;
      break;

    default:
      return -1;
    }
  }

  return token_type;
}

int token_is_reserved(InstructionNode *instruction_arr,
                      DirectiveNode *directive_arr,
                      char *token)
{
  return search_instruction_tmp(instruction_arr, token) ||
         search_directive_tmp(directive_arr, token);
}

int token_is_end_of_line(char *token)
{
  return str_compare(token, "\n");
}

int parse_directive(DirectiveNode *directive_arr,
                    char **direc_ptr,
                    DirectiveNode *direc)
{
  // 1st token: argument on right
  char *token = *direc_ptr + 1;

  if (token_is_end_of_line(token))
  {
    if (direc->num_params == 0)
      return 0;
    else
    {
      // syntax error: too few arguments
      printf("Erro sintático: poucos argumentos para diretiva %s\n",
             direc->name);
      return 1;
    }
  }

  if (str_compare(direc->name, "CONST") ||
      str_compare(direc->name, "EQU") ||
      str_compare(direc->name, "+"))
  {
    if (get_token_type(token) != PARSE_NUMBER)
    {
      printf("Erro sintático: argumento %s inválido para diretiva %s.\n",
             token, direc->name);
      return 1;
    }
  }

  else if (str_compare(direc->name, "EXTERN") ||
           str_compare(direc->name, "SECTION"))
  {
    if (get_token_type(token) != PARSE_SYMBOL)
    {
      printf("Erro sintático: argumento %s inválido para diretiva %s.\n",
             token, direc->name);
      return 1;
    }
  }

  else if (str_compare(direc->name, "IF"))
  {
    int type = get_token_type(token);
    if (type != PARSE_NUMBER && type != PARSE_SYMBOL)
    {
      printf("Erro sintático: argumento %s inválido para diretiva %s.\n",
             token, direc->name);
      return 1;
    }
  }

  // 2nd token: \n
  token++;
  if (!token_is_end_of_line(token))
  {
    // syntax error: too few arguments
    printf("Erro sintático: muitos argumentos para diretiva %s\n",
           direc->name);
    return 1;
  }

  return 0;
}

int parse_instruction(InstructionNode *instruction_arr,
                      DirectiveNode *directive_arr,
                      char **instr_ptr,
                      InstructionNode *instr)
{
  // 1st token: label
  char *token = *instr_ptr + 1;

  if (token_is_end_of_line(token))
  {
    printf("Erro sintático: poucos argumentos para %s.\n", instr->name);
    return 1;
  }

  if (get_token_type(token) != PARSE_SYMBOL)
  {
    // syntax error: invalid instruction argument
    printf("Erro sintático: argumento inválido para %s.\n", instr->name);
    return 1;
  }

  if (token_is_reserved(instruction_arr, directive_arr, token))
  {
    printf("Erro sintático: argumento de %s é palavra reservada %s.\n",
           instr->name, token);
    return 1;
  }

  // 2nd token?: macro
  token++;

  if (token_is_end_of_line(token))
    return 0;

  DirectiveNode *direc = search_directive_tmp(directive_arr, token);
  if (!direc)
  {
    // syntax error: too few arguments
    printf("Erro sintático: poucos argumentos para %s\n", instr->name);
    return 1;
  }
  else
  {
    if (!str_compare(direc->name, "+"))
    {
      printf("Erro sintático: diretiva indevida %s após instrução %s.\n",
             direc->name, instr->name);
      return 1;
    }

    if (!parse_directive(directive_arr, &token, direc))
      return 1;
  }

  {
    // syntax error: too many arguments
    printf("Erro sintático: muitos argumentos para instrução %s.\n",
           instr->name);
    return 1;
  }
}

int parse_line(char **line_start,
               InstructionNode *instruction_arr,
               DirectiveNode *directive_arr,
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
      {"", "enf_of_array", 0, 0, 0, (void *)NULL},
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
      {"", "enf_of_array", 0, (void *)NULL},
  };
}

