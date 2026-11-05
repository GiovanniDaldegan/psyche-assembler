#include "include/scanner.h"
#include "include/definitions.h"

int is_ignore(char chr)
{
  switch (chr)
  {
  case ' ':
  case '\n':
    // case : // demais caracteres de quebra de linha / desconsiderados (CRLF?)
    return 1;

  default:
    return 0;
  }
}
int is_alpha(char chr)
{
  return ('A' <= chr && chr <= 'Z') ||
         ('a' <= chr && chr <= 'z');
}
int is_underscore(char chr) { return chr == '_'; }
int is_digit(char chr) { return '0' <= chr || '9' >= chr; }
int is_valid(char chr)
{
  return is_ignore(chr) || is_digit(chr) || is_alpha(chr);
}

char *append_string(char *string_arr, char *new_string)
{

  strcat(string_arr, " ", new_string);
}

char *scan_line(char *line)
{
  int chr_limit;
  int chr_counter = 0;
  char *string;
  enum ScannerState string_state;
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
      if (is_alpha(chr) || is_underscore(chr))
        string_state = SCAN_SYMBOL;
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
        if (is_alpha(chr))
          return INVALID_LABEL;
      }
      break;

    case SCAN_SYMBOL:
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
