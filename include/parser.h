#include "../include/hash_tables.h"

enum ParserStringState
{
  PARSE_NUMBER,
  PARSE_SYMBOL,
  PARSE_NONE,
  PARSE_UNDEFINED
};

int parse_line();
int parse_code();
