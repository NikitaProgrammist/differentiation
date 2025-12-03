#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"
#include "differentiation.h"

TreeErr Teilor(Tree * tree, Tree ** ty, const char * param, TreeElem_t * arr, size_t n) {
  treeVerify(tree, "BEFORE");
  int ind = getInd(tree, param);
  if (ind == -1) {
    treeInit(ty);
    copyVars((*ty)->vars, tree->vars);
    Data data;
    data.val.val = getResult(tree->root, arr);
    (*ty)->root = createNode(VAL, data, NULL, NULL);
    (*ty)->len = 1;
    treeVerify(*ty, "AFTER");
    return SUCCESS;
  }
  Tree ** dif = (Tree **) calloc(n + 1, sizeof(Tree *));
  dif[0] = tree;
  for (size_t i = 0; i < n; i++) {
    TreeErr result = differentiation(dif[i], dif + i + 1, param);
    if (result != SUCCESS) {
      for (size_t j = 1; j <= i ; j++) {
        treeDestroy(dif[j]);
      }
      free(dif);
      return result;
    }
  }
  TreeElem_t result = getResult(dif[0]->root, arr);
  dif[0] = NULL;
  treeInit(dif);
  copyVars((*dif)->vars, tree->vars);
  Data data;
  data.val.val = result;
  dif[0]->root = createNode(VAL, data, NULL, NULL);
  for (size_t i = 1; i <= n; i++) {
    result = getResult(dif[i]->root, arr) / tgamma(i + 1);
    treeDestroy(dif[i]);
    dif[i] = NULL;
    treeInit(dif + i);
    copyVars(dif[i]->vars, tree->vars);
    data.val.val = result;
    Data var;
    var.var.ind = ind;
    Data val;
    val.val.val = arr[ind];
    Data st;
    st.val.val = i;
    dif[i]->root = MUL_(createNode(VAL, data, NULL, NULL), POW_(SUB_(createNode(VAR, var, NULL, NULL),
                        createNode(VAL, val, NULL, NULL)), createNode(VAL, st, NULL, NULL)));
  }
  treeInit(ty);
  copyVars((*ty)->vars, tree->vars);
  (*ty)->root = dif[0]->root;
  for (size_t i = 0; i < 10; i++) {
    free(dif[0]->vars[i]);
  }
  free(dif[0]);
  for (size_t i = 1; i <= n; i++) {
    (*ty)->root = ADD_((*ty)->root, dif[i]->root);
    for (size_t j = 0; j < 10; j++) {
      free(dif[i]->vars[j]);
    }
    free(dif[i]);
  }
  parent((*ty)->root, &(*ty)->len);
  free(dif);
  simplify(ty);
  treeVerify(*ty, "AFTER");
  return SUCCESS;
}

TreeErr ndiff(Tree * tree, Tree ** dy, const char * param, size_t n) {
  treeVerify(tree, "BEFORE");
  Tree * cur = tree;
  Tree * next = NULL;
  for (size_t i = 0; i < n; i++) {
    TreeErr result = differentiation(cur, &next, param);
    if (result != SUCCESS) {
      if (cur != tree) {
        treeDestroy(cur);
      }
      treeDestroy(next);
      return result;
    }
    if (cur != tree) {
      treeDestroy(cur);
    }
    cur = next;
    next = NULL;
  }
  *dy = cur;
  treeVerify(*dy, "AFTER");
  return SUCCESS;
}

TreeErr differentiation(Tree * tree, Tree ** dy, const char * param) {
  treeVerify(tree, "BEFORE");
  if (*dy != NULL) {
    return INCORRECT_DATA;
  }
  if (tree->root == NULL) {
    return EMPTY_TREE;
  }
  int ind = getInd(tree, param);
  treeInit(dy);
  copyVars((*dy)->vars, tree->vars);
  if (ind == -1) {
    (*dy)->root = ZeroNode;
    (*dy)->len = 1;
    treeVerify(*dy, "AFTER");
    return SUCCESS;
  }
  (*dy)->root = differen(tree->root, ind);
  parent((*dy)->root, &(*dy)->len);
  treeVerify(*dy, "BEFORE");
  simplify(dy);
  treeVerify(*dy, "AFTER");
  return SUCCESS;
}

int getInd(Tree * tree, const char * param) {
  for (int i = 0; i < 10; i++) {
    if (tree->vars[i] == 0) {
      return -1;
    }
    if (!strncmp(tree->vars[i], param, strlen(tree->vars[i]))) {
      return i;
    }
  }
  return -1;
}

Node_t * differen(Node_t * node, int ind) {
  if (node->type == VAL || (node->type == VAR && node->data.var.ind != ind)) {
    return mconstant();
  }
  else if (node->type == VAR && node->data.var.ind == ind) {
    return mx();
  }
  else if (node->type == OP && (node->data.op == ADD || node->data.op == SUB)) {
    return msum(node, ind);
  }
  else if (node->type == OP && (node->data.op == PLUS || node->data.op == MINUS)) {
    return mplus(node, ind);
  }
  else if (node->type == OP && node->data.op == MUL) {
    return mmul(node, ind);
  }
  else if (node->type == OP && node->data.op == DIV) {
    return mdiv(node, ind);
  }
  else if (node->type == OP && node->data.op == SIN) {
    return msin(node, ind);
  }
  else if (node->type == OP && node->data.op == COS) {
    return mcos(node, ind);
  }
  else if (node->type == OP && node->data.op == TAN) {
    return mtan(node, ind);
  }
  else if (node->type == OP && node->data.op == COT) {
    return mcot(node, ind);
  }
  else if (node->type == OP && node->data.op == ASIN) {
    return masin(node, ind);
  }
  else if (node->type == OP && node->data.op == ACOS) {
    return macos(node, ind);
  }
  else if (node->type == OP && node->data.op == ATAN) {
    return matan(node, ind);
  }
  else if (node->type == OP && node->data.op == ACOT) {
    return macot(node, ind);
  }
  else if (node->type == OP && node->data.op == SINH) {
    return msinh(node, ind);
  }
  else if (node->type == OP && node->data.op == COSH) {
    return mcosh(node, ind);
  }
  else if (node->type == OP && node->data.op == TANH) {
    return mtanh(node, ind);
  }
  else if (node->type == OP && node->data.op == COTH) {
    return mcoth(node, ind);
  }
  else if (node->type == OP && node->data.op == ASINH) {
    return masinh(node, ind);
  }
  else if (node->type == OP && node->data.op == ACOSH) {
    return macosh(node, ind);
  }
  else if (node->type == OP && node->data.op == ATANH) {
    return matanh(node, ind);
  }
  else if (node->type == OP && node->data.op == ACOTH) {
    return macoth(node, ind);
  }
  else if (node->type == OP && node->data.op == LN) {
    return mln(node, ind);
  }
  else if (node->type == OP && node->data.op == LOG) {
    return mlog(node, ind);
  }
  else if (node->type == OP && node->data.op == POW) {
    return mpow(node, ind);
  }
  return NanNode;
}

void parent(Node_t * node, size_t * len) {
  (*len)++;
  if (node->left != NULL) {
    parent(node->left, len);
    node->left->parent = node;
  }
  if (node->right != NULL) {
    parent(node->right, len);
    node->right->parent = node;
  }
}

Node_t * createNode(Type type, Data data, Node_t * left, Node_t * right) {
  Node_t * result = (Node_t *) calloc(1, sizeof(Node_t));
  result->type = type;
  result->data = data;
  result->left = left;
  result->right = right;
  return result;
}

void copyVars(char ** new_vars, char ** old_vars) {
  for (size_t i = 0; i < 10; i++) {
    if (old_vars[i] == NULL) {
      return;
    }
    new_vars[i] = strdup(old_vars[i]);
  }
}

void simplify(Tree ** tree) {
  size_t length = (*tree)->len;
  while (1) {
    (*tree)->root = calcConstant((*tree)->root, &(*tree)->len);
    (*tree)->root = removeOneZero((*tree)->root, &(*tree)->len);
    if (length != (*tree)->len) {
      length = (*tree)->len;
    }
    else {
      break;
    }
  }
}

Node_t * calcConstant(Node_t * node, size_t * len) {
  if ((node == NULL) || (node->left == NULL && node->right == NULL)) {
    return node;
  }
  Node_t * left = calcConstant(node->left, len);
  Node_t * right = calcConstant(node->right, len);
  if ((left->type == VAL && right == NULL) || (left->type == VAL && right->type == VAL)) {
    Data data;
    if (right != 0) {
      data.val.val = getVal(node, left->data.val.val, right->data.val.val);
    }
    else {
      data.val.val = getVal(node, left->data.val.val, 0);
    }
    node->type = VAL;
    node->data = data;
    DestroyNode(node->left, len);
    if (node->right != NULL) {
      DestroyNode(node->right, len);
    }
    node->left = node->right = NULL;
  }
  return node;
}

Node_t * removeOneZero(Node_t * node, size_t * len) {
  if ((node == NULL) || (node->left == NULL && node->right == NULL)) {
    return node;
  }
  Node_t * left = removeOneZero(node->left, len);
  Node_t * right = removeOneZero(node->right, len);
  if (node->type == OP) {
    if (node->data.op == PLUS) {
      return dropNode(node, left, len);
    }
    if (node->data.op == MINUS && node->left != NULL && node->left->type == OP && node->left->data.op == MINUS) {
      Node_t * cur = dropNode(node, left, len);
      return dropNode(cur, cur->left, len);
    }
    else if (left->type == VAL && left->data.val.val == 1) {
      if (node->data.op == MUL) {
        DestroyNode(left, len);
        return dropNode(node, right, len);
      }
      else if (node->data.op == POW) {
        subTreeToConst(node, left, right, 1, len);
      }
    }
    else if (right != NULL && right->type == VAL && right->data.val.val == 1) {
      if (node->data.op == MUL || node->data.op == DIV || node->data.op == POW) {
        DestroyNode(right, len);
        return dropNode(node, left, len);
      }
    }
    else if (left->type == VAL && left->data.val.val == 0) {
      if (node->data.op == ADD || node->data.op == SUB) {
        DestroyNode(left, len);
        return dropNode(node, right, len);
      }
      else if (node->data.op == MUL || node->data.op == POW) {
        subTreeToConst(node, left, right, 0, len);
      }
    }
    else if (right != NULL && right->type == VAL && right->data.val.val == 0) {
      if (node->data.op == ADD || node->data.op == SUB) {
        DestroyNode(right, len);
        return dropNode(node, left, len);
      }
      else if (node->data.op == MUL) {
        subTreeToConst(node, left, right, 0, len);
      }
      else if (node->data.op == POW) {
        subTreeToConst(node, left, right, 1, len);
      }
    }
  }
  return node;
}

Node_t * dropNode(Node_t * node, Node_t * subnode, size_t * len) {
  subnode->parent = node->parent;
  if (node->parent != NULL && node == node->parent->left) {
    node->parent->left = subnode;
  }
  else if (node->parent != NULL) {
    node->parent->right = subnode;
  }
  free(node);
  (*len)--;
  return subnode;
}

void subTreeToConst(Node_t * node, Node_t * left, Node_t * right, TreeElem_t scalar, size_t * len) {
  DestroyNode(left, len);
  DestroyNode(right, len);
  node->left = node->right = NULL;
  node->type = VAL;
  node->data.val.val = scalar;
}

Node_t * copy(Node_t * node) {
  if (node == NULL) {
    return NULL;
  }
  return COPY(node);
}

Node_t * mconstant() {
  return ZeroNode;
}

Node_t * mx() {
  return OneNode;
}

Node_t * msum(Node_t * node, int ind) {
  return BDIF(node);
}

Node_t * mplus(Node_t * node, int ind) {
  return UDIF(node);
}

Node_t * msin(Node_t * node, int ind) {
  return MUL_(COS_(CL, NULL), DL);
}

Node_t * mcos(Node_t * node, int ind) {
  return MUL_(MINUS_(SIN_(CL, NULL), NULL), DL);
}

Node_t * mtan(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, POW_(COS_(CL, NULL), TwoNode)), DL);
}

Node_t * mcot(Node_t * node, int ind) {
  return MUL_(MINUS_(DIV_(OneNode, POW_(SIN_(CL, NULL), TwoNode)), NULL), DL);
}

Node_t * masin(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, POW_(SUB_(OneNode, POW_(CL, TwoNode)), HalfNode)), DL);
}

Node_t * macos(Node_t * node, int ind) {
  return MINUS_(masin(node, ind), NULL);
}

Node_t * matan(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, ADD_(OneNode, POW_(CL, TwoNode))), DL);
}

Node_t * macot(Node_t * node, int ind) {
  return MINUS_(matan(node, ind), NULL);
}

Node_t * msinh(Node_t * node, int ind) {
  return MUL_(COSH_(CL, NULL), DL);
}

Node_t * mcosh(Node_t * node, int ind) {
  return MUL_(SINH_(CL, NULL), DL);
}

Node_t * mtanh(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, POW_(COSH_(CL, NULL), TwoNode)), DL);
}

Node_t * mcoth(Node_t * node, int ind) {
  return MUL_(MINUS_(DIV_(OneNode, POW_(SINH_(CL, NULL), TwoNode)), NULL), DL);
}

Node_t * masinh(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, POW_(ADD_(OneNode, POW_(CL, TwoNode)), HalfNode)), DL);
}

Node_t * macosh(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, POW_(SUB_(POW_(CL, TwoNode), OneNode), HalfNode)), DL);
}

Node_t * matanh(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, SUB_(OneNode, POW_(CL, TwoNode))), DL);
}

Node_t * macoth(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, SUB_(OneNode, POW_(CL, TwoNode))), DL);
}

Node_t * mln(Node_t * node, int ind) {
  return MUL_(DIV_(OneNode, CL), DL);
}

Node_t * mlog(Node_t * node, int ind) {
  Node_t * divide = DIV_(LN_(CR, NULL), LN_(CL, NULL));
  Node_t * result = differen(divide, ind);
  size_t _ = 0;
  DestroyNode(divide, &_);
  return result;
}

Node_t * mmul(Node_t * node, int ind) {
  return ADD_(MUL_(DL, CR), MUL_(CL, DR));
}

Node_t * mdiv(Node_t * node, int ind) {
  return DIV_(SUB_(MUL_(DL, CR), MUL_(DR, CL)), POW_(CR, TwoNode));
}

Node_t * mpow(Node_t * node, int ind) {
  if (node->left->type == VALUE) {
    return st(node, ind);
  }
  else if (node->right->type == VALUE) {
    return osn(node, ind);
  }
  return sm(node, ind);
}

Node_t * osn(Node_t * node, int ind) {
  return MUL_(MUL_(CR, POW_(CL, SUB_(CR, OneNode))), DL);
}

Node_t * st(Node_t * node, int ind) {
  return MUL_(MUL_(POW_(CL, CR), LN_(CL, NULL)), DR);
}

Node_t * sm(Node_t * node, int ind) {
  Node_t * stepen = MUL_(CR, LN_(CL, NULL));
  Node_t * result = MUL_(POW_(CL, CR), differen(stepen, ind));
  size_t _ = 0;
  DestroyNode(stepen, &_);
  return result;
}
