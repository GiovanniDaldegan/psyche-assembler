typedef struct _symbol
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

Symbol *init_symbol(char *symbol, int value, int defined);

SymbolArray *init_symbol_array(int capacity);
void resize_symbol_array(SymbolArray *symbol_arr, int increment);
void insert_symbol(SymbolArray *symbol_array, Symbol *symbol);
