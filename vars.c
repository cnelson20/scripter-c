#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vars.h"

struct variable_node *root = NULL;

int add_node_tree(struct variable_node *n) {
	if (root == NULL) {
		root = n;
		return 0;
	}
	struct variable_node *curr = root;
	while (curr) {
		int strcmp_res = strcmp(n->name, curr->name);
		
		if (!strcmp_res) {
			free_variable(curr->value);
			curr->value = n->value;
			n->value = NULL;
			free_variable_node(n);
			return 1;
		} else if (strcmp_res > 0) { // n->name > curr->name
			if (!curr->right) {
				curr->right = n;
				return 0;
			}
			curr = curr->right;
		} else {
			if (!curr->left) {
				curr->left = n;
				return 0;
			}
			curr = curr->left;
		}
	}
}

void free_variable(struct variable *v) {
	if (v->type == TYPE_STR) {
		free(v->data.s);
	}
	free(v);
}

void free_variable_node(struct variable_node *n) {
	free_variable(n->value);
	free(n->name);
	free(n);
}


struct variable *get_label_value(char *label) {
	struct variable_node *curr = root;
	while (curr) {
		int strcmp_res = strcmp(label, curr->name);
		if (!strcmp_res) {
			return curr->value;
		} else if (strcmp_res > 0) {
			curr = curr->right;
		} else {
			curr = curr->left;
		}
	}
	return NULL; // not found
}

struct variable *dup_variable(struct variable *p) {
	struct variable *new = malloc(sizeof(struct variable));
	new->type = p->type;
	if (p->type == TYPE_INT) {
		new->data.i = p->data.i;
	} else { // TYPE_STR
		new->data.s = strdup(p->data.s);
	}
	return new;
}

int set_label_value(char *label, struct variable *v) {
	struct variable_node *new = malloc(sizeof(struct variable_node));
	new->left = NULL;
	new->right = NULL;
	new->name = strdup(label);
	new->value = dup_variable(v);
	
	int already_defined = add_node_tree(new);
	
	return 0;
}