#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"
#include "get_tree.h"
#include "differentiation.h"

TreeErr getG(char * s, Tree ** tree) {
  if (*tree != NULL) {
    return INCORRECT_DATA;
  }
  treeInit(tree);
  (*tree)->root = getE(&s, (*tree)->vars);
  if (*s != '$') {
    treeDestroy(*tree);
    return INCORRECT_EXPR;
  }
  s++;
  parent((*tree)->root, &(*tree)->len);
  treeVerify(*tree, "AFTER");
  return SUCCESS;
}

Node_t * getE(char ** s, char ** vars) {
  Node_t * val = getT(s, vars);
  while (**s == '+' || **s == '-') {
    char op = **s;
    (*s)++;
    Node_t * val2 = getT(s, vars);
    if (op == '+') {
      val = ADD_(val, val2);
    }
    else {
      val = SUB_(val, val2);
    }
  }
  return val;
}

Node_t * getT(char ** s, char ** vars) {
  Node_t * val = getP(s, vars);
  while (**s == '*' || **s == '/' || **s == '^') {
    char op = **s;
    (*s)++;
    Node_t * val2 = getP(s, vars);
    if (op == '^') {
      val = POW_(val, val2);
    }
    else if (op == '*') {
      val = MUL_(val, val2);
    }
    else {
      val = DIV_(val, val2);
    }
  }
  return val;
}

Node_t * getP(char ** s, char ** vars) {
  if (**s == '(') {
    (*s)++;
    Node_t * val = getE(s, vars);
    if (**s == ')') {
      (*s)++;
    }
    return val;
  }
  else if ('0' <= **s && **s <= '9') {
    return getN(s);
  }
  char * cur = *s;
  Node_t * val = getF(s, vars);
  if (cur == *s) {
    free(val);
    return getV(s, vars);
  }
  return val;
}

Node_t * getN(char ** s) {
  Node_t * val = ZeroNode;
  while ('0' <= **s && **s <= '9') {
    val->data.val.val = val->data.val.val * 10 + **s - '0';
    (*s)++;
  }
  return val;
}

Node_t * getF(char ** s, char ** vars) {
  Node_t * val = createNode(OP, D(PLUS), NULL, NULL);
  for (size_t i = 0; i < 19; i++) {
    if (!strncmp(*s, operations[i].name, operations[i].len)) {
      val->data = operations[i].data;
      *s += operations[i].len + 1;
      val->left = getE(s, vars);
      (*s)++;
      break;
    }
  }
  if (!strncmp(*s, "log", 3)) {
    val->data = operations[LOG].data;
    *s += operations[LOG].len + 1;
    val->left = getE(s, vars);
    (*s)++;
    val->right = getE(s, vars);
    (*s)++;
  }
  return val;
}

Node_t * getV(char ** s, char ** vars) {
  const size_t max_size = 256;
  char buf[max_size] = {};
  size_t ind = 0;
  while (ind < max_size && (('a' <= **s && **s <= 'z') || ('A' <= **s && **s <= 'Z') || **s == '_')) {
    buf[ind++] = **s;
    (*s)++;
  }
  char f = 1;
  size_t i = 0;
  for (; i < 10; i++) {
    if (vars[i] == NULL) {
      vars[i] = strdup(buf);
      f = 0;
      break;
    }
    else if (!strncmp(vars[i], buf, strlen(vars[i]))) {
      f = 0;
      break;
    }
  }
  if (f) {
    (*s)--;
    return NanNode;
  }
  Data data;
  data.var.ind = i;
  return createNode(VAR, data, NULL, NULL);
}
