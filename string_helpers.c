#include <ctype.h>
#include <string.h>
#include <stdio.h>

char *strchr_not_quoted(char *str, int c) {
	int quoted = 0;
	
	while (*str) {
		if (quoted) {
			if (*str == quoted) { quoted = 0; }
		} else {
			if (*str == c) {
				return str;
			} else if (*str == '"' || *str == 0x27) {
				quoted = *str;
			}
		}
		++str;
	}
	return NULL;
}

char *find_non_whitespace_char_rev(char *str) {
	char *rev = str + strlen(str) - 1;
	while (rev > str && isspace(*rev)) {
		--rev;
	}
	return rev;
	
}

char *find_non_whitespace_char(char *str) {
	while (*str && isspace(*str)) {
		++str;
	}
	return str;
}

char *find_whitespace_char(char *str) {
	while (*str && !isspace(*str)) {
		++str;
	}
	return str;
}

int strlen_not_zero(char *str) {
	return str[0];
}

void trim_newline_char(char *str) {
	char *c = strrchr(str, '\n');
	if (c == NULL) return;
	*c = '\0';
	if (c > str && *(c - 1) == '\r') {
		*(c - 1) = '\0';
	}
}