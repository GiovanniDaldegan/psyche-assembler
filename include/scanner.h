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

int token_is_valid(char *token);

TokenArray *init_array_token(int capacity);
void token_array_resize(TokenArray *arr, int increment);
void token_array_resize(TokenArray *token_array, int increment);
void token_array_insert(TokenArray *token_array, char *token);

void scan_line(char *line, TokenArray *token_arr, HashTable *symbol_table);
void scan_file(char *i_filename, TokenArray *token_arr, HashTable *symbol_table);
