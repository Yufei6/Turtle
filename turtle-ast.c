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

struct ast_node *make_expr_name(const char* name) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_NAME;
	node->u.name = name;
	node->children_count = 0;
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

struct ast_node *make_cmd_backward(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_BACKWARD;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}


struct ast_node *make_cmd_position(struct ast_node *x, struct ast_node *y){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_POSITION;
	node->children_count = 2;
	node->children[0] = x;
	node->children[1] = y;
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

struct ast_node *make_cmd_left(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_LEFT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

struct ast_node *make_cmd_heading(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_HEADING;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

struct ast_node *make_cmd_up() {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_UP;
	node->children_count = 0;
	return node;
}

struct ast_node *make_cmd_down() {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_DOWN;
	node->children_count = 0;
	return node;
}

struct ast_node *make_cmd_print(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_PRINT;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}


struct ast_node *make_cmd_color(struct ast_node *expr) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_COLOR;
	node->children_count = 1;
	node->children[0] = expr;
	return node;
}

struct ast_node *make_cmd_repeat(struct ast_node *number, struct ast_node *cmd){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_REPEAT;
	node->children_count = 2;
	node->children[0]=number;
	node->children[1]=cmd;
	return node;
}



void ast_destroy(struct ast *self) {
	// while(self->unit!=NULL){
	// 	struct ast_node *temp = self->unit->next;
	// 	free(self->unit);
	// 	self->unit = temp;
	// }
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
	int nb=0;

	while(node != NULL){
		switch(node->kind){
			case KIND_CMD_SIMPLE:
				ast_cmd_eval(node,ctx);
			break;

			case KIND_CMD_REPEAT:
				nb = floor(node->children[0]->u.value);
				for(int i =0; i<nb; i++){
					ast_cmd_eval(node->children[1],ctx);
				}
			break;

			default:
			break;

		}
		node=node->next;
	}
}

void ast_cmd_eval(struct ast_node *node, struct context *ctx){
	char str1[100];
	char *ptr;
	switch(node->u.cmd){
		case(CMD_FORWARD):
			ctx->x += node->children[0]->u.value*sin(ctx->angle);
			ctx->y -= node->children[0]->u.value*cos(ctx->angle);
			printf("LineTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_BACKWARD):
			ctx->x -= node->children[0]->u.value*sin(ctx->angle);
			ctx->y += node->children[0]->u.value*cos(ctx->angle);
			printf("LineTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_POSITION):
			ctx->x = node->children[0]->u.value;
			ctx->y = node->children[1]->u.value;
			printf("MoveTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_RIGHT):
			ctx->angle += (node->children[0]->u.value/180)*PI;
			break;

		case(CMD_LEFT):
			ctx->angle -= (node->children[0]->u.value/180)*PI;
			break;

		case(CMD_HEADING):
			ctx->angle = (node->children[0]->u.value/180)*PI;
			break;

		case(CMD_UP):
			ctx->up = true;
			break;

		case(CMD_DOWN):
			ctx->up = false;
			break;

		case(CMD_PRINT):
			break;

		case(CMD_COLOR):
			// printf("COLOR TEST %s\n", node->children[0]->u.name);
			strcpy(str1, node->children[0]->u.name);
			ptr = strtok(str1, "\n");
			// printf("TEST COLOR %s\n", str1);
			if((strcmp(ptr,"red")==0)){
				printf("Color 1.000000 0.000000 0.000000\n");
			}
			else if(strcmp(ptr,"green")==0){
				printf("Color 0.000000 1.000000 0.000000\n");
			}
			else if(strcmp(ptr,"blue")==0){
				printf("Color 0.000000 0.000000 1.000000\n");
			}
			else if(strcmp(ptr,"black")==0){
				printf("Color 0.000000 0.000000 0.000000\n");
			}
			else if(strcmp(ptr,"gray")==0){
				printf("Color 0.500000 0.500000 0.500000\n");
			}
			else if(strcmp(ptr,"cyan")==0){
				printf("Color 0.000000 1.000000 1.000000\n");
			}
			else if(strcmp(ptr,"yellow")==0){
				printf("Color 1.000000 0.000000 1.000000\n");
			}
			else if(strcmp(ptr,"magenta")==0){
				printf("Color 1.000000 1.000000 0.000000\n");
			}
			break;

		default:
		break;
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

char* print_kind_cmd(enum ast_cmd kind_cmd){
	switch(kind_cmd){
		case CMD_UP:
			return "CMD_UP";
			break;
		case CMD_DOWN:
			return "CMD_DOWN";
			break;
		case CMD_RIGHT:
			return "CMD_RIGHT";
			break;
		case CMD_LEFT:
			return "CMD_LEFT";
			break;
		case CMD_HEADING:
			return "CMD_HEADING";
			break;
		case CMD_FORWARD:
			return "CMD_FORWARD";
			break;
		case CMD_BACKWARD:
			return "CMD_BACKWARD";
			break;
		case CMD_POSITION:
			return "CMD_POSITION";
			break;
		case CMD_HOME:
			return "CMD_HOME";
			break;
		case CMD_COLOR:
			return "CMD_COLOR";
			break;
		case CMD_PRINT:
			return "CMD_PRINT";
			break;
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
		printf("[Node: %s & %s] -> ", print_kind(current_node->kind), print_kind_cmd(current_node->u.cmd));
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
