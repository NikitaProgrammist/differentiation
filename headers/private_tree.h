#ifndef PRIVATE_TREE_H
#define PRIVATE_TREE_H

struct Node_t {
  Type type;
  Data data;
  Node_t * left;
  Node_t * right;
  Node_t * parent;
};

struct Tree {
  Node_t * root;
  char * vars[10];
  size_t len;
};

struct Operations {
  const char * name;
  Data data;
  size_t len;
};

inline Operations operations[30] = {{.name = "sin", .data = {.op = SIN}, .len = 3},
                                    {.name = "cos", .data = {.op = COS}, .len = 3},
                                    {.name = "tan", .data = {.op = TAN}, .len = 3},
                                    {.name = "ctg", .data = {.op = COT}, .len = 3},
                                    {.name = "arcsin", .data = {.op = ASIN}, .len = 6},
                                    {.name = "arccos", .data = {.op = ACOS}, .len = 6},
                                    {.name = "arctg", .data = {.op = ATAN}, .len = 5},
                                    {.name = "arcctg", .data = {.op = ACOT}, .len = 6},
                                    {.name = "sh", .data = {.op = SINH}, .len = 2},
                                    {.name = "ch", .data = {.op = COSH}, .len = 2},
                                    {.name = "th", .data = {.op = TANH}, .len = 2},
                                    {.name = "cth", .data = {.op = COTH}, .len = 3},
                                    {.name = "arsh", .data = {.op = ASINH}, .len = 4},
                                    {.name = "arch", .data = {.op = ACOSH}, .len = 4},
                                    {.name = "arth", .data = {.op = ATANH}, .len = 4},
                                    {.name = "arcth", .data = {.op = ACOTH}, .len = 5},
                                    {.name = "ln", .data = {.op = LN}, .len = 2},
                                    {.name = "+", .data = {.op = PLUS}, .len = 1},
                                    {.name = "-", .data = {.op = MINUS}, .len = 1},
                                    {.name = "log", .data = {.op = LOG}, .len = 3},
                                    {.name = "+", .data = {.op = ADD}, .len = 1},
                                    {.name = "-", .data = {.op = SUB}, .len = 1},
                                    {.name = "*", .data = {.op = MUL}, .len = 1},
                                    {.name = "/", .data = {.op = DIV}, .len = 1},
                                    {.name = "^", .data = {.op = POW}, .len = 1},
                                    {.name = "zero", .data = {.val = {0}}, .len = 4},
                                    {.name = "one", .data = {.val = {1}}, .len = 3},
                                    {.name = "two", .data = {.val = {2}}, .len = 3},
                                    {.name = "half", .data = {.val = {1.0 / 2.0}}, .len = 4},
                                    {.name = "nan", .data = {.val = {0.0 / 0.0}}, .len = 3}};

#define ZERO 25
#define ONE 26
#define TWO 27
#define HALF 28
#define NAN_VALUE 29

#define DL differen(node->left, ind)
#define DR differen(node->right, ind)

#define OP OPERATION
#define VAR VARIABLE
#define VAL VALUE
#define D(arg) operations[arg].data

#define CL copy(node->left)
#define CR copy(node->right)
#define ZeroNode createNode(VAL, D(ZERO), NULL, NULL)
#define OneNode createNode(VAL, D(ONE), NULL, NULL)
#define TwoNode createNode(VAL, D(TWO), NULL, NULL)
#define HalfNode createNode(VAL, D(HALF), NULL, NULL)
#define NanNode createNode(VAL, D(NAN_VALUE), NULL, NULL)
#define SIN_(arg1, arg2) createNode(OP, D(SIN), arg1, arg2)
#define COS_(arg1, arg2) createNode(OP, D(COS), arg1, arg2)
#define TAN_(arg1, arg2) createNode(OP, D(TAN), arg1, arg2)
#define COT_(arg1, arg2) createNode(OP, D(COT), arg1, arg2)
#define ASIN_(arg1, arg2) createNode(OP, D(ASIN), arg1, arg2)
#define ACOS_(arg1, arg2) createNode(OP, D(ACOS), arg1, arg2)
#define ATAN_(arg1, arg2) createNode(OP, D(ATAN), arg1, arg2)
#define ACOT_(arg1, arg2) createNode(OP, D(ACOT), arg1, arg2)
#define SINH_(arg1, arg2) createNode(OP, D(SINH), arg1, arg2)
#define COSH_(arg1, arg2) createNode(OP, D(COSH), arg1, arg2)
#define TANH_(arg1, arg2) createNode(OP, D(TANH), arg1, arg2)
#define COTH_(arg1, arg2) createNode(OP, D(COTH), arg1, arg2)
#define ASINH_(arg1, arg2) createNode(OP, D(ASINH), arg1, arg2)
#define ACOSH_(arg1, arg2) createNode(OP, D(ACOSH), arg1, arg2)
#define ATANH_(arg1, arg2) createNode(OP, D(ATANH), arg1, arg2)
#define ACOTH_(arg1, arg2) createNode(OP, D(ACOTH), arg1, arg2)
#define LN_(arg1, arg2) createNode(OP, D(LN), arg1, arg2)
#define LOG_(arg1, arg2) createNode(OP, D(LOG), arg1, arg2)
#define ADD_(arg1, arg2) createNode(OP, D(ADD), arg1, arg2)
#define SUB_(arg1, arg2) createNode(OP, D(SUB), arg1, arg2)
#define MUL_(arg1, arg2) createNode(OP, D(MUL), arg1, arg2)
#define DIV_(arg1, arg2) createNode(OP, D(DIV), arg1, arg2)
#define POW_(arg1, arg2) createNode(OP, D(POW), arg1, arg2)
#define PLUS_(arg1, arg2) createNode(OP, D(PLUS), arg1, arg2)
#define MINUS_(arg1, arg2) createNode(OP, D(MINUS), arg1, arg2)
#define COPY(node) createNode(node->type, node->data, CL, CR)
#define BDIF(node) createNode(node->type, node->data, DL, DR)
#define UDIF(node) createNode(node->type, node->data, DL, NULL)

#endif // PRIVATE_TREE_H
