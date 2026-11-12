#include "../include/semantic_analyzer.h"

/**
 * assumimos que todas as expressões são válidas, então
 * - símbolo/sinônimo no início da linha é declaração
 * - símbolo/sinônimo não no início é erro de não declarado
 * - DATA e TEXT são declarações váldias de seção
 *
 * a especificação não fala de checagem de instrução ou diretiva utilizada em
 * seções indevidas
 * precisa checar se EQU vem antes das seções?
 * precisa checar se IF está em TEXT?
 */

int define_synonym(SynonymArray *synonym_arr, char *token, int value)
{
  insert_synonym(synonym_arr, token, value);
  return SEMANTIC_OK;
}

int semantic_analysis(TokenArray *token_arr,
                      SymbolArray *symbol_arr,
                      SynonymArray *synonym_arr)
{
  int text_section = 0,
      in_text = 0,
      in_data = 0;
  int line_count = 1,
      token_count = 0;
  int err = SEMANTIC_OK;

  SymbolArray *first_ref = init_symbol_array(300);

  char **token_ptr = token_arr->array;
  for (; (token_ptr - token_arr->array) < token_arr->back; token_ptr++)
  {
    token_count++;

    if (str_is_end_of_line(*token_ptr))
    {
      line_count++;
      token_count = 0;
      continue;
    }

    if (str_compare(*token_ptr, "TEXT"))
    {
      in_data = 0;
      in_text = 1;
      text_section = 1;
    }
    else if (str_compare(*token_ptr, "DATA"))
    {
      in_text = 0;
      in_data = 1;
    }

    // syn
    if (str_compare(*(token_ptr + 1), "EQU"))
    {
      if (in_text || in_data)
      {
        printf("Erro semântico: sinônimo \"%s\" definido em seção ",
               *token_ptr);
        if (in_data)
          printf("DATA.\n");
        else if (in_text)
          printf("TEXT.\n");

        err = SYNONYM_IN_SECTION;
        break;
      }

      if (search_synonym(synonym_arr, *token_ptr))
      {
        printf("Erro semântico: redefinição de sinônimo \"%s\".\n", *token_ptr);
        err = SYNONYM_REDEFINED;
        break;
      }
      define_synonym(synonym_arr, *token_ptr, atoi(*(token_ptr + 2)));
      continue;
    }

    Symbol *symb = search_symbol(symbol_arr, *token_ptr);
    if (symb)
    {
      // saves the line count of the first reference to any symbol
      if (!search_symbol(first_ref, *token_ptr))
      {
        Symbol *new_symb = init_symbol(*token_ptr, line_count, 0);
        insert_symbol_ready(first_ref, new_symb);
      }

      if (symb->defined && token_count == 1)
      {
        printf("Erro semântico: redefinição do rótulo \"%s\".\n", *token_ptr);
        err = LABEL_REDEFINED;
        break;
      }

      if (symb->defined && search_synonym(synonym_arr, *token_ptr))
      {
        printf("Erro semântico: sinônimo numérico \"%s\" já definido com "
               "diretiva \"EQU\" também é definido como rótulo.\n",
               *token_ptr);
        err = LABEL_IS_SYNONYM;
        break;
      }

      if (!in_text && !in_data)
      {
        printf("Erro semântico: rótulo \"%s\" definido fora de seção válida."
               "\n",
               *token_ptr);
        err = LABEL_OUT_SECTION;
        break;
      }

      if (token_count == 1 || str_compare(*(token_ptr - 1), "PUBLIC"))
        update_symbol(symb, line_count, 1);
    }
  }

  if (err == SEMANTIC_OK)
  {
    if (!text_section)
    {
      printf("Erro semântico: seção de programa \"TEXT\" não declarada.\n");
      err = MISSING_TEXT;
    }
    else
    {
      for (int i = 0; i < symbol_arr->back; i++)
      {
        Symbol *symb = symbol_arr->array[i];
        if (!symb->defined && !search_synonym(synonym_arr, symb->token))
        {
          printf("Erro semântico: rótulo \"%s\" não definido.\n",
                 symbol_arr->array[i]->token);
          err = LABEL_UNDEFINED;
          line_count = search_symbol(first_ref, symb->token)->value;
          break;
        }
      }
    }
  }

  if (err != SEMANTIC_OK)
    printf("Linha: %d.\n", line_count);

  return err;
}
