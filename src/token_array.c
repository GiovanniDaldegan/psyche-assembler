#include "../include/token_array.h"

#define TOKEN_ARRAY_INCREMENT 100

TokenArray *init_token_array(int capacity)
{
  TokenArray *token_arr = (TokenArray *)malloc(sizeof(TokenArray));

  token_arr->capacity = capacity;
  token_arr->back = 0;
  token_arr->array = (char **)malloc(sizeof(char *) * capacity);

  return token_arr;
}

void resize_token_array(TokenArray *token_arr, int increment)
{
  token_arr->capacity += increment;
  char **new_array = (char **)malloc(sizeof(char **) * token_arr->capacity);

  for (int i = 0; i < token_arr->back; i++)
    new_array[i] = token_arr->array[i];

  free(token_arr->array);
  token_arr->array = new_array;
}

void insert_token(TokenArray *token_arr, char *token)
{
  if (token_arr->back > token_arr->capacity)
  {
    printf("Vetor de tokens deu overflow!\n");
    return;
  }
  else if (token_arr->back == token_arr->capacity)
    resize_token_array(token_arr, TOKEN_ARRAY_INCREMENT);

  token_arr->array[token_arr->back++] = init_string(token);
}
