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

Symbol *init_symbol(char *token, int value, int defined);
Synonym *init_synonym(char *token, int value);

SymbolArray *init_symbol_array(int capacity);
void resize_symbol_array(SymbolArray *symbol_array, int increment);
void insert_symbol(SymbolArray *symbol_array, char *token);
void update_symbol(SymbolArray *symbol_array, char *token, int value, int defined);

SynonymArray *init_synonym_array(int capacity);
void resize_synonym_array(SynonymArray *synonym_array, int increment);
void insert_synonym(SynonymArray *synonym_array, char *token, int value);
