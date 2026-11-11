#include <stdlib.h>

#include "utils.h"
#include "token_array.h"

typedef struct _instr_node
{
  char *key, *name;
  int opcode, size_words, num_params;
  struct _instr_node *next;
} InstructionNode;

typedef struct _direc_node
{
  char *key, *name;
  int num_params;
  struct _direc_node *next;
} DirectiveNode;

/*
typedef struct _symb_node
{
  char *key, *name;
  int addr, is_relative, *pendencies;
  struct _symb_node *next;
} SymbolNode;
*/

typedef struct _ext_symb_node
{
  char *key, *name;
  int ref_addr;
  struct _ext_symb_node *next;
} ExternSymbolNode;

typedef struct _macro_node
{
  char *key, *name, *num_params, *code;
  struct _macro_node *next;
} MacroNode;

typedef struct _hash_table
{
  int capacity, num_elements;
  void **table;
} HashTable;

/*
int instruction_hash_function(HashTable *table, char *key);

void init_hash_table_instruction(HashTable *instruction_table, int capacity);
void init_hash_table_directive(HashTable *directive_table, int capacity);
void init_hash_table_symbol(HashTable *symbol_table, int capacity);
void init_hash_table_macro(HashTable *macro_table, int capacity);

void set_node_instruction(InstructionNode *node, char *key, char *name, int opcode, int word_size, int num_params);
void set_node_directive(DirectiveNode *node, char *key, char *name, int num_params);
void set_node_symbol(SymbolNode *node, char *key, char *name);
void set_node_macro(MacroNode *node, char *key, char *name, char *num_params, char *code);

void insert_instruction(HashTable *instruction_table, InstructionNode *instruction);
void insert_directive(HashTable *directive_table, DirectiveNode *directive);
void insert_symbol(HashTable *symbol_table, SymbolNode *symbol);
void insert_macro(HashTable *macro_table, MacroNode *macro);

InstructionNode *search_instruction(HashTable *instruction_table, char *instruction_name);
DirectiveNode *search_directive(HashTable *directive_table, char *directive_name);
SymbolNode *search_symbol(HashTable *symbol_table, char *symbol_name);
MacroNode *search_macro(HashTable *macro_table, char *macro_name);
*/

// definições para implementação por vetor, por garantia
InstructionNode *search_instruction_tmp(InstructionNode *instruction_array, char *instruction_name);
DirectiveNode *search_directive_tmp(DirectiveNode *directive_array, char *directive_name);

// SymbolNode *search_symbol_tmp(SymbolNode *symbol_array, char *symbol_name);
// MacroNode *search_macro_tmp(MacroNode *macro_array, char *macro_name);
