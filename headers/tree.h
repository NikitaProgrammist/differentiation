#ifndef TREE_H
#define TREE_H

typedef struct Node_t Node_t;
typedef struct Tree Tree;
typedef double TreeElem_t;

enum TreeErr {
  SUCCESS,
  ASSERTED,
  CREATE_FAILED,
  NULL_POINTER,
  DELETE_FAILED,
  ALLOC_FAILED,
  CALLOC_FAILED,
  REALLOC_FAILED,
  CHECK_FAILED,
  CYCLE_ERR,
  EMPTY_TREE,
  FILE_ERR,
  INCORRECT_DATA,
  INCORRECT_ANSWER,
  INCORRECT_EXPR
};

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
  PLUS,
  MINUS,
  LOG,
  ADD,
  SUB,
  MUL,
  DIV,
  POW
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

TreeErr treeInit(Tree ** tree);
void treeDestroy(Tree * tree);
void DestroyNode(Node_t * node, size_t * len);
TreeErr mainTreeCycle(Tree * tree, Tree ** res, char * filename);
TreeErr getTree(Tree ** tree, char * filename);

TreeErr treeGetFirst(Tree * tree, Node_t ** result);
TreeErr treeGetLeft(Tree * tree, Node_t * elem, Node_t ** result);
TreeErr treeGetRight(Tree * tree, Node_t * elem, Node_t ** result);

TreeErr treeGetLen(Tree * tree, size_t * len);
TreeErr treeGetResult(Tree * tree, TreeElem_t * arr, TreeElem_t * result, char * filename=NULL);

TreeElem_t getVal(Node_t * node, TreeElem_t left, TreeElem_t right);
TreeElem_t getResult(Node_t * node, TreeElem_t * arr);

TreeErr treePrint(Tree * tree, FILE * file=stdin);
void PrintNode(Tree * tree, Node_t * node, FILE * file=stdout);
TreeErr saveTree(Tree * tree, const char * filename);

#endif // TREE_H
