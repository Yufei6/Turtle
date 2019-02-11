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
	// node->next = null;
	return node; 
}



void ast_destroy(struct ast *self) {
	free(self);
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

void ast_print(const struct ast *self) {

}
