#include <stdio.h>

int is_ignore(char chr);
int is_alpha(char chr);
int is_underscore(char chr);
int is_digit(char chr);
int is_valid(char chr);

char *parse_line(char *line);
int parse_file(char *i_filename, char *o_filename);
