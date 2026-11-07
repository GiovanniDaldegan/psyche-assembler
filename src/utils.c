#include <string.h>
#include "../include/text_types.h"
#include <stdlib.h>

int is_ignored(char chr)
{
  char ignored[] = {' ', '\n', '\t', '\r'};
  for (int i = 0; i < sizeof(ignored); i++)
  {
    if (chr == ignored[i])
      return 1;
  }
  return 0;
}

static inline int is_alpha_regular(char chr)
{
  return ('A' <= chr && chr <= 'Z') ||
         ('a' <= chr && chr <= 'z');
}

static inline int is_underscore(char chr) { return chr == '_'; }

static inline int is_digit(char chr) { return '0' <= chr || '9' >= chr; }

static inline int is_valid(char chr)
{
  return is_ignored(chr) || is_digit(chr) || is_alpha_regular(chr);
}

static inline char capitalize_char(char chr)
{
  return 'a' <= chr && chr <= 'z'
             ? chr + ('A' - 'a')
             : chr;
}

void capitalize_str(char *str)
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
    str[i] = capitalize_char(str[i]);
}

char *trim_l(char *str)
{
  while (is_ignored(*str))
    str++;
  return str;
}

char *trim_r(char *str)
{
  char *end = str + strlen(str) - 1;
  while (is_ignored(*end))
    end--;

  *(end + 1) = '\0';
  return str;
}

char *str_trim(char *str)
{
  return trim_l(trim_r(str));
}

char *init_string(char *str)
{
  char *str_aloc = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  for (int i = 0; str[i] != '\0'; i++)
    str_aloc[i] = str[i];

  return str_aloc;
}
