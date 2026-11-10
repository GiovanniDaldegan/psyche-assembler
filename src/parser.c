#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/utils.h"
#include "../include/hash_table.h"
#include "../include/error_types.h"

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
        return SYNTAX_UNDEFINED_ERROR;
      }
      break;

    case PARSE_SYMBOL:
      // erro doido: caractere inválido no parsing
      if (!char_is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return SYNTAX_UNDEFINED_ERROR;
      }
      break;

    case PARSE_NUMBER:
      if (!char_is_valid(token[i]) || !char_is_digit(token[i]))
      {
        printf("Erro sintático: erro doido, número inválido no parsing.\n");
        return SYNTAX_UNDEFINED_ERROR;
      }
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
         search_directive_tmp(directive_arr, token) ||
         token == "TEXT" || token == "DATA";
}

int token_is_end_of_line(char *token)
{
  return str_compare(token, "\n");
}

int parse_directive(char **direc_ptr,
                    DirectiveNode *direc)
{
  // 1st token: argument on right
  char **token_ptr = direc_ptr + 1;

  if (token_is_end_of_line(*token_ptr))
  {
    if (direc->num_params == 0)
      return 0;
    else
    {
      // space can be used with no arg
      if (str_compare(direc->name, "SPACE"))
        return 0;

      // syntax error: too few arguments
      printf("Erro sintático: poucos argumentos para diretiva %s\n",
             direc->name);
      return DIRECTIVE_FEW_ARGS;
    }
  }

  if (str_compare(direc->name, "CONST") ||
      str_compare(direc->name, "SPACE") ||
      str_compare(direc->name, "EQU") ||
      str_compare(direc->name, "+"))
  {
    if (get_token_type(*token_ptr) != PARSE_NUMBER)
    {
      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             *token_ptr, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
  }

  else if (str_compare(direc->name, "EXTERN") ||
           str_compare(direc->name, "SECTION"))
  {
    if (get_token_type(*token_ptr) != PARSE_SYMBOL)
    {
      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             *token_ptr, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
  }

  else if (str_compare(direc->name, "IF"))
  {
    int type = get_token_type(*token_ptr);
    if (type != PARSE_NUMBER && type != PARSE_SYMBOL)
    {
      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             *token_ptr, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
  }

  // 2nd token: \n
  token_ptr++;

  if (!token_is_end_of_line(*token_ptr))
  {
    // syntax error: too few arguments
    printf("Erro sintático: muitos argumentos para diretiva %s\n",
           direc->name);
    return DIRECTIVE_MANY_ARGS;
  }

  return 0;
}

int parse_instruction(InstructionNode *instruction_arr,
                      DirectiveNode *directive_arr,
                      char **instr_ptr,
                      InstructionNode *instr)
{
  // 1st token: label
  char **token_ptr = instr_ptr + 1;

  if (token_is_end_of_line(*token_ptr))
  {
    if (instr->num_params != 0)
    {
      printf("Erro sintático: poucos argumentos para instrução %s.\n",
             instr->name);
      return INSTRUCTION_FEW_ARGS;
    }
    return 0;
  }

  if (get_token_type(*token_ptr) != PARSE_SYMBOL)
  {
    // syntax error: invalid instruction argument
    printf("Erro sintático: argumento \"%s\" inválido para instrução %s.\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }

  if (token_is_reserved(instruction_arr, directive_arr, *token_ptr))
  {
    printf("Erro sintático: argumento \"%s\" da instrução \"%s\" é palavra "
           "reservada, portanto inválido.\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }

  // 2nd token: \n, label, macro
  token_ptr++;

  if (token_is_end_of_line(*token_ptr))
  {
    if (instr->num_params != 1)
    {
      printf("Erro sintático: poucos argumentos para instrução %s.\n",
             instr->name);
      return INSTRUCTION_FEW_ARGS;
    }
    return 0;
  }

  if (str_compare(instr->name, "COPY"))
  {
    if (get_token_type(*token_ptr) != PARSE_SYMBOL)
    {
      printf("Erro sintático: argumento \"%s\" inválido para instrução %s.\n",
             *token_ptr, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    if (token_is_reserved(instruction_arr, directive_arr, *token_ptr))
    {
      printf("Erro sintático: argumento \"%s\" da instrução \"%s\" é palavra "
             "reservada, portanto inválido.\n",
             *token_ptr, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    // 3rd token: \n, macro
    token_ptr++;

    if (token_is_end_of_line(*token_ptr))
      return 0;

    DirectiveNode *direc = search_directive_tmp(directive_arr, *token_ptr);
    if (!direc)
    {
      // syntax error: too few arguments
      printf("Erro sintático: argumento \"%s\" inválido para instrução %s\n",
             *token_ptr, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }
    else
    {
      if (!str_compare(direc->name, "+"))
      {
        printf("Erro sintático: diretiva indevida %s após instrução %s.\n",
               direc->name, instr->name);
        return INSTRUCTION_INVALID_ARG;
      }

      return parse_directive(token_ptr, direc);
    }
    return 0;
  }

  DirectiveNode *direc = search_directive_tmp(directive_arr, *token_ptr);
  if (!direc)
  {
    // syntax error: too few arguments
    printf("Erro sintático: argumento \"%s\" inválido para instrução %s\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }
  else
  {
    if (!str_compare(direc->name, "+"))
    {
      printf("Erro sintático: diretiva indevida %s após instrução %s.\n",
             direc->name, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    return parse_directive(token_ptr, direc);
  }
  /*
  {
    // syntax error: too many arguments
    printf("Erro sintático: muitos argumentos para instrução %s.\n",
           instr->name);
    return 1;
  }
  */
}

int parse_line(char **line_start,
               int token_count,
               InstructionNode *instruction_arr,
               DirectiveNode *directive_arr)
{
}

int parse_code(TokenArray *token_arr, SymbolArray *symbol_arr,
               InstructionNode *instr_arr, DirectiveNode *direc_arr)
{
  int token_line_count = 0;
  char **line_start = token_arr->array;
  for (int i = 0; i < token_arr->back; i++)
  {
    if (!str_compare(token_arr->array[i], "\n"))
      token_line_count++;
    else
    {
      if (token_line_count != 0)
      {
        int err =
            parse_line(line_start, token_line_count, instr_arr, direc_arr);

        // stop parsing
        // if (err != 0)
        //   return err;
      }
      token_line_count = 0;
      line_start = &token_arr->array[i + 1];
    }
  }
  printf("\n");
}

