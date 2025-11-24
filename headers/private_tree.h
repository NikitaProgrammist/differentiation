#ifndef PRIVATE_TREE_H
#define PRIVATE_TREE_H

enum Type {
  OPERATION,
  VARIABLE,
  VALUE
};

enum TypeOp {
  SIN,
  COS,
  TAN,
  COT,
  ASIN,
  ACOS,
  ATAN,
  ACOT,
  SINH,
  COSH,
  TANH,
  COTH,
  ASINH,
  ACOSH,
  ATANH,
  ACOTH,
  LN,
  LOG,
  ADD,
  SUB,
  MUL,
  DIV,
  POW,
  PLUS,
  MINUS
};

struct TypeVar {
  int ind;
};

struct TypeVal {
  TreeElem_t val;
};

union Data {
  TypeOp op;
  TypeVar var;
  TypeVal val;
};

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

inline const char * operations[25] = {"sin", "cos", "tg", "ctg",
                                      "arcsin", "arccos", "arctg", "arcctg",
                                      "sh", "ch", "th", "cth", "arsh", "arch",
                                      "arth", "arcth", "ln", "log",
                                      "+", "-", "*", "/", "^", "+", "-"};

inline const Data datas[30] = {{.op = SIN}, {.op = COS}, {.op = TAN}, {.op = COT},
                               {.op = ASIN}, {.op = ACOS}, {.op = ATAN}, {.op = ACOT},
                               {.op = SINH}, {.op = COSH}, {.op = TANH}, {.op = COTH},
                               {.op = ASINH}, {.op = ACOSH}, {.op = ATANH}, {.op = ACOTH},
                               {.op = LN}, {.op = LOG}, {.op = ADD}, {.op = SUB}, {.op = MUL},
                               {.op = DIV}, {.op = POW}, {.op = PLUS}, {.op = MINUS},
                               {.val = {0}}, {.val = {1}}, {.val = {2}}, {.val = {1 / 2}}, {.val = {0.0 / 0.0}}};

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
#define D(arg) datas[arg]
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
