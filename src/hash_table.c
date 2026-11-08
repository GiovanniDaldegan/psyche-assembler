#include <stdlib.h>
#include "../include/hash_tables.h"

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

/*
InstructionNode instruction_table[] = {
    {"", "ADD", 1, 2, 1, (void *)NULL},
    {"", "SUB", 2, 2, 1, (void *)NULL},
    {"", "MUL", 3, 2, 1, (void *)NULL},
    {"", "DIV", 4, 2, 1, (void *)NULL},
    {"", "JMP", 5, 2, 1, (void *)NULL},
    {"", "JMPN", 6, 2, 1, (void *)NULL},
    {"", "JMPP", 7, 2, 1, (void *)NULL},
    {"", "JMPZ", 8, 2, 1, (void *)NULL},
    {"", "COPY", 9, 3, 2, (void *)NULL},
    {"", "LOAD", 10, 2, 1, (void *)NULL},
    {"", "STORE", 11, 2, 1, (void *)NULL},
    {"", "INPUT", 12, 2, 1, (void *)NULL},
    {"", "OUTPUT", 13, 2, 1, (void *)NULL},
    {"", "STOP", 14, 1, 0, (void *)NULL},
};

DirectiveNode directive_table[] = {
    {"", "SPACE", 0, (void *)NULL},
    {"", "CONST", 1, (void *)NULL},
    {"", "EQU", 2, (void *)NULL},
    {"", "IF", 1, (void *)NULL},
    {"", "ORG", 1, (void *)NULL},
    {"", "MACRO", -1, (void *)NULL},
    {"", "ENDMACRO", 0, (void *)NULL},
    {"", "BEGIN", 0, (void *)NULL},
    {"", "END", 0, (void *)NULL},
    {"", "PUBLIC", 1, (void *)NULL},
    {"", "EXTERN", 1, (void *)NULL},
    {"", "+", 2, (void *)NULL},
    {"", "SECTION", 1, (void *)NULL},
};
*/

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
