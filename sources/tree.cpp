#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"

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

TreeErr treeGetResult(Tree * tree, TreeElem_t * arr, TreeElem_t * result) {
  treeVerify(tree, "BEFORE");
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

TreeErr treePrint(Tree * tree) {
  treeVerify(tree, "BEFORE");
  PrintNode(tree, tree->root);
  printf("\n");
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

void PrintNode(Tree * tree, Node_t * node, FILE * file) {
  if (node->type == OPERATION && (node->left == NULL || node->right == NULL)) {
    fprintf(file, " %s(", operations[node->data.op].name);
  }
  if (node->type == VARIABLE) {
    fprintf(file, " %s ", tree->vars[node->data.var.ind]);
  }
  if (node->type == VALUE) {
    fprintf(file, " %lg ", node->data.val.val);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL && node->parent != NULL &&
      ((node->parent->data.op > SUB && node->data.op < MUL) || node->parent->data.op >= POW)) {
    fprintf(file, " (");
  }
  if (node->left != NULL) {
    PrintNode(tree, node->left, file);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL) {
    fprintf(file, "%s", operations[node->data.op].name);
  }
  if (node->right != NULL) {
    PrintNode(tree, node->right, file);
  }
  if (node->type == OPERATION && node->left != NULL && node->right != NULL && node->parent != NULL &&
      ((node->parent->data.op > SUB && node->data.op < MUL) || node->parent->data.op >= POW)) {
    fprintf(file, ") ");
  }
  if (node->type == OPERATION && (node->left == NULL || node->right == NULL)) {
    fprintf(file, ") ");
  }
  return;
}

/*
TreeErr getTree(Tree * tree) {
  treeVerify(tree, "BEFORE");
  tree->root = (Node_t *) calloc(1, sizeof(Node_t));
  tree->root->type = OPERATION;
  tree->root->data.op = MUL;
  tree->root->left = (Node_t *) calloc(1, sizeof(Node_t));
  tree->root->left->type = VARIABLE;
  tree->root->left->data.var.ind = 0;
  tree->vars[0] = "x";
  tree->root->right = (Node_t *) calloc(1, sizeof(Node_t));
  tree->root->right->type = OPERATION;
  tree->root->right->data.op = ADD;
  tree->root->right->left = (Node_t *) calloc(1, sizeof(Node_t));
  tree->root->right->right = (Node_t *) calloc(1, sizeof(Node_t));
  tree->root->right->left->type = VARIABLE;
  tree->root->right->left->data.var.ind = 1;
  tree->vars[1] = "y";
  tree->root->right->right->type = VALUE;
  tree->root->right->right->data.val.val = 6;
  tree->len = 5;
  tree->root->left->parent = tree->root;
  tree->root->right->parent = tree->root;
  tree->root->right->left->parent = tree->root->right;
  tree->root->right->right->parent = tree->root->right;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}
*/

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
