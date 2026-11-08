#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/scanner.h"
#include "../include/utils.h"
#include "../include/token_array.h"
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

void scan_line(char *line, TokenArray *token_arr, HashTable *symbol_table)
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
      return;
      // INVALID_CHAR;
    }

    // store token
    if (token[strlen(token) - 1] == ':')
      token[strlen(token) - 1] = '\0';

    token_array_insert(token_arr, token);
  }
  token_array_insert(token_arr, "\n");
}

void scan_file(char *i_filename, TokenArray *token_arr, HashTable *symbol_table)
{
  FILE *i_file = fopen(i_filename, "r");
  // check input file errors
  if (i_file == NULL)
  {
    printf("ERRO: arquivo fonte inexistente.\n");
    return;
  }

  char *line = NULL;
  size_t len = 0;
  size_t read;

  while ((read = getline(&line, &len, i_file)) != EOF)
  {
    scan_line(line, token_arr, symbol_table);
  }

  fclose(i_file);
}
