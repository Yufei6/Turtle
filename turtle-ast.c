#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.141592653589793

struct ast_node *make_expr_value(double value) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_VALUE;
	node->u.value = value;
	return node;
}


struct ast_node *make_expr_fw(double value){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_FORWARD;
	node->children_count = 1;
	node->children[0] = make_expr_value(value);
	node->next = NULL;
	return node; 
}



void ast_destroy(struct ast *self) {
	//free(self);
}

/*
 * context
 */

void context_create(struct context *self) {
	self = calloc(1, sizeof(struct context));
	self->x = 0.0;
	self->y = 0.0;
	self->angle = 0.0;
	self->up = true;
}

/*
 * eval
 */

void ast_eval(const struct ast *self, struct context *ctx) {
	
}

/*
 * print
 */

char* print_kind(enum ast_kind kind) {
	switch(kind) {
		case KIND_CMD_SIMPLE:
			return "KIND_CMD_SIMPLE";
	}
}

void ast_print(const struct ast *self) {
	if (self != NULL) {
		printf("[root] -> ");
	}
	if (self->unit == NULL) {
		printf("...\n");
		return;
	}
	struct ast_node *current_node = self->unit;
	do {
		printf("[Node: %s] -> ", print_kind(current_node->kind));
		current_node = current_node->next;
	} while (current_node != NULL);
	printf("...\n");
}

void insert_node(struct ast *root, struct ast_node *new_node) {
	if (root == NULL || new_node == NULL) {
		return;
	}
	if (root->unit == NULL) {
		root->unit = new_node;
		return;
	}
	struct ast_node *current_node = root->unit;
	while(current_node->next != NULL) {
		current_node = current_node->next;
	}
	current_node->next = new_node;
}
