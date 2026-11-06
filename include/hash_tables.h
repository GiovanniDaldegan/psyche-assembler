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

int instruction_hash_function(HashTable *table, char *key);

void init_instruction_hash_table(HashTable *instruction_table, int num_elements, int capacity);
void init_directive_hash_table(HashTable *directive_table, int num_elements, int capacity);
void init_symbol_hash_table(HashTable *symbol_table, int num_elements, int capacity);
void init_macro_hash_table(HashTable *macro_table, int num_elements, int capacity);
// void init_macro_hash_table(HashTable *table, int num_elements, int capacity);

void set_instruction_node(InstructionNode *node, char *key, int opcode, int word_size, int num_params);
void set_directive_node(DirectiveNode *node, char *key, char *name, int num_params);
void set_symbol_node(SymbolNode *node, char *key, char *name);
void set_macro_node(MacroNode *node, char *key, char *name, char *num_params, char *code);

void insert_instruction(HashTable *instruction_table, InstructionNode *instruction);
void insert_directive(HashTable *directive_table, DirectiveNode *directive);
void insert_symbol(HashTable *symbol_table, SymbolNode *symbol);
void insert_macro(HashTable *macro_table, MacroNode *macro);

InstructionNode *search_instruction(HashTable *instruction_table, char *instruction_name);
DirectiveNode *search_directive(HashTable *directive_table, char *directive_name);
SymbolNode *search_symbol(HashTable *symbol_table, char *symbol_name);
MacroNode *search_macro(HashTable *macro_table, char *macro_name);

// definições para implementação por vetor, por garantia
InstructionNode *search_instruction_tmp(InstructionNode *instruction_table, char *instruction_name);
DirectiveNode *search_directive_tmp(DirectiveNode *directive_table, char *directive_name);
SymbolNode *search_symbol_tmp(SymbolNode *symbol_table, char *symbol_name);
MacroNode *search_macro_tmp(MacroNode *macro_table, char *macro_name);
