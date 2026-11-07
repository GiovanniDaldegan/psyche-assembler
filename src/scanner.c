#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/scanner.h"
#include "../include/error_types.h"
#include "../src/utils.c"
#include "../include/text_types.h"

char **scan_line(char *line)
{
  int chr_limit;
  int chr_counter = 0;
  char *string;
  int string_state;
  char **output;

  while (chr_counter < chr_limit)
  {
    char chr = line[chr_counter];

    if (!is_valid(chr))
    {
      // ERROR: caractere inválido
      return INVALID_CHAR;
    }

    switch (string_state)
    {
    case SCAN_NONE:
    case SCAN_IGNORE:
      // pode iniciar nova string
      if (is_alpha_regular(chr) || is_underscore(chr))
        string_state = SCAN_LABEL;
      else if (is_digit(chr))
        string_state = SCAN_NUMBER;

      // reiniciar string;
      break;

    case SCAN_NUMBER:
      // deve continuar number ou virar ignore
      if (is_ignored(chr))
        string_state = SCAN_IGNORE;
      else if (is_digit(chr))
        string = strcat(string, &chr);
      else
      {
        string_state = SCAN_UNDEFINED;

        // ERROR: número inválido / label iniciada com número
        // if (is_alpha_regular(chr))
        //   return INVALID_LABEL;
      }
      break;

    case SCAN_LABEL:
      // deve continuar LABEL ou virar ignore
      if (is_ignored(chr))
        string_state = SCAN_IGNORE;
      else
        string = strcat(string, &chr);
      break;

    default:
      // ERROR: já lascou, não identificado
      break;
    }
  }

  return output;
}

char **scan_file(char *i_filename)
{
  FILE *i_file = fopen(i_filename, "r");
  // check input file errors
  if (i_file == NULL)
  {
    printf("ERRO: arquivo inexistente.\n");
    return NULL;
  }

  char **token_array = (char **)malloc(sizeof(char *) * 100);

  char *line = NULL;
  size_t len = 0;
  size_t read;

  int pos = 0;
  while ((read = getline(&line, &len, i_file)) != EOF)
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

      // store token
      token_array[pos++] = init_string(token);
    }
  }

  fclose(i_file);
}

int main()
{
  scan_file("code_comment_CRLF.asm");
  return 0;
}
