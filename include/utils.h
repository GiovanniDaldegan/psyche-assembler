// char categories
int is_ignored(char chr);
int is_alpha_regular(char chr);
int is_underscore(char chr);
int is_digit(char chr);
int is_valid(char chr);

// char & string operations
char capitalize_char(char chr);
char *capitalize_str(char *str);
char *append_string(char *string_arr, char *new_string);
char *remove_char(char *str, char chr);
