#include "turtle-ast.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define PI 3.14159265358979323846
#define SQRT2 1.41421356237309504880
#define SQRT3 1.7320508075688772935

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

struct ast_node *make_expr_operateur(const char op, struct ast_node *left, struct ast_node *right){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_EXPR_BINOP;
	node->u.op = op;
	node->children_count = 2;
	node->children[0]=left;
	node->children[1]=right;
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


struct ast_node *make_cmd_color(struct ast_node *expr1, struct ast_node *expr2, struct ast_node *expr3) {
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SIMPLE;
	node->u.cmd = CMD_COLOR;
	node->children_count = 3;
	node->children[0] = expr1;
	node->children[1] = expr2;
	node->children[2] = expr3;
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


struct ast_node *make_cmd_block(struct ast_node *cmds_in_block){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_BLOCK;
	int nb=0;
	while(cmds_in_block!=NULL){
		node->children[nb]=cmds_in_block;
		nb++;
		cmds_in_block=cmds_in_block->next;
	}
	node->children_count = nb;
	return node;
}

struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node *expr){
	struct ast_node *node = calloc(1, sizeof(struct ast_node));
	node->kind = KIND_CMD_SET;
	node->children_count = 2;
	node->children[0]=name;
	node->children[1]=expr;
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
	int nb=0, nb2=0;

	while(node != NULL){
		switch(node->kind){
			case KIND_CMD_SIMPLE:
				ast_cmd_eval(node,ctx);
			break;

			case KIND_CMD_REPEAT:
				nb = floor(node->children[0]->u.value);
				if(node->children[1]->kind==KIND_CMD_SIMPLE){
					for(int i =0; i<nb; i++){
						ast_cmd_eval(node->children[1],ctx);
					}
				}
				else if(node->children[1]->kind==KIND_CMD_BLOCK){
					nb2 = node->children[1]->children_count;
					for(int i=0; i<nb; i++){
						for(int j=0; j<nb2; j++){
							ast_cmd_eval(node->children[1]->children[j],ctx);
						}
					}
				}
			break;

			case KIND_CMD_BLOCK:
				nb = node->children_count;
				for(int i=0; i<nb; i++){
					ast_cmd_eval(node->children[i],ctx);
				}
			break;

			case KIND_EXPR_BINOP:
				ast_binop_eval(node);
			break;

			case  KIND_EXPR_NAME:
				
			break;

			default:
			break;

		}
		node=node->next;
	}
}

void ast_binop_eval(struct ast_node *node){
	switch(node->u.op){
		case '+':
			node->u.value=node->children[0]->u.value + node->children[1]->u.value;
		break;
		case '-':
			node->u.value=node->children[0]->u.value - node->children[1]->u.value;
		break;
		case '*':
			node->u.value=node->children[0]->u.value * node->children[1]->u.value;
		break;
		case '/':
			node->u.value=node->children[0]->u.value / node->children[1]->u.value;
		break;
		case '^':
			node->u.value=pow(node->children[0]->u.value,node->children[1]->u.value);
		break;
		default:
			printf("I WORK FOR NOTHING\n");
		break;
	}
}

void ast_cmd_eval(struct ast_node *node, struct context *ctx){
	switch(node->u.cmd){
		case(CMD_FORWARD):
			if(node->children[0]->kind==KIND_EXPR_BINOP){
				ast_binop_eval(node->children[0]);
			}
			ctx->x += node->children[0]->u.value*sin(ctx->angle*PI/180);
			ctx->y -= node->children[0]->u.value*cos(ctx->angle*PI/180);
			printf("LineTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_BACKWARD):
			ctx->x -= node->children[0]->u.value*sin(ctx->angle*PI/180);
			ctx->y += node->children[0]->u.value*cos(ctx->angle*PI/180);
			printf("LineTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_POSITION):
			ctx->x = node->children[0]->u.value;
			ctx->y = node->children[1]->u.value;
			printf("MoveTo %f %f\n",ctx->x,ctx->y);
			break;

		case(CMD_RIGHT):
			ctx->angle += (node->children[0]->u.value);
			break;

		case(CMD_LEFT):
			ctx->angle -= (node->children[0]->u.value);
			break;

		case(CMD_HEADING):
			ctx->angle = (node->children[0]->u.value);
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
			printf("Color %f %f %f\n",node->children[0]->u.value,node->children[1]->u.value,node->children[2]->u.value);
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
