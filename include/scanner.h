#include "../include/hash_table.h"
#include "../include/token_array.h"

enum ScannerStringState
{
  SCAN_NUMBER,
  SCAN_LABEL,
  SCAN_IGNORE,
  SCAN_NONE,
  SCAN_UNDEFINED
};

int token_is_valid(char *token);

void scan_line(char *line, TokenArray *token_arr, HashTable *symbol_table);
void scan_file(char *i_filename, TokenArray *token_arr, HashTable *symbol_table);
