#include "../include/scanner.h"

int token_is_valid(char *token)
{
  for (int i = 0; i < strlen(token); i++)
  {
    if (!char_is_valid(token[i]))
      return 0;
  }
  return 1;
}

int token_is_symbol(char *token)
{
  if (!char_is_alpha_regular(token[0]) && !char_is_underscore(token[0]))
    return 0;

  for (int i = 1; token[i] != '\0'; i++)
  {
    if (!char_is_symbol_valid(token[i]))
      return 0;
  }
  return 1;
}

int token_is_number(char *token)
{
  for (int i = 0; token[i] != '\0'; i++)
  {
    if (!char_is_digit(token[i]))
      return 0;
  }
  return 1;
}

int token_is_operand(char *token)
{
  if (str_compare(token, "+"))
    return 1;
  return 0;
}

int scan_line(char *line, TokenArray *token_arr, SymbolArray *symbol_arr,
              InstructionNode *instr_arr, DirectiveNode *direc_arr)
{
  for (
      char *token = strtok(line, " ");
      token != NULL;
      token = strtok(NULL, " "))
  {
    // format token
    token = str_trim(token);
    capitalize_str(token);

    // ignore comments
    if (token[0] == ';' || token[0] == '\0')
      break;

    // error: invalid char
    if (!token_is_valid(token))
    {
      printf("Erro léxico: token \"%s\" contém caractere inváido.\n", token);
      return INVALID_CHAR;
    }

    if (str_compare(token, ":"))
    {
      printf("Erro léxico: símbolo \"%s\" não é válido sem rótulo.\n", token);
      return INVALID_DECLARATION;
    }

    // store symbol
    if (token[strlen(token) - 1] == ':')
      token[strlen(token) - 1] = '\0';

    // error invalid token start
    if (!token_is_operand(token) &&
        !token_is_number(token) &&
        !token_is_symbol(token))
    {
      printf("Erro léxico: símbolo \"%s\" é inválido.\n", token);
      return INVALID_SYMBOL;
    }
    else if (token_is_symbol(token))
    {
      if (!search_symbol(symbol_arr, token) &&
          !search_instruction_tmp(instr_arr, token) &&
          !search_directive_tmp(direc_arr, token) &&
          !str_compare(token, "TEXT") && !str_compare(token, "DATA"))
        insert_symbol(symbol_arr, token);
    }

    insert_token(token_arr, token);
  }
  insert_token(token_arr, "\n");

  return 0;
}

int scan_file(char *i_filename, TokenArray *token_arr, SymbolArray *symbol_arr,
              InstructionNode *instr_arr, DirectiveNode *direc_arr)
{
  FILE *i_file = fopen(i_filename, "r");
  // check input file errors
  if (i_file == NULL)
  {
    printf("Erro: arquivo fonte inexistente.\n");
    return FILE_ERROR;
  }

  int err = 0;
  char *line = NULL;
  size_t len = 0;
  size_t read;
  int line_count = 0;

  while ((read = getline(&line, &len, i_file)) != EOF)
  {
    line_count++;
    err = scan_line(line, token_arr, symbol_arr, instr_arr, direc_arr);

    if (err != 0)
    {
      printf("Linha %d.\n", line_count);
      return err;
    }
  }
  fclose(i_file);

  return err;
}
