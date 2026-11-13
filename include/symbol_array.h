#include <stdlib.h>
#include <string.h>

typedef struct _symb
{
  char *token;
  int value;
  int defined;
} Symbol;

typedef struct _symb_arr
{
  int capacity, back;
  Symbol **array;
} SymbolArray;

typedef struct _syn
{
  char *token;
  int value;
} Synonym;

typedef struct _syn_arr
{
  int capacity, back;
  Synonym **array;
} SynonymArray;

typedef struct _pend
{
  char *token;
  int addr;
} Pendency;

typedef struct _pend_arr
{
  int capacity, back;
  Pendency **array;
} PendencyArray;

Symbol *init_symbol(char *token, int value, int defined);
void update_symbol(Symbol *symbol, int value, int defined);
Synonym *init_synonym(char *token, int value);
Pendency *init_pendency(char *token, int addr);

SymbolArray *init_symbol_array(int capacity);
void resize_symbol_array(SymbolArray *symbol_array, int increment);
void insert_symbol(SymbolArray *symbol_array, char *token);
Symbol *search_symbol(SymbolArray *symbol_array, char *token);

SynonymArray *init_synonym_array(int capacity);
void resize_synonym_array(SynonymArray *synonym_array, int increment);
void insert_synonym(SynonymArray *synonym_array, char *token, int value);
Synonym *search_synonym(SynonymArray *synonym_array, char *token);

PendencyArray *init_pendency_array(int capacity);
void resize_pendency_array(PendencyArray *pendency_array, int increment);
void insert_pendency(PendencyArray *pendency_array, char *token, int addr);
