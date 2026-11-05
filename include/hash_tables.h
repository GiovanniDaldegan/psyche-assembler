typedef struct
{
  char *key;
  char *name;
  int word_size;
  int num_arguments;
  InstructionNode *next;
} InstructionNode;

typedef struct
{
  char *key;
  char *name;
  int word_size;
  int num_arguments;
  DirectiveNode *next;
} DirectiveNode;

typedef struct
{
  char *key;
  char *name;
  int value;
  SymbolNode *next;
  int *pendencies;
} SymbolNode;

typedef struct
{
  char *key;
  char *name;
  int value;
  UseNode *next;
} UseNode;

typedef struct
{
  char *key;
  char *name;
  char *num_params;
  char *code;
  MacroNode *next;
} MacroNode;

typedef struct
{
  int num_elements, capacity;
  void **table;
} HashTable;

void set_instruction_node(InstructionNode *node, char *key, char *name, int word_size, int num_arguments);
void set_directive_node(DirectiveNode *node, char *key, char *name);

int instruction_hash_function(HashTable *table, char *key);
int directive_hash_function(HashTable *table, char *key);

void init_instruction_hash_table(HashTable *table, int num_elements, int capacity);
void init_directive_hash_table(HashTable *table, int num_elements, int capacity);
void init_symbol_hash_table(HashTable *table, int num_elements, int capacity);
// void init_macro_hash_table(HashTable *table, int num_elements, int capacity);

void insert_instruction(HashTable *table, InstructionNode *instruction);
void insert_directive(HashTable *table, DirectiveNode *directive);
void insert_symbol(HashTable *table, SymbolNode *symbol);
