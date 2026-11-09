#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/scanner.h"
#include "../include/utils.h"
#include "../include/token_array.h"
#include "../include/symbol_array.h"
#include "../include/error_types.h"

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
  for (int i = 0; token[i] != '\0'; i++)
  {
    if (!char_is_symbol_valid(token[i]))
      return 0;
  }
  return 1;
}

void scan_line(char *line, TokenArray *token_arr, SymbolArray *symbol_arr)
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
      return;
      // INVALID_CHAR;
    }

    // error invalid token start
    if (strlen(token) > 1 &&
        !char_is_alpha_regular(token[0]) &&
        !char_is_underscore(token[0]))
    {
      printf("Erro léxico: símbolo \"%s\" contém caractere inicial inválido.\n",
             token);
      return;
    }

    if (str_compare(token, ":"))
    {
      printf("Erro sintático: símbolo %s não é válido.\n", token);
      return;
    }

    // store symbol
    if (token[strlen(token) - 1] == ':')
      token[strlen(token) - 1] = '\0';

    if (!token_is_symbol(token))
    {
      // error invalid expression
      printf("Erro léxico: expressão \"%s\" não tem separação de espaços.\n",
             token);
    }

    insert_token(token_arr, token);
  }
  insert_token(token_arr, "\n");
}

void scan_file(char *i_filename, TokenArray *token_arr, SymbolArray *symbol_arr)
{
  FILE *i_file = fopen(i_filename, "r");
  // check input file errors
  if (i_file == NULL)
  {
    printf("Erro: arquivo fonte inexistente.\n");
    return;
  }

  char *line = NULL;
  size_t len = 0;
  size_t read;

  while ((read = getline(&line, &len, i_file)) != EOF)
  {
    scan_line(line, token_arr, symbol_arr);
  }

  fclose(i_file);
}
