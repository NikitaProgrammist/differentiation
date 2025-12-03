#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"
#include "check_tree.h"
#include "get_tree.h"
#include "differentiation.h"

int main() {
  clear();

  double vals[] = {0, 1};

  Tree * tree = NULL;
  size_t n = 0;
  char * str = NULL;
  getline(&str, &n, stdin);
  TreeErr r = getG(str, &tree);
  if (r != SUCCESS) {
    parseErrors(r);
    free(str);
    return 0;
  }
  free(str);

  Tree * d = NULL;
  differentiation(tree, &d, "x");
  treePrint(d);

  Tree * dd = NULL;
  ndiff(tree, &dd, "x", 5);
  treePrint(dd);

  Tree * ty = NULL;
  Teilor(tree, &ty, "x", vals, 10);
  treePrint(ty);


  double result = 0;
  treeGetResult(tree, vals, &result);
  printf("%lg\n", result);

  treePrint(tree);
  treeDestroy(ty);
  treeDestroy(dd);
  treeDestroy(d);
  treeDestroy(tree);
  return 0;
}
