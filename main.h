#ifndef _MAIN_H
#define _MAIN_H

struct scripter_options {
	char *script_filename;
	int echo_commands;
	int interactive_mode;
};

struct file_contents {
	char **lines;
	int lines_size;
	int num_lines;
};

int main(int argc, char *argv[]);

void parse_line_number_label(int);

void parse_options(int argc, char *argv[]);

void read_file_contents(FILE *fp);

void print_help_message(void);
void print_mem_error(int);
void print_usage(int);

#endif