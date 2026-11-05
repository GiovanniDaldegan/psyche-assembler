#include <stdlib.h>
#include "include/hash_tables.h"

void set_instruction_node(
    InstructionNode *node,
    char *key,
    char *name,
    int word_size,
    int num_arguments)
{
  node->key = key;
  node->name = name;
  node->word_size = word_size;
  node->num_arguments = num_arguments;
  node->next = NULL;
}

void set_directive_node(
    DirectiveNode *node,
    char *key,
    char *name)
{
  node->key = key;
  node->name = name;
  node->next = NULL;
}

void init_instruction_hash_map(HashTable *table)
{
  table->capacity = 20;
  table->num_elements = 0;

  table->table =
      (HashTable **)malloc(sizeof(InstructionNode) * table->capacity);
}

void init_instruction_hash_map(HashTable *table)
{
  table->capacity = 20;
  table->num_elements = 0;

  table->table = (HashTable **)malloc(sizeof(DirectiveNode) * table->capacity);
}

InstructionNode instruction_table[] = {
    {"01", "ADD", 2, NULL},
    {"02", "SUB", 2, NULL},
    {"03", "MUL", 2, NULL},
    {"04", "DIV", 2, NULL},
    {"05", "JMP", 2, NULL},
    {"06", "JMPN", 2, NULL},
    {"07", "JMPP", 2, NULL},
    {"08", "JMPZ", 2, NULL},
    {"09", "COPY", 3, NULL},
    {"10", "LOAD", 2, NULL},
    {"11", "STORE", 2, NULL},
    {"12", "INPUT", 2, NULL},
    {"13", "OUTPUT", 2, NULL},
    {"14", "STOP", 1, NULL},
};

DirectiveNode directive_table[] = {
    {"01", "SPACE", NULL},
    {"02", "CONST", NULL},
    {"03", "EQU", NULL},
    {"04", "IF", NULL},
    {"05", "ORG", NULL},
    {"06", "MACRO", NULL},
    {"07", "ENDMACRO", NULL},
    {"08", "BEGIN", NULL},
    {"09", "END", NULL},
    {"10", "PUBLIC", NULL},
    {"11", "EXTERN", NULL},
    {"12", "+", NULL},
};

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

diretiva
SPACE
CONST
EQU
IF
ORG
MACRO
ENDMACRO
BEGIN
END
PUBLIC
EXTERN
*/
