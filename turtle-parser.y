%{
#include <stdio.h>

#include "turtle-ast.h"

int yylex();
void yyerror(struct ast *ret, const char *);

%}

%debug
%defines

%define parse.error verbose

%parse-param { struct ast *ret }

%union {
  double value;
  const char *name;
  struct ast_node *node;
}

%token <value>    VALUE       "value"
%token <name>     NAME        "name"

%token            KW_FORWARD  "forward"
%token            KW_RIGHT  "right"
%token		  QUIT	"quit"
/* TODO: add other tokens */

%type <node> unit cmds cmd expr

%%

unit:
    cmds              { $$ = $1; ret->unit = $$; }
;

cmds:
    cmd cmds          { $1->next = $2; $$ = $1; }
  | /* empty */       { $$ = NULL; }
;

cmd:
    KW_FORWARD expr { $$ = make_cmd_forward($2); insert_node(ret, $$) ;}
    |KW_RIGHT expr { $$ = make_cmd_right($2);
    insert_node(ret, $$) ;}
    |QUIT { fprintf(stderr, "byebye"); }
;

expr:
    VALUE             { $$ = make_expr_value($1); }
    /* TODO: add identifier */
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
