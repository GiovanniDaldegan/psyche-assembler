// char categories
int char_is_ignored(char chr);
int char_is_alpha_regular(char chr);
int char_is_underscore(char chr);
int char_is_digit(char chr);
static inline int char_is_valid_special(char chr);
int char_is_valid(char chr);

// char & string operations
char capitalize_char(char chr);
void capitalize_str(char *str);

char *init_string(char *str);

char *trim_l(char *str);
char *trim_r(char *str);
char *str_trim(char *str);

int str_compare(char *str1, char *str2);
char *get_filename_extention(char *filename);

void print_string_array(char **arr, int n_elements);