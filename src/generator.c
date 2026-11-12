/**
 * iterar sobre token arr
 * substituir sinônimos por valores
 * realizar rotinas de diretivas
 * - SPACE [X] insere X 00s
 * - CONST X insere X
 * - EQU já foi
 * - IF avalia valor (talvez sinônimo) e pula ou não linha
 * - + soma endereço com valor (talvez sinônimo)
 * - BEGIN faz porra alguma
 * - END também faz nada
 * - PUBLIC X insere X na tabela de definições
 * - EXTERN rótulo temporariamente 0
 * - SECTION inicia seção (separação dos novos token arrays)
 *
 * adicionar posição de referência a rótulo à lista de pendências
 *
 * substituir instruções por opcodes
 * adicionar valor dos rótulos nas referências
 */

int get_numeric_value(SynonymArray *synonym_arr, char *token)
{
  if (str_is_numeric(token))
    return atoi(token);
  else
  {
    Synonym *syn = search_synonym(synonym_arr, token);
    if (syn)
      return syn->value;
  }

  return -1;
}

char *concat_sections(char *sec_text, char *sec_data)
{
  char *code =
      (char *)malloc(sizeof(char) * (strlen(sec_text) + strlen(sec_data)));

  strcpy(code, sec_text);
  strcpy(code + strlen(sec_text), sec_data);

  return code;
}

int inital_pass(TokenArray *token_arr,
                DirectiveNode *directive_arr,
                SymbolArray *symbol_array,
                SynonymArray *synonym_arr,
                SymbolArray *def_table,
                SymbolArray *use_table,
                char *code)
{
  int in_text = 0,
      in_data = 0,
      ignore_next_line = 0;

  int line_count = 1,
      empty_line = 0;

  TokenArray *text_arr = init_token_array(300);
  TokenArray *data_arr = init_token_array(300);
  TokenArray **curr_sec_tok_arr = NULL;

  for (char **token_ptr = token_arr->array;
       (token_ptr - token_arr->array) < token_arr->back;
       token_ptr++)
  {
    if (ignore_next_line)
    {
      while (!str_compare(*(token_ptr + 1), "\n"))
        token_ptr++;
      ignore_next_line = 0;
      continue;
    }

    if (str_is_end_of_line(*token_ptr))
    {
      if (empty_line)
        continue;

      // while token_ptr lays on a \n, skip
      empty_line = 1;

      if (!curr_sec_tok_arr)
        continue;
      else
      {
        insert_token(*curr_sec_tok_arr, "\n");
        continue;
      }
    }
    else
    {
      empty_line = 0;
    }

    // directives
    if (str_compare(*token_ptr, "SECTION"))
    {
      token_ptr++;
      if (str_compare(*token_ptr, "TEXT"))
      {
        in_text = 1;
        in_data = 0;
        curr_sec_tok_arr = &text_arr;
      }
      else if (str_compare(*token_ptr, "DATA"))
      {
        in_text = 0;
        in_data = 1;
        curr_sec_tok_arr = &data_arr;
      }
      else
      {
        printf("Erro doido: seção inválida em geração de código.\n");
        return -1;
      }
    }

    else if (str_compare(*(token_ptr + 1), "EQU"))
    {
      if (!str_is_numeric(*(token_ptr + 2)))
      {
        printf("%s não é número??\n", *(token_ptr + 2));
      }

      insert_synonym(synonym_arr, *(token_ptr), atoi(*(token_ptr + 2)));
      token_ptr += 2;

      // doesn't change the token array
      empty_line = 1;
    }

    else if (str_compare(*token_ptr, "CONST"))
    {

      token_ptr++;
      int val = get_numeric_value(synonym_arr, *token_ptr);
      char *val_str = int_to_str(val);

      if (!curr_sec_tok_arr)
        continue;
      else
        insert_token(*curr_sec_tok_arr, val_str);
    }

    else if (str_compare(*token_ptr, "+"))
    {
      token_ptr++;
      char *val_str;
      if (str_is_numeric(*(token_ptr)))
        val_str = *token_ptr;
      else
      {
        Synonym *syn = search_synonym(synonym_arr, *(token_ptr));
        if (syn)
          val_str = int_to_str(syn->value);
        else
          val_str = "";
      }

      if (!curr_sec_tok_arr)
      {
        printf("Erro na geração: diertiva inválida fora de seção.\n");
        return GENERATION_FAILED;
      }
      else
        insert_token(*curr_sec_tok_arr, val_str);
    }

    else if (str_compare(*token_ptr, "SPACE"))
    {
      int val = get_numeric_value(synonym_arr, *(token_ptr + 1));

      if (val != -1)
      {
        for (int i = 0; i < val; i++)
        {
          if (!curr_sec_tok_arr)
          {
            printf("Erro na geração: diertiva inválida fora de seção.\n");
            return GENERATION_FAILED;
          }
          else
            insert_token(*curr_sec_tok_arr, "0");

          // stops at the last "0"
          if (i == val - 1)
            token_ptr++;
        }
      }
      else
      {
        if (!curr_sec_tok_arr)
        {
          printf("Erro na geração: diertiva inválida fora de seção.\n");
          return GENERATION_FAILED;
        }
        else
          insert_token(*curr_sec_tok_arr, "0");
      }
    }

    else if (str_compare(*token_ptr, "IF"))
    {
      printf("if com %s: ", *(token_ptr + 1));
      if (get_numeric_value(synonym_arr, *(token_ptr + 1)))
      {
        printf("não ignora\n");
        ignore_next_line = 1;
      }
      else
        printf("ignora\n");

      token_ptr++;

      // doesn't change the token array
      empty_line = 1;
    }

    else if (str_compare(*token_ptr, "PUBLIC"))
    {
      Symbol *symb = search_symbol(symbol_array, *(token_ptr + 1));
      if (symb)
      {
        printf("definição públiac de %s\n", *(token_ptr + 1));
        insert_symbol_ready(def_table, symb);
      }
      else
      {
        printf("símbolo \"%s\" não definido!\n", *(token_ptr + 1));
      }
      token_ptr++;

      // doesn't change the token array
      empty_line = 1;
    }

    else if (str_compare(*(token_ptr + 1), "EXTERN"))
    {
      Symbol *symb = search_symbol(symbol_array, *(token_ptr));
      if (symb)
      {
        printf("rótulo externo %s\n", *(token_ptr));
        insert_symbol_ready(use_table, symb);
      }
      else
      {
        printf("símbolo não definido!\n");
      }
      token_ptr += 2;
    }

    else if (str_compare(*token_ptr, "BEGIN") ||
             str_compare(*token_ptr, "END"))
    {
      // ignore directive (but starts new line)
      continue;
    }

    // any other token (non-directive, non )
    else
    {

      if (!curr_sec_tok_arr)
      {
        printf("Erro na geração: diertiva inválida fora de seção.\n");
        return GENERATION_FAILED;
      }
      else
        insert_token(*curr_sec_tok_arr, *token_ptr);
    }
  }
}

int first_pass(TokenArray *token_arr,
               InstructionNode *instruction_arr)
{
  for (char **token_ptr = token_arr->array;
       (token_ptr - token_arr->array) < token_arr->back;
       token_ptr++)
  {
  }
}

int second_pass(TokenArray *token_arr,
                InstructionNode *instruction_arr,
                SymbolArray *sybmol_arr)
{
  for (char **token_ptr = token_arr->array;
       (token_ptr - token_arr->array) < token_arr->back;
       token_ptr++)
  {
  }
}

int two_pass(TokenArray *token_arr,
             InstructionNode *instruction_arr,
             DirectiveNode *directive_arr,
             SymbolArray *sybmol_arr,
             char *gen_code)
{
  SynonymArray *synonym_arr = init_synonym_array(20);
  SymbolArray *def_table = init_symbol_array(300);
  SymbolArray *use_table = init_symbol_array(300);

  inital_pass(token_arr, directive_arr, sybmol_arr, synonym_arr,
              def_table, use_table, gen_code);
  /*
    printf("DEF TABLE\n");
    for (int i = 0; i < def_table->back; i++)
    {
      printf("%s:%d |", def_table->array[i]->token, def_table->array[i]->value);
    }

    printf("USE TABLE\n");
    for (int i = 0; i < use_table->back; i++)
    {
      printf("%s:%d |", use_table->array[i]->token, use_table->array[i]->value);
    }
   */
  // first_pass();
  // second_pass(token_arr, instruction_arr, directive_arr, sybmol_arr);
}
