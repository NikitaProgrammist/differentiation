#include <stdio.h>

#include "tree.h"
#include "private_tree.h"
#include "utils.h"
#include "differentiation.h"

int main() {
  clear();

  double vals[] = {5, 1};

  Tree * tree = NULL;
  treeInit(&tree);
  getTree(tree);

  Tree * d = NULL;
  differentiation(tree, &d, "x");

  double result = 0;
  treeGetResult(d, vals, &result);
  printf("%lg\n", result);

  Tree * dd = NULL;
  ndiff(tree, &dd, "x", 10);
  treePrint(dd);

  Tree * ty = NULL;
  Teilor(tree, &ty, "x", vals, 10);
  treePrint(ty);

  treePrint(tree);
  treePrint(d);
  treeDestroy(ty);
  treeDestroy(dd);
  treeDestroy(d);
  treeDestroy(tree);
  return 0;
}
