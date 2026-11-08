#include "../include/hash_tables.h"

enum ScannerStringState
{
  SCAN_NUMBER,
  SCAN_LABEL,
  SCAN_IGNORE,
  SCAN_NONE,
  SCAN_UNDEFINED
};

typedef struct _token_array
{
  int capacity;
  int back;
  char **array;
} TokenArray;

TokenArray *init_array_token(int capacity);
void token_array_resize(TokenArray *arr, int increment);

void scan_line(char *line, TokenArray *token_arr, HashTable *symbol_table);
void scan_file(char *i_filename, TokenArray *token_arr, HashTable *symbol_table);
