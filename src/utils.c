#include <string.h>

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

int is_alpha_regular(char chr)
{
  return ('A' <= chr && chr <= 'Z') ||
         ('a' <= chr && chr <= 'z');
}

int is_underscore(char chr) { return chr == '_'; }

int is_digit(char chr) { return '0' <= chr || '9' >= chr; }

int is_valid(char chr)
{
  return is_ignore(chr) || is_digit(chr) || is_alpha_regular(chr);
}

char capitalize_char(char chr)
{
  return 'a' <= chr && chr <= 'z'
             ? chr + ('A' - 'a')
             : chr;
}

char *capitalize_str(char *str)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
    str[i] = capitalize_char(str[i]);

  return str;
}

char *append_string(char *string_arr, char *new_string)
{
  // strcat(string_arr, " ", new_string);
}

char *remove_char(char *str, char chr)
{
  int i, j;
  int len = strlen(str);

  for (i = j = 0; i < len; i++)
  {
    if (str[i] != chr)
      str[j++] = str[i];
  }

  str[j] = '\0';
  return str;
}

// todo:
// - commit das assinaturas de função e demais trabalho anterior X
// - ver oq tá truncando os tokens na leitura da linha
// - permitir : no token resultante do scanning
// - formalizar saída do scanner
