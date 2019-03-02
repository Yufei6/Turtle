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

%token      KW_FORWARD  "forward"
%token      KW_BACKWARD  "backward"
%token      KW_POSITION  "position"
%token      KW_RIGHT  "right"
%token      KW_LEFT  "left"
%token      KW_HEADING  "heading"
%token      KW_UP  "up"
%token      KW_DOWN  "down"
%token      KW_PRINT  "print"
%token      KW_COLOR  "color"
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
    KW_FORWARD expr { $$ = make_cmd_forward($2); insert_node(ret, $$); }
    |KW_BACKWARD expr { $$ = make_cmd_backward($2); insert_node(ret, $$); }
    |KW_POSITION expr expr { $$ = make_cmd_position($2,$3); insert_node(ret, $$); }
    |KW_RIGHT expr { $$ = make_cmd_right($2); insert_node(ret, $$); }
    |KW_LEFT expr { $$ = make_cmd_left($2); insert_node(ret, $$); }
    |KW_HEADING expr { $$ = make_cmd_heading($2); insert_node(ret, $$); }
    |KW_UP {$$ = make_cmd_up(); insert_node(ret, $$);}
    |KW_DOWN {$$ = make_cmd_down(); insert_node(ret, $$);}
    |KW_PRINT expr{$$ = make_cmd_print($2); insert_node(ret,$$);}
    |KW_COLOR expr{$$ = make_cmd_color($2); insert_node(ret,$$);}
    |QUIT { fprintf(stderr, "byebye"); }
;

expr:
    VALUE             { $$ = make_expr_value($1); }
    |NAME              {printf("HEY NAME:%s\n",$1); $$ = make_expr_name($1); }
;

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
