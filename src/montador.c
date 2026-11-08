#include <stdio.h>
#include "../src/utils.c"
#include "../src/scanner.c"
#include "../src/parser.c"
#include "../src/generator.c"

// argv: [0] source, [1] target obj
int main(int argc, char *argv[])
{
  if (argc == 2 && str_compare(argv[1], "-h"))
  {
    printf("usage: montador <código-fonte>.asm\n\nCódigo fonte deve ser de tipo"
           ".asm.\nProduz arquivo de texto de tipo .obj com código montado.\n");
    return 0;
  }
  else if (argc != 2)
  {
    printf("ERRO: argumentos inválidos. Use 'montador -h' para saber"
           "mais sobre o uso.\n");
    return 1;
  }

  char *ext = get_filename_extention(argv[1]);
  if (!str_compare(ext, ".asm"))
  {
    printf("ERRO: arquivo com extensão inválida!\n");
    return 1;
  }

  TokenArray *token_arr = init_array_token(100);
  HashTable *symbol_table;
  // init_hash_table_symbol(symbol_table, HASH_TABLE_INCREMENT);

  scan_file(argv[1], token_arr, symbol_table);

  print_string_array(token_arr->array, token_arr->back);

  return 0;
}
