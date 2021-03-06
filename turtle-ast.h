#ifndef TURTLE_AST_H
#define TURTLE_AST_H
#include <stddef.h>
#include <stdbool.h>

// simple commands
enum ast_cmd {
  CMD_UP,
  CMD_DOWN,
  CMD_RIGHT,
  CMD_LEFT,
  CMD_HEADING,
  CMD_FORWARD,
  CMD_BACKWARD,
  CMD_POSITION,
  CMD_HOME,
  CMD_COLOR,
  CMD_PRINT,
};

// internal functions
enum ast_func {
  FUNC_COS,
  FUNC_RANDOM,
  FUNC_SIN,
  FUNC_SQRT,
  FUNC_TAN,
};

// kind of a node in the abstract syntax tree
enum ast_kind {
  KIND_CMD_SIMPLE,
  KIND_CMD_REPEAT,
  KIND_CMD_BLOCK,
  KIND_CMD_PROC,
  KIND_CMD_CALL,
  KIND_CMD_SET,

  KIND_EXPR_FUNC,
  KIND_EXPR_VALUE,
  KIND_EXPR_BINOP,
  KIND_EXPR_BLOCK,
  KIND_EXPR_NAME,
};

#define AST_CHILDREN_MAX 3

// a node in the abstract syntax tree
struct ast_node {
  enum ast_kind kind; // kind of the node

  union {
    enum ast_cmd cmd;   // kind == KIND_CMD_SIMPLE
    double value;       // kind == KIND_EXPR_VALUE, for literals
    char op;            // kind == KIND_EXPR_BINOP, for operators in expressions
    const char *name;   // kind == KIND_EXPR_NAME, the name of procedures and variables
    enum ast_func func; // kind == KIND_EXPR_FUNC, a function
  } u;

  size_t children_count;  // the number of children of the node
  struct ast_node *children[AST_CHILDREN_MAX];  // the children of the node (arguments of commands, etc)
  struct ast_node *next;  // the next node in the sequence
};


// TODO: make some constructors to use in parser.y
// for example:
struct ast_node *make_expr_value(double value);
struct ast_node *make_expr_name(const char *name);
struct ast_node *make_expr_operateur(const char op, struct ast_node *left, struct ast_node *right);
struct ast_node *make_expr_operateur_oppose(struct ast_node *expr);
struct ast_node *make_cmd_forward(struct ast_node *expr);
struct ast_node *make_cmd_backward(struct ast_node *expr);
struct ast_node *make_cmd_position(struct ast_node *x, struct ast_node *y);
struct ast_node *make_cmd_right(struct ast_node *expr);
struct ast_node *make_cmd_left(struct ast_node *expr);
struct ast_node *make_cmd_heading(struct ast_node *expr);
struct ast_node *make_cmd_up();
struct ast_node *make_cmd_down();
struct ast_node *make_cmd_print(struct ast_node *expr);
struct ast_node *make_cmd_color(struct ast_node *expr1,struct ast_node *expr2,struct ast_node *expr3);
struct ast_node *make_cmd_repeat(struct ast_node *number, struct ast_node *cmd);
struct ast_node *make_cmd_block(struct ast_node *cmds_in_block);
struct ast_node *make_cmd_set(struct ast_node *name, struct ast_node *expr);
struct ast_node *make_expr_func_sin(struct ast_node *expr);
struct ast_node *make_expr_func_cos(struct ast_node *expr);
struct ast_node *make_expr_func_tan(struct ast_node *expr);
struct ast_node *make_expr_func_sqrt(struct ast_node *expr);
struct ast_node *make_expr_func_random(struct ast_node *min, struct ast_node *max);




// root of the abstract syntax tree
struct ast {
  struct ast_node *unit;
};

void insert_node(struct ast *root, struct ast_node *new_node);

// do not forget to destroy properly! no leaks allowed!
void ast_destroy(struct ast *self);

// the execution context
struct context {
  double x;
  double y;
  double angle;
  bool up;

  // TODO: add procedure handling
  // TODO: add variable handling
};

// create an initial context
void context_create(struct context *self);

// print the tree as if it was a Turtle program
void ast_print(const struct ast *self);

// evaluate the tree and generate some basic primitives
void ast_eval(const struct ast *self, struct context *ctx);
void ast_cmd_eval(struct ast_node *node, struct context *ctx);
void ast_binop_eval(struct ast_node *node);


#endif /* TURTLE_AST_H */
