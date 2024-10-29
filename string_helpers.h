#ifndef _STRING_HELPERS
#define _STRING_HELPERS

char *find_non_whitespace_char(char *str);
char *find_non_whitespace_char_rev(char *str);
char *find_whitespace_char(char *str);

char *strchr_not_quoted(char *str, int c);

int strlen_not_zero(char *str);
void trim_newline_char(char *str);

#endif