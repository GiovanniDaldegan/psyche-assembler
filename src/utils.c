#include "../include/utils.h"

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

int char_is_underscore(char chr) { return chr == '_'; }

static inline int char_is_valid_special(char chr)
{
  return chr == '_' ||
         chr == ':' ||
         chr == '+';
}

static inline int char_is_digit(char chr) { return '0' <= chr && chr <= '9'; }

static inline int char_is_valid(char chr)
{
  return char_is_ignored(chr) ||
         char_is_digit(chr) ||
         char_is_alpha_regular(chr) ||
         char_is_valid_special(chr);
}

static inline int char_is_symbol_valid(char chr)
{
  return char_is_digit(chr) ||
         char_is_alpha_regular(chr) ||
         char_is_underscore(chr);
}

static inline char capitalize_char(char chr)
{
  return 'a' <= chr && chr <= 'z'
             ? chr + ('A' - 'a')
             : chr;
}

int str_is_numeric(char *str)
{
  while (*str != '\0')
  {
    if (!char_is_digit(*str))
    {
      return 0;
    }
    str++;
  }
  return 1;
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
  if (!str1 || !str2)
    return 0;

  int i = 0;

  do
  {
    if (str1[i] != str2[i])
      return 0;
    i++;
  } while (str1[i - 1] != '\0' && str2[i - 1] != '\0');

  return 1;
}

int str_is_end_of_line(char *token) { return str_compare(token, "\n"); }

void print_string_array(char **arr, int n_elements)
{
  for (int i = 0; i < n_elements; i++)
  {
    if (!str_compare(arr[i], "\n"))
      i < n_elements - 1
          ? printf("%s, ", arr[i])
          : printf("%s\n", arr[i]);
  }
}

char *get_filename_extention(char *filename)
{
  char *back = filename + strlen(filename);
  while (back > filename)
  {
    if (*back == '.')
      return back;

    back--;
  }
  return NULL;
}

// `separator = 0` adds no separator
char *array_to_str(char **str_arr, int str_count, char separator)
{
  int total_size = 0;
  int sep_incr = (separator == '\0') ? 0 : 1;

  for (int i = 0; i < str_count; i++)
    total_size += strlen(str_arr[i]) + sep_incr;

  char *str = (char *)malloc(total_size);
  char *pos = str;
  for (int i = 0; i < str_count; i++)
  {
    strcpy(pos, str_arr[i]);
    if (separator != '\0')
      *(pos + 1) = separator;
    pos += strlen(str_arr[i]) + sep_incr;
  }

  return str;
}

char *int_to_str(long long value)
{
  if (value == 0)
  {
    char *str = (char *)malloc(2);
    str[0] = '0';
    str[1] = '\0';
    return str;
  }

  int sign = (value < 0) ? -1 : 1;
  value *= sign;

  int len = 0;
  char *str = (char *)malloc(35),
       *str_invert = (char *)malloc(35);

  while (value > 0)
  {
    str_invert[len++] = '0' + (value % 10);
    value /= 10;
  }

  str[(sign == -1) + len] = '\0';
  int j = 0;
  for (int i = (sign == -1) + len - 1; i >= (sign == -1); i--)
  {
    str[i] = str_invert[j++];
  }
  if (sign == -1)
    str[0] = '-';

  return str;
}
