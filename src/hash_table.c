#include <stdlib.h>
#include "../include/hash_tables.h"
#include "../include/utils.h"
#include "../include/token_array.h"

#define HASH_TABLE_INCREMENT 20

void set_node_instruction(
    InstructionNode *node,
    char *key,
    char *name,
    int opcode,
    int size_words,
    int num_params)
{
  node->key = key;
  node->name = name;
  node->opcode = opcode;
  node->size_words = size_words;
  node->num_params = num_params;
  node->next = NULL;
}

void set_node_directive(
    DirectiveNode *node,
    char *key,
    char *name,
    int num_params)
{
  node->key = key;
  node->name = name;
  node->num_params = num_params;
  node->next = NULL;
}

void init_hash_table_instruction(HashTable *table, int capacity)
{
  table->capacity = HASH_TABLE_INCREMENT;
  table->num_elements = 0;

  table->table = (void **)malloc(sizeof(InstructionNode) * table->capacity);
}

void init_hash_table_directive(HashTable *table, int capacity)
{
  table->capacity = HASH_TABLE_INCREMENT;
  table->num_elements = 0;

  table->table = (void **)malloc(sizeof(DirectiveNode) * table->capacity);
}

InstructionNode *search_instruction_tmp(InstructionNode **instruction_arr, char *instruction_name)
{
  long unsigned arr_size = sizeof(instruction_arr) / sizeof(InstructionNode);
  for (int i = 0; i < arr_size; i++)
  {
    if (str_compare(instruction_arr[i]->name, instruction_name))
      return instruction_arr[i];
  }
  return NULL;
}

DirectiveNode *search_directive_tmp(DirectiveNode **directive_arr, char *directive_name)
{
  long unsigned arr_size = sizeof(directive_arr) / sizeof(DirectiveNode);
  for (int i = 0; i < arr_size; i++)
  {
    if (str_compare(directive_arr[i]->name, directive_name))
      return directive_arr[i];
  }
  return NULL;
}

SymbolNode *search_symbol_tmp(SymbolNode **symbol_arr, int symbol_count, char *symbol_name)
{
  for (int i = 0; i < symbol_count; i++)
  {
    if (str_compare(symbol_arr[i]->name, symbol_name))
      return symbol_arr[i];
  }
  return NULL;
}

/*
tabelas de consulta

op    instr     size
01    ADD       2
02    SUB       2
03    MUL       2
04    DIV       2
05    JMP       2
06    JMPN      2
07    JMPP      2
08    JMPZ      2
09    COPY      3
10    LOAD      2
11    STORE     2
12    INPUT     2
13    OUTPUT    2
14    STOP      1

diretiva  n params  obs
SPACE     0         XX
CONST     1         valor
EQU       2         registro tabela de sinônimos
IF        1         flag e linha seguinte
ORG       1         fodase
MACRO     ?         estado de macro, registro tabela nome de macro, append definição
ENDMACRO  0         fim estado de macro
BEGIN     0         estado de módulo
END       0         fim estado de macro
PUBLIC    1         registro tabela símbolo
EXTERN    1         registro tabela uso
SECTION   1         nome seção (EXCLUSIVO DATA ou TEXT)
*/
