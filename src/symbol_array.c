#include <stdlib.h>
#include "../include/symbol_array.h"

#define SYMBOL_INITIAL_CAPACITY 300

Symbol *init_symbol(char *token, int value, int defined)
{
  Symbol *symb = (Symbol *)malloc(sizeof(Symbol));

  symb->token = (char*) malloc(sizeof(char) * (strlen(token) + 1));
  strcpy(symb->token, token);
  symb->value = value;
  symb->defined = defined;

  return symb;
}

SymbolArray *init_symbol_array(int capacity)
{
  SymbolArray *symbol_arr = (SymbolArray *)malloc(sizeof(SymbolArray));

  capacity = SYMBOL_INITIAL_CAPACITY;
  symbol_arr->capacity = capacity;
  symbol_arr->back = 0;
  symbol_arr->array = (Symbol **)malloc(sizeof(Symbol *) * capacity);

  return symbol_arr;
}

void resize_symbol_array(SymbolArray *symbol_arr, int increment)
{
  increment = SYMBOL_INITIAL_CAPACITY;
  symbol_arr->capacity += increment;
  Symbol **new_array = (Symbol **)malloc(sizeof(Symbol *) * symbol_arr->capacity);

  for (int i = 0; i < symbol_arr->back; i++)
    new_array[i] = symbol_arr->array[i];

  free(symbol_arr->array);
  symbol_arr->array = new_array;
}

void insert_symbol(SymbolArray *symbol_arr, char *token)
{
  if (symbol_arr->back > symbol_arr->capacity)
  {
    printf("Vetor de símbolos deu overflow!\n");
    return;
  }
  else if (symbol_arr->back == symbol_arr->capacity)
    resize_symbol_array(symbol_arr, SYMBOL_INITIAL_CAPACITY);

  symbol_arr->array[symbol_arr->back++] = init_symbol(token, -1, 0);
}

Symbol *search_symbol(SymbolArray *symbol_arr, char *token)
{
  for (int i = 0; i < symbol_arr->back; i++)
  {
    if (str_compare(symbol_arr->array[i]->token, token))
      return symbol_arr->array[i];
  }
  return NULL;
}
