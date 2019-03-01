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



struct ast_node *make_cmd_forward(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_FORWARD;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}


struct ast_node *make_cmd_right(struct ast_node *expr) {
  struct ast_node *node = calloc(1, sizeof(struct ast_node));
  node->kind = KIND_CMD_SIMPLE;
  node->u.cmd = CMD_RIGHT;
  node->children_count = 1;
  node->children[0] = expr;
  return node;
}



void ast_destroy(struct ast *self) {
	// free(self);
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
	struct ast_node *node = self->unit;
	while(node != NULL){
		if(node->kind==KIND_CMD_SIMPLE){
			switch(node->u.cmd){
				case(CMD_FORWARD):
					ctx->y+=node->children[0]->u.value*cos(ctx->angle);
					ctx->x+=node->children[0]->u.value*sin(ctx->angle);
					printf("LineTo %f %f\n",ctx->x,ctx->y);
					break;
				case(CMD_RIGHT):
					ctx->angle+=(node->children[0]->u.value/180)*PI;
					break;
				default:
					break;
			}
		}
		node=node->next;
	}
}

/*
 * print
 */

char* print_kind(enum ast_kind kind) {
	switch(kind) {
		case KIND_CMD_SIMPLE:
			return "KIND_CMD_SIMPLE";
			break;
		default:
			return "INKNOW STILL";
	}
}

void ast_print(const struct ast *self) {
	// if (self != NULL) {
	// 	printf("[root] -> ");
	// }
	// if (self->unit == NULL) {
	// 	printf("...\n");
	// 	return;
	// }
	// struct ast_node *current_node = self->unit;
	// do {
	// 	printf("[Node: %s] -> ", print_kind(current_node->kind));
	// 	current_node = current_node->next;
	// } while (current_node != NULL);
	// printf("...\n");
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
