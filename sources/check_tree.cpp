#ifdef CHECK

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "my_printf.h"
#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"

Errors verify(const Tree * tree, const char * file, const char * func, const size_t len, const char * position) {
  if (tree == NULL) {
    return INVALID;
  }
  if (checkCycle(tree) != VALID) {
    dump(tree, file, func, len, position, INVALID);
    return INVALID;
  }
  dump(tree, file, func, len, position, VALID);
  checkStatement(tree == NULL);
  return VALID;
}

Errors checkCycle(const Tree * tree) {
  if (tree->root == NULL) {
    return VALID;
  }
  Node_t ** stack = (Node_t **) calloc(tree->len + 1, sizeof(Node_t *));
  if (stack == NULL) {
    return INVALID;
  }
  stack[0] = tree->root;
  size_t len = 0;
  int i = 0;
  while (i >= 0 && len < tree->len + 1) {
    Node_t * node = stack[i];
    for (size_t j = i; j < len; j++) {
        stack[j] = stack[j + 1];
    }
    i--;
    if (node->left != NULL) {
        stack[++i] = node->left;
        len++;
    }
    if (node->right != NULL) {
        stack[++i] = node->right;
        len++;
    }
  }
  free(stack);
  return (i == -1) ? VALID : INVALID;
}

void dump(const Tree * tree, const char * file, const char * func, const size_t strnum, const char * position, Errors img) {
  FILE * fp = fopen("./log.html", "a");
  if (fp == NULL) {
    return;
  }
  fprintf(fp, "<pre>\n");
  fprintf(fp, "<h3> DUMP <font color=red> %s </font> %s (%zu) </h3>\n", position, func, strnum);
  fprintf(fp, "Tree { ./%s:%zu }\n", file, strnum);
  static size_t picture_number = 0;
  if (tree && tree->root) {
    Node_t ** stack = (Node_t **) calloc(tree->len + 1, sizeof(Node_t *));
    int * state = (int *) calloc(tree->len + 1, sizeof(int));
    int stack_size = 0;
    Node_t * current = tree->root;
    int current_state = 0;
    while (current || stack_size > 0) {
      if (current) {
        if (current_state == 0) {
          if (current->type == OPERATION) {
            if (!current->left || !current->right) {
              fprintf(fp, "%s(", getNodeDataString(current, tree));
            }
            else if (current->parent &&
                     ((current->parent->data.op > SUB && current->data.op < MUL) ||
                       current->parent->data.op >= POW)) {
              fprintf(fp, "(");
            }
          }
          stack[stack_size] = current;
          state[stack_size] = 1;
          stack_size++;
          current = current->left;
          current_state = 0;
          continue;
        }
      }
      stack_size--;
      current = stack[stack_size];
      current_state = state[stack_size];
      if (current_state == 1) {
        if (current->type == VARIABLE) {
          fprintf(fp, "%s", tree->vars[current->data.var.ind]);
        }
        else if (current->type == VALUE) {
          fprintf(fp, "%lg", current->data.val.val);
        }
        if (current->type == OPERATION && current->left && current->right) {
          fprintf(fp, " %s ", getNodeDataString(current, tree));
        }
        stack[stack_size] = current;
        state[stack_size] = 2;
        stack_size++;
        current = current->right;
        current_state = 0;
        continue;
      }
      if (current_state == 2) {
        if (current->type == OPERATION) {
          if (current->left && current->right && current->parent &&
              ((current->parent->data.op > SUB && current->data.op < MUL) ||
                current->parent->data.op >= POW)) {
            fprintf(fp, ")");
          }
          if (!current->left || !current->right) {
            fprintf(fp, ")");
          }
        }
        current = NULL;
      }
    }
    free(stack);
    free(state);
    fprintf(fp, "\n");
    if (!img) {
      createGraph(tree, picture_number);
      fprintf(fp, "\nImage:\n<img src=./img/img%zu.png width=500px>\n", picture_number);
      picture_number++;
    }
  }
  else {
    fprintf(fp, "Пустое дерево или нулевой указатель.\n");
  }
  fclose(fp);
}

void toLatex(Tree * tree, char * filename, const char * funcname, const char * time) {
  FILE * file = fopen(filename, "a");
  fprintf(file, "\nЭтап %s %s.\n\n", time, funcname);
  fprintf(file, "\\[\n");
  treePrint(tree, file);
  fprintf(file, "\n\\]\n");
  fclose(file);
}

const char * getNodeDataString(const Node_t * node, const Tree * tree) {
    static char buffer[256];
    switch (node->type) {
        case OPERATION:
            if (node->data.op >= 0 && node->data.op < 26) {
                return operations[node->data.op].name;
            }
            return "unknown_op";
        case VARIABLE:
            if (node->data.var.ind < tree->len) {
                return tree->vars[node->data.var.ind];
            }
            return "unknown_var";
        case VALUE:
            snprintf(buffer, sizeof(buffer), "%.2f", node->data.val.val);
            return buffer;
        default:
            return "unknown";
    }
}

const char* getNodeTypeString(Type type) {
    switch (type) {
        case OPERATION: return "OPERATION";
        case VARIABLE: return "VARIABLE";
        case VALUE: return "VALUE";
        default: return "UNKNOWN";
    }
}

void createSquareNodes(FILE * file, const Node_t * node, const Tree* tree) {
  if (!node) return;

  fprintf(file, "  node_%p [label=<\n", node);
  fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
  fprintf(file, "      <tr><td border=\"1\" color=\"#ffff00\" colspan=\"2\"><b>%s</b></td></tr>\n",
          getNodeTypeString(node->type));
  fprintf(file, "      <tr><td border=\"1\" color=\"#ffff00\" colspan=\"2\"><b>%s</b></td></tr>\n",
          getNodeDataString(node, tree));
  fprintf(file, "      <tr>\n");

  if (node->left == NULL) {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\">nil</td>\n");
  } else {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\">%p</td>\n", (void*)node->left);
  }

  if (node->right == NULL) {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\">nil</td>\n");
  } else {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\">%p</td>\n", (void*)node->right);
  }

  fprintf(file, "      </tr>\n");

  fprintf(file, "      <tr>\n");
  if (node->parent == NULL) {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\" colspan=\"2\">root</td>\n");
  } else {
    fprintf(file, "        <td border=\"1\" color=\"#ffff00\" colspan=\"2\">%p</td>\n", (void*)node->parent);
  }
  fprintf(file, "      </tr>\n");

  fprintf(file, "    </table>\n");
  fprintf(file, "  >];\n");

  createSquareNodes(file, node->left, tree);
  createSquareNodes(file, node->right, tree);
}

void createDirectEdges(FILE * file, Node_t * node) {
  if (!node) return;
  if (node->left) {
    fprintf(file, "  node_%p -> node_%p [color=\"#4d78cc\"];\n", (void*)node, (void*)node->left);
  }
  if (node->right) {
    fprintf(file, "  node_%p -> node_%p [color=\"#109b48\"];\n", (void*)node, (void*)node->right);
  }
  createDirectEdges(file, node->left);
  createDirectEdges(file, node->right);
}

void createGraph(const Tree * tree, size_t picture_number) {
  char filename[100];
  sprintf(filename, "./img/img%zu.txt", picture_number);
  FILE * file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }
  fprintf(file, "digraph G {\n");
  fprintf(file, "  bgcolor=\"#1f1f1f\";\n");
  fprintf(file, "  rankdir=TB;\n");
  fprintf(file, "  nodesep=0.5;\n");
  fprintf(file, "  ranksep=0.8;\n\n");
  fprintf(file, "  node [shape=plain, fontname=\"Arial\", fontsize=12, style=\"filled\", fillcolor=\"#6e7681\", color=\"#FFFF00\", fontcolor=\"black\"];\n");
  fprintf(file, "  edge [penwidth=2, arrowsize=0.8, color=\"#FFFF00\"];\n\n");
  fprintf(file, "  info [label=<\n");
  fprintf(file, "    <table border=\"1\" cellborder=\"1\" cellspacing=\"0\" bgcolor=\"#6e7681\" color=\"#FFFF00\">\n");
  if (tree->root == NULL) {
    fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\"><b>root = nil</b></td></tr>\n");
  }
  else {
    fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\"><b>root = %p</b></td></tr>\n", (void*)tree->root);
  }
  fprintf(file, "      <tr><td border=\"1\" color=\"#ffff66\">size = %zu</td></tr>\n", tree->len);
  fprintf(file, "    </table>\n");
  fprintf(file, "  >];\n\n");
  fprintf(file, "  // Tree nodes\n");
  createSquareNodes(file, tree->root, tree);
  if (tree->root != NULL) {
    fprintf(file, "  info -> node_%p [color=\"#f112bdff\"];\n", (void*)tree->root);
  }
  createDirectEdges(file, tree->root);
  fprintf(file, "}\n");
  fclose(file);
  char command[256];
  snprintf(command, sizeof(command), "dot -Tpng \"./img/img%zu.txt\" -o \"./img/img%zu.png\" -n2", picture_number, picture_number);
  system(command);
}

void printErrorLines(const char * file, const char * func, size_t line_number) {
  colorPrintf(RED, BOLD, "file: %s:%zu\nfunc: %s\n", file, line_number, func);

  const int MAX_LINE = 150;
  char line[MAX_LINE] = {};
  FILE * fp = fopen(file, "r");
  if (fp == NULL) {
    return;
  }

  for (size_t i = 0; i <= line_number; i++) {
    fgets(line, MAX_LINE, fp);
    if (i >= line_number - 2)
      colorPrintf(RED, BOLD, "%zu: %s", i + 1, line);
  }

  fclose(fp);
}

#endif // CHECK
