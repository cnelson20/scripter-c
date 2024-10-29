#ifndef _VARS_H
#define _VARS_H

struct variable_node {
	struct variable_node *left, *right;
	char *name;
	struct variable *value;
};

#define TYPE_INT 0
#define TYPE_STR 1

struct variable {
	int type;
	union {
		int i;
		char *s;
	} data;
};

void free_variable(struct variable *v);
void free_variable_node(struct variable_node *n);

struct variable *get_label_value(char *label);
int set_label_value(char *label, struct variable *v);

#endif