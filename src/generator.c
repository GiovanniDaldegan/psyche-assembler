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
      ignore_curr_line = 0,
      ignore_next_line = 0;

  int line_count = 1,
      token_count = 0;

  TokenArray *text_arr = init_token_array(300);
  TokenArray *data_arr = init_token_array(300);
  TokenArray **curr_sec_tok_arr;

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

    token_count++;
    if (str_is_end_of_line(*token_ptr))
    {
      token_count = 0;
      if (in_data)
      {
        insert_token(data_arr, "\n");
      }
      else if (in_text)
      {
        insert_token(text_arr, "\n");
      }
    }
    else if (str_compare(*token_ptr, "SECTION"))
    {
      token_ptr++;
      if (str_compare(*token_ptr, "TEXT"))
      {
        printf("seção %s\n", *token_ptr);
        in_text = 1;
        in_data = 0;
        curr_sec_tok_arr = &text_arr;
      }
      else if (str_compare(*token_ptr, "DATA"))
      {
        printf("seção %s\n", *token_ptr);
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
      // ignore_curr_line = 1;
      token_ptr += 2;
    }

    else if (str_compare(*token_ptr, "CONST"))
    {
      // if (curr_sec_tok_arr == NULL)
      // {
      //   printf("seção não definida, token %s\n", *token_ptr);
      //   continue;
      // }
      // insert_token(*curr_sec_tok_arr, *(token_ptr + 1));

      token_ptr++;
      int val = get_numeric_value(synonym_arr, *token_ptr);
      char *val_str = int_to_str(val);

      if (in_data)
      {
        insert_token(data_arr, val_str);
      }
      else if (in_text)
      {
        insert_token(text_arr, val_str);
      }
    }

    else if (str_compare(*token_ptr, "+"))
    {
      // if (curr_sec_tok_arr == NULL)
      // {
      //   printf("seção não definida, token %s\n", *token_ptr);
      //   continue;
      // }
      // insert_token(*curr_sec_tok_arr, *(token_ptr + 1));

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

      if (in_data)
      {
        insert_token(data_arr, val_str);
      }
      else if (in_text)
      {
        insert_token(text_arr, val_str);
      }
    }

    else if (str_compare(*token_ptr, "SPACE"))
    {
      int val = get_numeric_value(synonym_arr, *(token_ptr + 1));

      if (val != -1)
      {
        for (int i = 0; i < val; i++)
        {
          // if (curr_sec_tok_arr == NULL)
          // {
          //   printf("seção não definida, token %s\n", *token_ptr);
          //   continue;
          // }
          // insert_token(*curr_sec_tok_arr, "0");

          if (in_data)
          {
            insert_token(data_arr, "0");
          }
          else if (in_text)
          {
            insert_token(text_arr, "0");
          }

          // stops at the last "0"
          if (i == val - 1)
            token_ptr++;
        }
      }
      else
      {
        // if (curr_sec_tok_arr == NULL)
        // {
        //   printf("seção não definida, token %s\n", *token_ptr);
        //   continue;
        // }
        // insert_token(*curr_sec_tok_arr, "0");

        if (in_data)
        {
          insert_token(data_arr, "0");
        }
        else if (in_text)
        {
          insert_token(text_arr, "0");
        }
        // token_ptr++;
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

    else
    {
      /*
      printf("token qualquer %s, tok c %d\n", *token_ptr, token_count);

      char *tok = *token_ptr;
      Synonym *syn = search_synonym(synonym_arr, tok);
      if (syn)
      {
        tok = int_to_str(syn->value);
        printf("synonynynyn %s -> %s\n", syn->token, tok);
        // tok = (char *)malloc(strlen(itoa(syn->value, tok, 10)) + 1);
      }
      */

      // if (curr_sec_tok_arr == NULL)
      // {
      //   printf("seção não definida, token %s\n", *token_ptr);
      //   continue;
      // }
      // insert_token(*curr_sec_tok_arr, *token_ptr);

      if (in_data)
      {
        insert_token(data_arr, *token_ptr);
      }
      else if (in_text)
      {
        insert_token(text_arr, *token_ptr);
      }
    }
  }

  printf("coleta TEXT:\n");
  for (int i = 0; i < text_arr->back; i++)
  {
    printf("%s|", text_arr->array[i]);
  }
  printf("coleta DATA\n");
  for (int i = 0; i < data_arr->back; i++)
  {
    printf("%s|", data_arr->array[i]);
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
