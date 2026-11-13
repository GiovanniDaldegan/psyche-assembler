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

#include "../include/generator.h"

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

int insert_in_section(TokenArray **section_ptr, char *token)
{
  if (section_ptr == NULL)
    return OUT_OF_SECTION;

  insert_token(*section_ptr, token);
  return GENERATION_OK;
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
                SynonymArray *synonym_arr,
                SymbolArray *def_table,
                SymbolArray *use_table,
                TokenArray *program_arr,
                int debug_flag)
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

      if (!in_text && !in_data)
        continue;
      else
        insert_in_section(curr_sec_tok_arr, "\n");
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

      empty_line = 1;
    }

    else if (str_compare(*(token_ptr + 1), "EQU"))
    {
      if (!str_is_numeric(*(token_ptr + 2)))
      {
        printf("Erro na geração: %s não é número??\n", *(token_ptr + 2));
      }

      insert_synonym(synonym_arr, *(token_ptr), atoi(*(token_ptr + 2)));
      token_ptr += 2;

      // skips next \n
      empty_line = 1;
    }

    else if (str_compare(*token_ptr, "CONST"))
    {
      token_ptr++;
      int val = get_numeric_value(synonym_arr, *token_ptr);
      char *val_str = int_to_str(val);

      if (insert_in_section(curr_sec_tok_arr, val_str) != GENERATION_OK)
      {
        printf("Erro na geração: diretiva %s inválida fora de seção.\n",
               *(token_ptr - 1));
        return GENERATION_FAILED;
      }
    }

    else if (str_compare(*token_ptr, "+"))
    {
      token_ptr++;
      int val = get_numeric_value(synonym_arr, *token_ptr);
      char *val_str = int_to_str(val);

      if (insert_in_section(curr_sec_tok_arr, val_str) != GENERATION_OK)
      {
        printf("Erro na geração: diretiva %s inválida fora de seção.\n",
               *(token_ptr - 1));
        return GENERATION_FAILED;
      }
    }

    else if (str_compare(*token_ptr, "SPACE"))
    {
      int val = get_numeric_value(synonym_arr, *(token_ptr + 1));

      if (val != -1)
      {
        for (int i = 0; i < val; i++)
        {
          if (insert_in_section(curr_sec_tok_arr, "0") != GENERATION_OK)
          {
            printf("Erro na geração: diretiva %s inválida fora de seção.\n",
                   *token_ptr);
            return GENERATION_FAILED;
          }

          // stops at the last "0"
          if (i == val - 1)
            token_ptr++;
        }
      }
      else
      {
        if (insert_in_section(curr_sec_tok_arr, "0") != GENERATION_OK)
        {
          printf("Erro na geração: diretiva inválida fora de seção.\n");
          return GENERATION_FAILED;
        }
      }
    }

    else if (str_compare(*token_ptr, "IF"))
    {
      if (get_numeric_value(synonym_arr, *(token_ptr + 1)) == 0)
        ignore_next_line = 1;

      token_ptr++;

      // skips next \n
      empty_line = 1;
    }

    else if (str_compare(*token_ptr, "PUBLIC"))
    {
      token_ptr++;
      insert_symbol(def_table, *token_ptr);

      // skips next \n
      empty_line = 1;
    }

    else if (str_compare(*(token_ptr + 1), "EXTERN"))
    {
      insert_symbol(use_table, *(token_ptr));
      token_ptr += 2;
    }

    else if (str_compare(*token_ptr, "END"))
    {
      // ignore directive
      empty_line = 1;
      continue;
    }

    else if (str_compare(*token_ptr, "BEGIN"))
    {
      // ignore directive (but starts new line)
      continue;
    }

    // any other token (non-directive, non )
    else
    {
      // printf("%s,", *token_ptr);

      if (!str_is_end_of_line(*token_ptr))
        insert_in_section(curr_sec_tok_arr, *token_ptr);
    }
  }

  for (int i = 0; i < text_arr->back; i++)
    insert_in_section(&program_arr, text_arr->array[i]);

  for (int i = 0; i < data_arr->back; i++)
    insert_in_section(&program_arr, data_arr->array[i]);

  if (debug_flag)
  {
    printf("Representação intermediária do programa:\n");
    for (int i = 0; i < program_arr->back; i++)
    {
      printf("%s ", program_arr->array[i]);
    }
    printf("\n");
  }
}

int first_pass(TokenArray *program_arr,
               InstructionNode *instruction_arr,
               DirectiveNode *directive_arr,
               SymbolArray *symbol_table,
               SymbolArray *def_table)
{
  int token_count = 0,
      position_counter = 0;

  TokenArray *new_program_arr = init_token_array(300);

  for (char **token_ptr = program_arr->array;
       (token_ptr - program_arr->array) < program_arr->back;
       token_ptr++)
  {
    if (str_is_end_of_line(*token_ptr))
    {
      token_count = 0;
      continue;
    }

    token_count++;

    int is_def = token_count == 1 &&
                 !search_instruction_tmp(instruction_arr, *token_ptr) &&
                 !search_directive_tmp(directive_arr, *token_ptr);

    if (!is_def &&
        (str_is_numeric(*token_ptr) || !str_is_numeric(*(token_ptr + 1))))
      position_counter++;

    if (is_def)
    {
      if (search_symbol(symbol_table, *token_ptr))
        printf("Erro doido: símbolo \"%s\" já definido.\n", *token_ptr);
      else
      {
        Symbol *new_symb = init_symbol(*token_ptr, position_counter, 1);
        insert_symbol_ready(symbol_table, new_symb);

        Symbol *def_symb = search_symbol(def_table, *token_ptr);
        if (def_symb)
          update_symbol(def_symb, position_counter, 1);
      }
    }
  }
}

int second_pass(TokenArray **program_arr,
                SymbolArray *sybmol_table,
                SymbolArray *use_table,
                PendencyArray *pendency_arr)
{
  TokenArray *new_program = init_token_array(300);
  int token_count = 0,
      position_count = 0;

  for (char **token_ptr = (*program_arr)->array;
       (token_ptr - (*program_arr)->array) < (*program_arr)->back;
       token_ptr++)
  {
    // substitute symbs with vals
    token_count++;

    if (str_is_end_of_line(*token_ptr))
    {
      token_count = 0;
      continue;
    }

    if (token_count != 1)
    {
      Symbol *label = search_symbol(sybmol_table, *token_ptr);
      if (label)
      {
        int val = label->value;

        if (str_is_numeric(*(token_ptr + 1)))
        {
          val += atoi(*(token_ptr + 1));
          token_ptr++;
        }

        insert_token(new_program, int_to_str(val));
        position_count++;
        continue;
      }
      else
      {
        Symbol *ext_ref = search_symbol(use_table, *token_ptr);
        if (ext_ref)
        {
          insert_pendency(pendency_arr, *token_ptr, position_count - 1);

          insert_token(new_program, "0");
          position_count++;
          continue;
        }
      }
    }

    Symbol *symb = search_symbol(sybmol_table, *token_ptr);
    if (!symb)
      insert_token(new_program, *token_ptr);

    position_count++;
  }

  free(*program_arr);
  *program_arr = new_program;
}

int generate_object(TokenArray **program_arr,
                    SymbolArray *symbol_table,
                    SymbolArray *def_table,
                    SymbolArray *use_table,
                    InstructionNode *instruction_arr)
{
  // 1. new program
  TokenArray *new_program_arr = init_token_array(300);

  for (char **token_ptr = (*program_arr)->array;
       (token_ptr - (*program_arr)->array) < (*program_arr)->back;
       token_ptr++)
  {

    InstructionNode *instr =
        search_instruction_tmp(instruction_arr, *token_ptr);
    if (instr)
      insert_token(new_program_arr, int_to_str(instr->opcode));
    else
      insert_token(new_program_arr, *token_ptr);
  }

  free(*program_arr);
  *program_arr = new_program_arr;

  // 2. header
  // for (int i = 0; i < symbol_table, )
}

int two_pass(TokenArray *token_arr,
             InstructionNode *instruction_arr,
             DirectiveNode *directive_arr,
             SymbolArray *symbol_table,
             char *gen_code,
             int debug_flag)
{
  SynonymArray *synonym_arr = init_synonym_array(20);

  TokenArray *program_arr = init_token_array(300);
  SymbolArray *def_table = init_symbol_array(300);
  SymbolArray *use_table = init_symbol_array(300);

  PendencyArray *pendency_arr = init_pendency_array(300);

  inital_pass(token_arr, synonym_arr, def_table, use_table,
              program_arr, debug_flag);

  first_pass(program_arr, instruction_arr, directive_arr,
             symbol_table, def_table);

  printf("Rótulos declarados:\n");
  for (int i = 0; i < symbol_table->back; i++)
  {
    printf("%s: %d, ",
           symbol_table->array[i]->token, symbol_table->array[i]->value);
  }
  printf("\n\n");

  if (debug_flag)
  {
    printf("Tabela de definições:\n");
    for (int i = 0; i < def_table->back; i++)
      printf("%s: %d, ",
             def_table->array[i]->token, def_table->array[i]->value);

    printf("\n\nTabela de uso:\n");
    for (int i = 0; i < use_table->back; i++)
      printf("%s: %d, ",
             use_table->array[i]->token, use_table->array[i]->value);

    printf("\n\n");
  }

  second_pass(&program_arr, symbol_table, use_table, pendency_arr);

  if (debug_flag)
  {
    printf("Pendências:\n");
    for (int i = 0; i < pendency_arr->back; i++)
      printf("%s: %d, ",
             pendency_arr->array[i]->token, pendency_arr->array[i]->addr);

    printf("\n\nPrograma com referências calculadas:\n");
    for (int i = 0; i < program_arr->back; i++)
      printf("%s ", program_arr->array[i]);
    printf("\n\n");
  }
}
