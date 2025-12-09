#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"
#include "check_tree.h"
#include "get_tree.h"
#include "differentiation.h"
#include "my_printf.h"

int main(int argc, char ** argv) {
  if (argc != 3) {
    colorPrintf(RED, BOLD, "Некорректное количество аргументов.\n");
    return 0;
  }
  clear(argv[2]);

  Tree * tree = NULL;
  TreeErr result = getTree(&tree, argv[1]);
  if (result != SUCCESS) {
    parseErrors(result);
    return 0;
  }
  Tree * res = NULL;
  result = mainTreeCycle(tree, &res, argv[2]);
  if (result != SUCCESS) {
    parseErrors(result);
    treeDestroy(tree);
    return 0;
  }

  treeDestroy(tree);
  treeDestroy(res);
  FILE * file = fopen(argv[2], "a");
  fprintf(file, "\\end{document}\n");
  fclose(file);
  return 0;
}
