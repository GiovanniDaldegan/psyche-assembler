#include "../include/symbol_array.h"

#define SYMBOL_INITIAL_CAPACITY 300
#define SYNONYM_INITIAL_CAPACITY 20

Symbol *init_symbol(char *token, int value, int defined)
{
  Symbol *symb = (Symbol *)malloc(sizeof(Symbol));

  symb->token = (char *)malloc(sizeof(char) * (strlen(token) + 1));
  strcpy(symb->token, token);
  symb->value = value;
  symb->defined = defined;

  return symb;
}

void update_symbol(Symbol *symb, int value, int defined)
{
  symb->value = value;
  symb->defined = defined;
}

Synonym *init_synonym(char *token, int value)
{
  Synonym *syn = (Synonym *)malloc(sizeof(Synonym));

  syn->token = (char *)malloc(sizeof(char) * (strlen(token) + 1));
  strcpy(syn->token, token);
  syn->value = value;

  return syn;
}

Pendency *init_pendency(char *token, int addr)
{
  Pendency *pend = (Pendency *)malloc(sizeof(Pendency));

  pend->token = (char *)malloc(sizeof(char) * (strlen(token) + 1));
  strcpy(pend->token, token);
  pend->addr = addr;

  return pend;
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

void insert_symbol_ready(SymbolArray *symbol_arr, Symbol *symb)
{
  if (symbol_arr->back > symbol_arr->capacity)
  {
    printf("Vetor de símbolos deu overflow!\n");
    return;
  }
  else if (symbol_arr->back == symbol_arr->capacity)
    resize_symbol_array(symbol_arr, SYMBOL_INITIAL_CAPACITY);

  symbol_arr->array[symbol_arr->back++] = symb;
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

SynonymArray *init_synonym_array(int capacity)
{
  SynonymArray *symbol_arr = (SynonymArray *)malloc(sizeof(SynonymArray));

  capacity = SYNONYM_INITIAL_CAPACITY;
  symbol_arr->capacity = capacity;
  symbol_arr->back = 0;
  symbol_arr->array = (Synonym **)malloc(sizeof(Synonym *) * capacity);

  return symbol_arr;
}

void resize_synonym_array(SynonymArray *synonym_array, int increment)
{
  increment = SYNONYM_INITIAL_CAPACITY;
  synonym_array->capacity += increment;
  Synonym **new_array = (Synonym **)malloc(sizeof(Synonym *) * synonym_array->capacity);

  for (int i = 0; i < synonym_array->back; i++)
    new_array[i] = synonym_array->array[i];

  free(synonym_array->array);
  synonym_array->array = new_array;
}

void insert_synonym(SynonymArray *synonym_array, char *token, int value)
{
  if (synonym_array->back > synonym_array->capacity)
  {
    printf("Vetor de símbolos deu overflow!\n");
    return;
  }
  else if (synonym_array->back == synonym_array->capacity)
    resize_synonym_array(synonym_array, SYNONYM_INITIAL_CAPACITY);

  synonym_array->array[synonym_array->back++] =
      init_synonym(token, value);
}

Synonym *search_synonym(SynonymArray *synonym_array, char *token)
{
  for (int i = 0; i < synonym_array->back; i++)
  {
    if (str_compare(synonym_array->array[i]->token, token))
      return synonym_array->array[i];
  }
  return NULL;
}

PendencyArray *init_pendency_array(int capacity)
{
  PendencyArray *pendency_array = (PendencyArray *)malloc(sizeof(PendencyArray));

  capacity = SYNONYM_INITIAL_CAPACITY;
  pendency_array->capacity = capacity;
  pendency_array->back = 0;
  pendency_array->array = (Pendency **)malloc(sizeof(Pendency *) * capacity);

  return pendency_array;
}

void resize_pendency_array(PendencyArray *pendency_array, int increment)
{
  increment = SYNONYM_INITIAL_CAPACITY;
  pendency_array->capacity += increment;
  Pendency **new_array = (Pendency **)malloc(sizeof(Pendency *) * pendency_array->capacity);

  for (int i = 0; i < pendency_array->back; i++)
    new_array[i] = pendency_array->array[i];

  free(pendency_array->array);
  pendency_array->array = new_array;
}

void insert_pendency(PendencyArray *pendency_array, char *token, int addr)
{
  if (pendency_array->back > pendency_array->capacity)
  {
    printf("Vetor de símbolos deu overflow!\n");
    return;
  }
  else if (pendency_array->back == pendency_array->capacity)
    resize_pendency_array(pendency_array, SYNONYM_INITIAL_CAPACITY);

  pendency_array->array[pendency_array->back++] = init_pendency(token, addr);
}
