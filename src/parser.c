#include "../include/parser.h"

int get_token_type(char *token)
{
  if (!token)
    return -1;

  // token_type in ParserStringState
  int token_type = PARSE_NONE;

  for (int i = 0; i < strlen(token); i++)
  {
    switch (token_type)
    {
    case PARSE_NONE:
      if (char_is_alpha_regular(token[i]) || char_is_underscore(token[i]))
        token_type = PARSE_SYMBOL;
      else if (char_is_digit(token[i]))
        token_type = PARSE_NUMBER;
      // erro doido: caractere inválido no parsing
      else if (!char_is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return SYNTAX_UNDEFINED_ERROR;
      }
      break;

    case PARSE_SYMBOL:
      // erro doido: caractere inválido no parsing
      if (!char_is_valid(token[i]))
      {
        printf("Erro sintático: erro doido, caractere inválido no parsing.\n");
        return SYNTAX_UNDEFINED_ERROR;
      }
      break;

    case PARSE_NUMBER:
      if (!char_is_valid(token[i]) || !char_is_digit(token[i]))
      {
        printf("Erro sintático: erro doido, número inválido no parsing.\n");
        return SYNTAX_UNDEFINED_ERROR;
      }
      break;

    default:
      return -1;
    }
  }

  return token_type;
}

int token_is_reserved(InstructionNode *instruction_arr,
                      DirectiveNode *directive_arr,
                      char *token)
{
  return search_instruction_tmp(instruction_arr, token) ||
         search_directive_tmp(directive_arr, token) ||
         token == "TEXT" || token == "DATA";
}

int parse_directive(InstructionNode *instruction_arr,
                    DirectiveNode *directive_arr,
                    SynonymArray *synonym_arr,
                    char ***direc_ptr,
                    DirectiveNode *direc,
                    int debug_flag)
{
  if (debug_flag)
    printf("direc %s (", **direc_ptr);

  // 1st token: argument on right
  (*direc_ptr)++;
  char *token = **direc_ptr;

  if (str_is_end_of_line(token))
  {
    if (direc->num_params == 0)
    {
      if (debug_flag)
        printf(")");
      return 0;
    }
    else
    {
      // space can be used with no arg
      if (str_compare(direc->name, "SPACE"))
      {
        if (debug_flag)
          printf(")");
        return 0;
      }

      if (debug_flag)
        printf(">\n\n");

      // syntax error: too few arguments
      printf("Erro sintático: poucos argumentos para diretiva %s\n",
             direc->name);
      return DIRECTIVE_FEW_ARGS;
    }
  }
  else
  {
    if (direc->num_params == 0)
    {
      printf("Erro sintático: muitos argumentos para diretiva %s.\n",
             direc->name);
      return DIRECTIVE_MANY_ARGS;
    }
  }

  if (debug_flag)
    printf("arg %s", token);

  if (token_is_reserved(instruction_arr, directive_arr, token))
  {
    if (debug_flag)
      printf(">\n\n");

    printf("Erro sintático: argumento \"%s\" de diretiva é palavra reservada",
           token);
    return DIRECTIVE_INVALID_ARG;
  }

  // numeric arg
  if (str_compare(direc->name, "EQU"))
  {
    if (get_token_type(token) != PARSE_NUMBER)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             token, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
    if (debug_flag)
      printf(")");

    insert_synonym(synonym_arr, *(*direc_ptr - 2), 0);

    (*direc_ptr)++;
    return 0;
  }

  // symbolic arg
  else if (str_compare(direc->name, "PUBLIC") ||
           str_compare(direc->name, "SECTION"))
  {
    if (str_compare(direc->name, "SECTION") &&
        !str_compare(**direc_ptr, "DATA") &&
        !str_compare(**direc_ptr, "TEXT"))
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: nome de seção deve ser \"TEXT\" ou \"DATA\", "
             "\"%s\" é inválido.\n",
             **direc_ptr);
      return DIRECTIVE_INVALID_ARG;
    }
    else if (get_token_type(token) != PARSE_SYMBOL)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             token, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
    if (debug_flag)
      printf(")");

    (*direc_ptr)++;
    return 0;
  }

  // both symbol or numeric arg
  else if (str_compare(direc->name, "IF"))
  {
    int type = get_token_type(token);
    if (type != PARSE_NUMBER && type != PARSE_SYMBOL)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: argumento \"%s\" inválido para diretiva %s.\n",
             token, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }
    if (debug_flag)
      printf(")");

    (*direc_ptr)++;
    return 0;
  }

  // numeric arg or accepts synonym arg (with EQU)
  else if (str_compare(direc->name, "CONST") ||
           str_compare(direc->name, "SPACE") ||
           str_compare(direc->name, "+"))
  {
    if (get_token_type(token) != PARSE_NUMBER &&
        !search_synonym(synonym_arr, token))
    {
      printf("Erro sintático: argumento \"%s\" não foi declarado como constante"
             " numérica antes de referência, portanto é inválido para diretiva "
             "%s.\n",
             token, direc->name);
      return DIRECTIVE_INVALID_ARG;
    }

    (*direc_ptr)++;
    return 0;
  }

  if (debug_flag)
    printf(">\n\n");
  printf("Erro sintático: diretiva %s não definida.\n", direc->name);
  return DIRECTIVE_UNDEFINED;
}

/**
 * parses an expression like: <label> | <label> <directive> <operand>
 * increments `expr_ptr` util it lays on the last token on expression
 */
int parse_expression(InstructionNode *instruction_arr,
                     DirectiveNode *directive_arr,
                     SynonymArray *synonym_arr,
                     char ***expr_ptr,
                     int debug_flag)
{
  if (debug_flag)
    printf("expr %s (", **expr_ptr);

  // 1st token: label
  char *token_ptr = **expr_ptr;

  if (str_is_end_of_line(token_ptr))
  {
    if (debug_flag)
      printf(".\n");

    // talvez deixe pro parse_line decidir
    printf("Erro sintático: expressão vazia.\n");
    return EXPRESSION_INVALID;
  }

  if (get_token_type(token_ptr) != PARSE_SYMBOL)
  {
    if (debug_flag)
      printf(">\n\n");

    printf("Erro sintático: expressão iniciada com token indevido %s\n",
           token_ptr);
    return EXPRESSION_INVALID;
  }

  // 2nd token: \n?, label?, directive?
  (*expr_ptr)++;
  token_ptr = **expr_ptr;

  if (str_is_end_of_line(token_ptr) || token_is_symbol(token_ptr))
  {
    if (debug_flag)
      printf(")");
    return 0;
  }

  DirectiveNode *direc = search_directive_tmp(directive_arr, token_ptr);
  if (!direc)
  {
    if (debug_flag)
      printf(">\n\n");

    printf("Erro sintático: token \"%s\" inválido em expressão.\n", token_ptr);
    return EXPRESSION_INVALID;
  }

  int err = parse_directive(instruction_arr, directive_arr, synonym_arr,
                            expr_ptr, direc, debug_flag);

  if (debug_flag && err == 0)
    printf(")");

  return err;
}

// NOTE: doesn't increment `instr_ptr`
int parse_instruction(InstructionNode *instruction_arr,
                      DirectiveNode *directive_arr,
                      SynonymArray *synonym_arr,
                      char **instr_ptr,
                      InstructionNode *instr,
                      int debug_flag)
{
  if (debug_flag)
    printf("instr %s (", instr->name);

  // 1st token: expression
  char **token_ptr = instr_ptr + 1;

  if (str_is_end_of_line(*token_ptr))
  {
    if (instr->num_params != 0)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: poucos argumentos para instrução %s.\n",
             instr->name);
      return INSTRUCTION_FEW_ARGS;
    }

    if (debug_flag)
      printf(")");
    return 0;
  }
  else
  {
    if (instr->num_params == 0)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: muitos argumentos para instrução %s.\n",
             instr->name);
      return INSTRUCTION_MANY_ARGS;
    }
  }

  if (get_token_type(*token_ptr) != PARSE_SYMBOL)
  {
    if (debug_flag)
      printf(">\n\n");

    // syntax error: invalid instruction argument
    printf("Erro sintático: argumento \"%s\" inválido para instrução %s.\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }

  if (token_is_reserved(instruction_arr, directive_arr, *token_ptr))
  {
    if (debug_flag)
      printf(">\n\n");

    printf("Erro sintático: argumento \"%s\" da instrução \"%s\" é palavra "
           "reservada, portanto inválido.\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }

  // 1st expr
  int err = parse_expression(instruction_arr, directive_arr, synonym_arr,
                             &token_ptr, debug_flag);
  if (err != 0)
    return err;

  // token after 1st expr: \n, expression

  if (str_is_end_of_line(*token_ptr))
  {
    if (instr->num_params != 1)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: poucos argumentos para instrução %s.\n",
             instr->name);
      return INSTRUCTION_FEW_ARGS;
    }
    if (debug_flag)
      printf(")");
    return 0;
  }
  else
  {
    if (instr->num_params == 1)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: muitos argumentos para instrução %s\n",
             instr->name);
      return INSTRUCTION_MANY_ARGS;
    }
  }

  if (str_compare(instr->name, "COPY"))
  {
    if (get_token_type(*token_ptr) != PARSE_SYMBOL)
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: argumento \"%s\" inválido para instrução %s.\n",
             *token_ptr, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    if (token_is_reserved(instruction_arr, directive_arr, *token_ptr))
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: argumento \"%s\" da instrução \"%s\" é palavra "
             "reservada, portanto inválido.\n",
             *token_ptr, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    // 2nd expr
    err = parse_expression(instruction_arr, directive_arr, synonym_arr,
                           &token_ptr, debug_flag);
    if (err != 0)
      return err;

    // token after 2nd expr: \n

    if (str_is_end_of_line(*token_ptr))
    {
      if (debug_flag)
        printf(")");
      return 0;
    }

    if (debug_flag)
      printf(">\n\n");

    printf("Erro sintático: não foi possível fazer parsing da instrução %s.\n",
           instr->name);
    return INSTRUCTION_FEW_ARGS;
  }

  printf("Erro sintático: falha no parsing de instrução inválida.\n");
  return INSTRUCTION_FAIL;

  /*
  DirectiveNode *direc = search_directive_tmp(directive_arr, *token_ptr);
  if (!direc)
  {
    if (debug_flag)
      printf(">\n\n");

    // syntax error: too few arguments
    printf("Erro sintático: argumento \"%s\" inválido para instrução %s\n",
           *token_ptr, instr->name);
    return INSTRUCTION_INVALID_ARG;
  }
  else
  {
    if (!str_compare(direc->name, "+"))
    {
      if (debug_flag)
        printf(">\n\n");

      printf("Erro sintático: diretiva indevida %s após instrução %s.\n",
             direc->name, instr->name);
      return INSTRUCTION_INVALID_ARG;
    }

    err = parse_directive(instruction_arr, directive_arr, &token_ptr, direc, debug_flag);

    if (debug_flag)
      printf(")");
    return err;
  }
   */
}

// NOTE: doesn't increment `line_start`, parse_code iterator deals with that
int parse_line(char **line_start,
               int token_count,
               InstructionNode *instruction_arr,
               DirectiveNode *directive_arr,
               SynonymArray *synonym_arr,
               int debug_flag)
{
  int err = 0;
  char **token_ptr = line_start;

  // empty line
  if (token_count == 0)
    return 0;

  // line: 0 label, 1 istruction/directive/symbol (equ), 2 args/expressions

  if (get_token_type(*token_ptr) != PARSE_SYMBOL)
  {
    // syntax error: line starts with constant numeric value
    printf("Erro sintático: linha iniciada com token indevido \"%s\".\n",
           *token_ptr);
    return INVALID_LINE;
  }

  // is symbol
  if (!token_is_reserved(instruction_arr, directive_arr, *token_ptr))
  {
    if (debug_flag)
      printf("%s: ", *token_ptr);

    // 2nd token: instruction, directive
    token_ptr++;

    if (str_is_end_of_line(*token_ptr))
    {
      printf("Erro sintático: rótulo \"%s\" sem instrução ou diretiva.\n",
             *(token_ptr - 1));
      return INVALID_LINE;
    }
  }

  InstructionNode *instr = search_instruction_tmp(instruction_arr, *token_ptr);
  if (instr)
    return parse_instruction(instruction_arr, directive_arr, synonym_arr,
                             token_ptr, instr, debug_flag);

  DirectiveNode *direc = search_directive_tmp(directive_arr, *token_ptr);
  if (direc)
  {
    err = parse_directive(instruction_arr, directive_arr, synonym_arr,
                          &token_ptr, direc, debug_flag);

    if (err == SYNTAX_OK &&
        !str_is_end_of_line(*token_ptr) &&
        token_count > (token_ptr - line_start))
    {
      if (debug_flag)
        printf("\n");

      printf("Erro sintático: muitos argumentos para diretiva %s.\n",
             direc->name);
      return DIRECTIVE_MANY_ARGS;
    }

    return err;
  }

  printf("Erro sintático: linha não reconhecida (iniciada em %s).\n",
         *line_start);
  return INVALID_LINE;
}

int parse_code(TokenArray *token_arr, SymbolArray *symbol_arr,
               InstructionNode *instr_arr, DirectiveNode *direc_arr,
               int debug_flag)
{
  int line_count = 0;
  int token_line_count = 0;
  char **line_start = token_arr->array;

  SynonymArray *synonym_arr = init_synonym_array(300);

  for (int i = 0; i < token_arr->back; i++)
  {
    if (!str_compare(token_arr->array[i], "\n"))
      token_line_count++;
    else
    {
      line_count++;
      if (token_line_count != 0)
      {
        int err =
            parse_line(line_start, token_line_count, instr_arr, direc_arr,
                       synonym_arr, debug_flag);

        if (debug_flag)
          printf("\n");
        // stop parsing
        if (err != 0)
        {
          printf("Linha %d.\n", line_count);
          return err;
        }
      }
      token_line_count = 0;
      line_start = &token_arr->array[i + 1];
    }
  }

  return SYNTAX_OK;
}
