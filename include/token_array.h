#include <stdlib.h>

typedef struct _token_array
{
  int capacity;
  int back;
  char **array;
} TokenArray;

TokenArray *init_token_array(int capacity);
void resize_token_array(TokenArray *token_array, int increment);
void insert_token(TokenArray *token_array, char *token);
