#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"
#include "get_tree.h"
#include "differentiation.h"

TreeErr treeInit(Tree ** tree) {
  *tree = (Tree *) calloc(1, sizeof(Tree));
  if (*tree == NULL) {
    return CREATE_FAILED;
  }
  (*tree)->len = 0;
  (*tree)->root = NULL;
  treeVerify(*tree, "AFTER");
  return SUCCESS;
}

void treeDestroy(Tree * tree) {
  if (tree == NULL) {
    return;
  }
  DestroyNode(tree->root, &tree->len);
  for (size_t i = 0; i < 10; i++) {
    free(tree->vars[i]);
  }
  free(tree);
}

void DestroyNode(Node_t * node, size_t * len) {
  if (node->left != NULL) {
    DestroyNode(node->left, len);
  }
  if (node->right != NULL) {
    DestroyNode(node->right, len);
  }
  free(node);
  (*len)--;
  return;
}

TreeErr mainTreeCycle(Tree * tree, Tree ** res, char * filename) {
  printf("Выберите опцию: [1]первая производная, [2]производная n порядка, [3]разложение по Тейлору до o((x - x_0)^n), [4]значение выражения в точке.\n");
  char c = getchar();
  size_t len_str = 0;
  char * str = NULL;
  size_t len_val = 0;
  TreeElem_t * vals = NULL;
  size_t ord = 0;
  TreeErr result = SUCCESS;
  while (getchar() != '\n');

  if (c == '1' || c == '2' || c == '3') {
    printf("Введите переменную, по которой надо дифференцировать.\n");
    getline(&str, &len_str, stdin);
  }
  if (c == '2' || c == '3') {
    printf("Введите порядок дифференцирования.\n");
    scanf("%zu", &ord);
  }
  if (c == '3' || c == '4') {
    printf("Введите количество значений и значения переменных.\n");
    scanf("%zu", &len_val);
    vals = (TreeElem_t *) calloc(len_val, sizeof(TreeElem_t));
    for (size_t i = 0; i < len_val; i++) {
      scanf("%lg", vals + i);
    }
  }

  if (c == '1') {
    result = differentiation(tree, res, str, filename);
    free(str);
    tex(*res, filename, "AFTER");
    return result;
  }
  if (c == '2') {
    result = ndiff(tree, res, str, ord, filename);
    free(str);
    tex(*res, filename, "AFTER");
    return result;
  }
  if (c == '3') {
    result = Teilor(tree, res, str, vals, ord, filename);
    free(str);
    free(vals);
    tex(*res, filename, "AFTER");
    return result;
  }
  if (c == '4') {
    result = treeInit(res);
    if (result != SUCCESS) {
      return result;
    }
    (*res)->root = (Node_t *) calloc(1, sizeof(Node_t));
    (*res)->root->type = VALUE;
    result = treeGetResult(tree, vals, &(*res)->root->data.val.val, filename);
    tex(*res, filename, "AFTER");
    free(vals);
    return result;
  }
  tex(*res, filename, "AFTER");
  return result;
}

TreeErr getTree(Tree ** tree, char * filename) {
  FILE * file = fopen(filename, "r");
  size_t n = 0;
  char * str = NULL;
  getline(&str, &n, file);
  TreeErr result = getG(str, tree);
  free(str);
  fclose(file);
  return result;
}

TreeErr treeGetFirst(Tree * tree, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = tree->root;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetLeft(Tree * tree, Node_t * elem, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = elem->left;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetRight(Tree * tree, Node_t * elem, Node_t ** result) {
  treeVerify(tree, "BEFORE");
  *result = elem->right;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetLen(Tree * tree, size_t * len) {
  treeVerify(tree, "BEFORE");
  *len = tree->len;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeGetResult(Tree * tree, TreeElem_t * arr, TreeElem_t * result, char * filename) {
  treeVerify(tree, "BEFORE");
  tex(tree, filename, "BEFORE");
  *result = getResult(tree->root, arr);
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeElem_t getResult(Node_t * node, TreeElem_t * arr) {
  TreeElem_t left = NAN, right = NAN;
  if (node->type == VAL) {
    return node->data.val.val;
  }
  else if (node->type == VAR) {
    return arr[node->data.var.ind];
  }
  if (node->left != NULL) {
    left = getResult(node->left, arr);
  }
  if (node->right != NULL) {
    right = getResult(node->right, arr);
  }
  return getVal(node, left, right);
}

TreeElem_t getVal(Node_t * node, TreeElem_t left, TreeElem_t right) {
  switch (node->data.op) {
    case SIN:
      return sin(left);
    case COS:
      return cos(left);
    case TAN:
      return tan(left);
    case COT:
      return 1.0 / tan(left);
    case ASIN:
      return asin(left);
    case ACOS:
      return acos(left);
    case ATAN:
      return atan(left);
    case ACOT:
      return M_PI_2 - atan(left);
    case SINH:
      return sinh(left);
    case COSH:
      return cosh(left);
    case TANH:
      return tanh(left);
    case COTH:
      return 1.0 / tanh(left);
    case ASINH:
      return asinh(left);
    case ACOSH:
      return acosh(left);
    case ATANH:
      return atanh(left);
    case ACOTH:
      return tanh(1.0 / left);
    case LN:
      return log(left);
    case LOG:
      return log(right) / log(left);
    case ADD:
      return left + right;
    case SUB:
      return left - right;
    case MUL:
      return left * right;
    case DIV:
      return left / right;
    case POW:
      return pow(left, right);
    case PLUS:
      return left;
    case MINUS:
      return -left;
    default:
      return NAN;
  }
}

TreeErr treePrint(Tree * tree, FILE * file) {
  treeVerify(tree, "BEFORE");
  PrintNode(tree, tree->root, file);
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

void PrintNode(Tree * tree, Node_t * node, FILE * file) {
  if (node->type == OPERATION && (node->left == NULL || node->right == NULL)) {
    fprintf(file, " \\%s{", operations[node->data.op].name);
  }
  if (node->type == VARIABLE) {
    fprintf(file, " {%s} ", tree->vars[node->data.var.ind]);
  }
  if (node->type == VALUE) {
    fprintf(file, " {%lg} ", node->data.val.val);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL && node->parent != NULL &&
      ((node->parent->data.op > SUB && node->data.op < MUL) || node->parent->data.op >= POW)) {
    fprintf(file, " (");
  }
  if (node->left != NULL) {
    PrintNode(tree, node->left, file);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL) {
    switch(node->data.op) {
      case ADD:
        fprintf(file, " + ");
        break;
      case SUB:
        fprintf(file, " - ");
        break;
      case MUL:
        fprintf(file, " \\cdot ");
        break;
      case DIV:
        fprintf(file, " \\over ");
        break;
      case POW:
        fprintf(file, " ^ ");
        break;
      default:
        fprintf(file, " %s ", operations[node->data.op].name);
        break;
    }
  }
  if (node->right != NULL) {
    PrintNode(tree, node->right, file);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL && node->parent != NULL &&
      ((node->parent->data.op > SUB && node->data.op < MUL) || node->parent->data.op >= POW)) {
    fprintf(file, ") ");
  }
  if (node->type == OPERATION && (node->left == NULL || node->right == NULL)) {
    fprintf(file, "} ");
  }
  return;
}

TreeErr saveTree(Tree * tree, const char * filename) {
  treeVerify(tree, "BEFORE");
  FILE * file = fopen(filename, "w");
  if (file == NULL) {
    return FILE_ERR;
  }
  PrintNode(tree, tree->root, file);
  treeVerify(tree, "AFTER");
  fclose(file);
  return SUCCESS;
}
