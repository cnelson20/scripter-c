#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "string_helpers.h"
#include "vars.h"
#include "main.h"

struct scripter_options options;
struct file_contents fc;

int main(int argc, char *argv[]) {
	parse_options(argc, argv);
	
	FILE *fp = fopen(options.script_filename, "r");
	if (fp == NULL) {
		printf("scripter: Unable to open file '%s': %s\n", options.script_filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	read_file_contents(fp);
	
	int i;
	if (!interactive_mode) for (i = 0; i < fc.num_lines; i++) {
		if (fc.lines[i][0] == '#') {
			parse_line_number_label(i);
		}
	}
	
	for (i = 0; i < fc.num_lines; i++) {
		switch (fc.num_lines[i][0]) {
			case '$':
				define_variable(i);
			case '#':
				if (interactive_mode) parse_line_number_label(i);
				break;
			case 'x':
				if (interactive_mode) exit(EXIT_SUCCESS);
				// Otherwise, print the default error
			default:
				printf("line must start with one of $, -, @, ?, %, >, or #\n");
				if (!interactive_mode) exit(EXIT_FAILURE);
				break;
		}
	}
	
	fclose(fp);
	
	return 0;
}

#define LINE_BUFF_SIZE 4096

void read_file_contents(FILE *fp) {
	char line_buff[LINE_BUFF_SIZE];
	int eof_reached = 0;
	
	fc.num_lines = 0;
	fc.lines_size = 8;
	fc.lines = malloc(sizeof(char *) * fc.lines_size);
	
	while (!eof_reached) {
		if (fgets(line_buff, LINE_BUFF_SIZE, fp) == NULL) {
			eof_reached = 1;
			break;
		}
		trim_newline_char(line_buff);
		int strlen_line = strlen(line_buff);
		if (fc.num_lines >= fc.lines_size) { // Realloc lines array if full
			fc.lines_size *= 2;
			char **new_ptr = realloc(fc.lines, sizeof(char *) * fc.lines_size);
			if (new_ptr == NULL) {
				print_mem_error(EXIT_FAILURE);
			}
			fc.lines = new_ptr;
		}
		if (strlen_line == 0) {
			fc.lines[fc.num_lines] = "";
		} else {
			char *new_str = find_non_whitespace_char(line_buff);			
			char *end_ptr = strchr_not_quoted(new_str, ';');
			if (end_ptr) { *end_ptr = '\0'; }
			end_ptr = find_non_whitespace_char_rev(new_str);
			if (*end_ptr) {
				*(end_ptr + 1) = '\0';
			}
			
			fc.lines[fc.num_lines] = strdup(new_str);
			if (fc.lines[fc.num_lines] == NULL) {
				print_mem_error(EXIT_FAILURE);
			}
		}
		++fc.num_lines;
	}
	
	
}

void define_variable(int line_num) {
	struct variable v;
	char *s;
	
	s = fc.lines[line_num];
}

void parse_line_number_label(int line_num) {
	struct variable v;
	char *s;
	
	s = fc.lines[line_num];	
	if (*find_whitespace_char(s)) {
		printf("scripter: '#' must be followed by label name, instead got '%s'\n", s);
		exit(EXIT_FAILURE);
	}
	
	v.type = TYPE_INT;
	v.data.i = line_num;
	set_label_value(s, &v);
}

void parse_options(int argc, char *argv[]) {
	int single_quote_arg_provided = 0;
	
	// Initialize options
	options.interactive_mode = 0;
	options.echo_commands = 0;
	options.script_filename = NULL;
	
	while (--argc) {
		char *next_option = *(++argv);
		if (!single_quote_arg_provided && next_option[0] == '-') { 
			// Option flags
			switch(next_option[1]) {
				case '\0': // if argument - is provided, treat all future args as files even if they start with '-'
					single_quote_arg_provided = 1;
					break;
				case 'e':
					options.echo_commands = 1;
					break;
				case 'i':
					options.interactive_mode = 1;
					break;
				case 'h':
					print_usage(EXIT_SUCCESS); // no return so doesn't need a break
				default:
					printf("scripter: Invalid option '%s'\n", next_option);
					print_help_message();
					exit(EXIT_FAILURE);
			}
		} else if (strlen_not_zero(next_option)) {
			if (options.script_filename != NULL) {
				printf("scripter: Invalid argument '%s': script file already provided\n", next_option);
				print_help_message();
				exit(EXIT_FAILURE);
			}
			options.script_filename = next_option;
		} // Ignore option if strlen() == 0
	}
	
	if (!options.interactive_mode && options.script_filename == NULL) {
		printf("scripter: No script file provided\n");
		print_help_message();
		exit(EXIT_FAILURE);
	}
}

void print_help_message() {
	printf("Try 'scripter -h' for more information.\n");
}

void print_mem_error(int return_code) {
	printf("scripter: Error occured when allocating mem\n");
	exit(return_code);
}

void print_usage(int return_code) {
	printf("Usage: scripter [options] file...\n");
	printf("Options:\n");
	printf("\t-e: Echo commands as they are executed\n");
	printf("\t-i: Run scripter in interactive mode\n");
	printf("\t-h: Print this information\n");
	
	exit(return_code);
}