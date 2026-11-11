#include <stdio.h>
#include "utils.c"
#include "hash_table.c"
#include "token_array.c"
#include "symbol_array.c"
#include "scanner.c"
#include "parser.c"
#include "generator.c"

// argv: [0] source, [1] target obj
int main(int argc, char *argv[])
{
  if (argc == 2 && str_compare(argv[1], "-h"))
  {
    printf("usage: montador <código-fonte>.asm [-d]\n\n"
           "Arquivo fonte deve ser de tipo .asm.\nProduz arquivo de texto de "
           "tipo .obj com código montado.\n"
           "Flag \"-d\" ativa modo de debug.\n");
    return 0;
  }
  else if (argc < 2 || argc > 3)
  {
    printf("ERRO: argumentos inválidos. Execute 'montador -h' para saber mais "
           "sobre o uso.\n");
    return 1;
  }

  char *ext = get_filename_extention(argv[1]);
  if (!str_compare(ext, ".asm"))
  {
    printf("ERRO: tipo de arquivo inválido!\n");
    return 1;
  }

  int debug_flag = 0;
  if (argc == 3 && str_compare(argv[2], "-d"))
    debug_flag = 1;

  int err = 0;

  TokenArray *token_arr = init_token_array(300);
  SymbolArray *symbol_arr = init_symbol_array(300);

  // init instr, direct
  InstructionNode instruction_array[] = {
      {"", "ADD", 1, 2, 1, (void *)NULL},
      {"", "SUB", 2, 2, 1, (void *)NULL},
      {"", "MULT", 3, 2, 1, (void *)NULL},
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
      {"", "EOA", 0, 0, 0, (void *)NULL},
  };

  DirectiveNode directive_array[] = {
      {"", "SPACE", 1, (void *)NULL},
      {"", "CONST", 1, (void *)NULL},
      {"", "EQU", 1, (void *)NULL},
      {"", "IF", 1, (void *)NULL},
      {"", "ORG", 1, (void *)NULL},
      {"", "MACRO", -1, (void *)NULL},
      {"", "ENDMACRO", 0, (void *)NULL},
      {"", "BEGIN", 0, (void *)NULL},
      {"", "END", 0, (void *)NULL},
      {"", "PUBLIC", 1, (void *)NULL},
      {"", "EXTERN", 0, (void *)NULL},
      {"", "+", 1, (void *)NULL},
      {"", "SECTION", 1, (void *)NULL},
      {"", "EOA", 0, (void *)NULL},
  };

  if (debug_flag)
    printf("=======================\n"
           ">> ETAPA DE SCANNING <<\n"
           "=======================\n\n");

  err = scan_file(argv[1], token_arr, symbol_arr, instruction_array, directive_array);
  
  if (err != 0)
  {
    printf("\nFalha no scanning. A montagem foi abortada.\n");
    return 1;
  }
  
  // debug scanning
  if (debug_flag)
  {
    printf("Tokens válidos detectados pelo scanner:\n");
    print_string_array(token_arr->array, token_arr->back);
    printf("\n\n");

    printf("Símbolos registrados pelo scanner:\n");
    for (int i = 0; i < symbol_arr->back; i++)
      printf("%s, ", symbol_arr->array[i]->token);

    printf("\n\nFim do scanning. Pressione Enter.");
    getchar();
    printf("\n");

    printf("======================\n"
           ">> ETAPA DE PARSING <<\n"
           "======================\n\n");
  }

  err = parse_code(token_arr, symbol_arr, (InstructionNode *)instruction_array, (DirectiveNode *)directive_array, debug_flag);

  if (err != 0)
  {
    printf("\nParsing falhou. A montagem foi abortada.\n");
    return 1;
  }

  if (debug_flag)
  {
    printf("\n\nFim do parsing. Pressione Enter.");
    getchar();
    printf("\n\n");
  }

  return 0;
}
