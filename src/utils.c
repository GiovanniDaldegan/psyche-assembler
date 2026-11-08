#include <string.h>
#include "../include/text_types.h"
#include <stdlib.h>

int char_is_ignored(char chr)
{
  char ignored[] = {' ', '\n', '\t', '\r'};
  for (int i = 0; i < sizeof(ignored); i++)
  {
    if (chr == ignored[i])
      return 1;
  }
  return 0;
}

static inline int char_is_alpha_regular(char chr)
{
  return ('A' <= chr && chr <= 'Z') ||
         ('a' <= chr && chr <= 'z');
}

static inline int char_is_underscore(char chr) { return chr == '_'; }

static inline int char_is_digit(char chr) { return '0' <= chr || '9' >= chr; }

static inline int char_is_valid(char chr)
{
  return char_is_ignored(chr) ||
         char_is_digit(chr) ||
         char_is_alpha_regular(chr);
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
  while (char_is_ignored(*str))
    str++;
  return str;
}

char *trim_r(char *str)
{
  char *end = str + strlen(str) - 1;
  while (char_is_ignored(*end))
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
  int i;
  for (i = 0; str[i] != '\0'; i++)
    str_aloc[i] = str[i];
  str_aloc[i] = '\0';

  return str_aloc;
}

int str_compare(char *str1, char *str2)
{
  int i = 0;

  do
  {
    if (str1[i] != str2[i])
      return 0;
    i++;
  } while (str1[i - 1] != '\0' && str2[i - 1] != '\0');

  return 1;
}

void print_string_array(char **arr, int n_elements)
{
  for (int i = 0; i < n_elements; i++)
  {
    i != n_elements - 1
        ? printf("%s, ", arr[i])
        : printf("%s\n", arr[i]);
  }
}
