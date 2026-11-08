typedef struct _token_array
{
  int capacity;
  int back;
  char **array;
} TokenArray;

TokenArray *init_array_token(int capacity);
void token_array_resize(TokenArray *token_array, int increment);
void token_array_insert(TokenArray *token_array, char *token);
