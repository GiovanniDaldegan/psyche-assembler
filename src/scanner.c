#include <stdio.h>
#include <string.h>

#include "../include/scanner.h"
#include "../include/error_types.h"
#include "../src/utils.c"
#include "../include/text_types.h"

char *scan_line(char *line)
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
      if (is_ignore(chr))
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
      if (is_ignore(chr))
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

int scan_file(char *i_filename, char *o_filename)
{
  FILE *i_file = fopen(i_filename, "r");
  // input file errors

  int line_counter;
  int line_limit;
  // int position_counter;

  char *output;

  while (line_counter < line_limit)
  {
    // dividir arquivo em linhas

    // obter próxima linha
    char *line;
    char *line_scan = scan_line(line);
    strcat(output, line_scan);

    line_counter++;
  }

  fclose(i_filename);

  FILE *o_file = fopen(o_filename, "w");
  // output file errors
}

int main()
{
  return 0;
}
