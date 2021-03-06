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
%token <name>   NAME      "name"
%token <name>   OPERATEUR     "op"


%token      RED     
%token      GREEN 
%token      BLUE 
%token      BLACK 
%token      GRAY 
%token      CYAN 
%token      YELLOW 
%token      MAGENTA 

%token      KW_SET        "set"
%token      KW_FORWARD    "forward"
%token      KW_BACKWARD   "backward"
%token      KW_POSITION   "position"
%token      KW_RIGHT      "right"
%token      KW_LEFT       "left"
%token      KW_HEADING    "heading"
%token      KW_UP         "up"
%token      KW_DOWN       "down"
%token      KW_PRINT      "print"
%token      KW_COLOR      "color"
%token      KW_REPEAT     "repeat"
%token      LP            "lp"
%token      RP            "rp"
%token      SIN           "sin"
%token      COS           "cos"
%token      TAN           "tan"
%token      SQRT          "sqrt"
%token      RANDOM        "random"
%token      LL            "ll"
%token      RR            "rr"
%token      COMMA         "comma"
%token      ADD           "add"
%token      SUB           "sub"
%token      DIV           "div"
%token      MUL           "mul"
%token      PUI           "pui"
%token      OPPOSE        "oppose"
%token		  QUIT	        "quit"
/* TODO: add other tokens */

%left OPPOSE
%left ADD SUB
%left MUL DIV
%left PUI
%left SIN COS TAN SQRT RANDOM


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
    |KW_SET expr expr{$$ = make_cmd_set($2,$3); insert_node(ret,$$);}
    |KW_COLOR RED{$$ = make_cmd_color(make_expr_value(1),make_expr_value(0),make_expr_value(0)); insert_node(ret,$$);}
    |KW_COLOR GREEN{$$ = make_cmd_color(make_expr_value(0),make_expr_value(1),make_expr_value(0)); insert_node(ret,$$);}
    |KW_COLOR BLUE{$$ = make_cmd_color(make_expr_value(0),make_expr_value(0),make_expr_value(1)); insert_node(ret,$$);}
    |KW_COLOR BLACK{$$ = make_cmd_color(make_expr_value(0),make_expr_value(0),make_expr_value(0)); insert_node(ret,$$);}
    |KW_COLOR GRAY{$$ = make_cmd_color(make_expr_value(0.5),make_expr_value(0.5),make_expr_value(0.5)); insert_node(ret,$$);}
    |KW_COLOR CYAN{$$ = make_cmd_color(make_expr_value(0),make_expr_value(1),make_expr_value(1)); insert_node(ret,$$);}
    |KW_COLOR YELLOW{$$ = make_cmd_color(make_expr_value(1),make_expr_value(0),make_expr_value(1)); insert_node(ret,$$);}
    |KW_COLOR MAGENTA{$$ = make_cmd_color(make_expr_value(1),make_expr_value(1),make_expr_value(0)); insert_node(ret,$$);}

    |KW_REPEAT expr cmd{$$ = make_cmd_repeat($2,$3); insert_node(ret,$$);}
    |LP cmds RP{$$ = make_cmd_block($2); insert_node(ret,$$);}
    |QUIT { fprintf(stderr, "byebye"); }
    |NAME { $$ = make_expr_name($1); }
;

expr:
    VALUE             { $$ = make_expr_value($1); }
  | '(' expr ')'        { $$ = $2; }
  | NAME              { $$ = make_expr_name($1); }
  | OPPOSE expr      { $$ = make_expr_operateur_oppose($2); }
  | expr ADD expr { $$ = make_expr_operateur('+',$1,$3); }
  | expr SUB expr { $$ = make_expr_operateur('-',$1,$3); }
  | expr MUL expr { $$ = make_expr_operateur('*',$1,$3); }
  | expr DIV expr { $$ = make_expr_operateur('/',$1,$3); }
  | expr PUI expr { $$ = make_expr_operateur('^',$1,$3); }
  | SIN expr      { $$ = make_expr_func_sin($2); }
  | COS expr      { $$ = make_expr_func_cos($2); }
  | TAN expr      { $$ = make_expr_func_tan($2); }
  | SQRT expr     { $$ = make_expr_func_sqrt($2); }
  | RANDOM '(' expr COMMA expr ')'   { $$ = make_expr_func_random($3,$5); }

  
;


  

%%

void yyerror(struct ast *ret, const char *msg) {
  (void) ret;
  fprintf(stderr, "%s\n", msg);
}
