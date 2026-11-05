typedef struct _instr_node
{
  char *key;
  char *name;
  int opcode;
  int size_words;
  int num_params;
  InstructionNode *next;
} InstructionNode;

typedef struct _direc_node
{
  char *key;
  char *name;
  int num_params;
  DirectiveNode *next;
} DirectiveNode;

typedef struct _symb_node
{
  char *key;
  char *name;
  int addr;
  int is_relative;
  int *pendencies;
  SymbolNode *next;
} SymbolNode;

typedef struct _ext_symb_node
{
  char *key;
  char *name;
  int ref_addr;
  ExternSymbolNode *next;
} ExternSymbolNode;

typedef struct _macro_node
{
  char *key;
  char *name;
  char *num_params;
  char *code;
  MacroNode *next;
} MacroNode;

typedef struct _hash_table
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
