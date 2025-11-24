#ifndef DIFFERENTIATION_H
#define DIFFERENTIATION_H

TreeErr Teilor(Tree * tree, Tree ** ty, char * param, TreeElem_t * arr, size_t n);
TreeErr ndiff(Tree * tree, Tree ** dy, char * param, size_t n);
TreeErr differentiation(Tree * tree, Tree ** dy, char * param);
Node_t * differen(Node_t * node, int ind);
int getInd(Tree * tree, char * param);
void parent(Node_t * node, size_t * len);
Node_t * createNode(Type type, Data data, Node_t * left, Node_t * right);
Node_t * copy(Node_t * node);
void simplify(Tree ** tree);
Node_t * calcConstant(Node_t * node, size_t * len);
Node_t * removeOneZero(Node_t * node, size_t * len);
Node_t * dropNode(Node_t * node, Node_t * subnode, size_t * len);
void subTreeToConst(Node_t * node, Node_t * left, Node_t * right, TreeElem_t scalar, size_t * len);
Node_t * mconstant();
Node_t * mx();
Node_t * msum(Node_t * node, int ind);
Node_t * mplus(Node_t * node, int ind);
Node_t * msin(Node_t * node, int ind);
Node_t * mcos(Node_t * node, int ind);
Node_t * mtan(Node_t * node, int ind);
Node_t * mcot(Node_t * node, int ind);
Node_t * masin(Node_t * node, int ind);
Node_t * macos(Node_t * node, int ind);
Node_t * matan(Node_t * node, int ind);
Node_t * macot(Node_t * node, int ind);
Node_t * msinh(Node_t * node, int ind);
Node_t * mcosh(Node_t * node, int ind);
Node_t * mtanh(Node_t * node, int ind);
Node_t * mcoth(Node_t * node, int ind);
Node_t * masinh(Node_t * node, int ind);
Node_t * macosh(Node_t * node, int ind);
Node_t * matanh(Node_t * node, int ind);
Node_t * macoth(Node_t * node, int ind);
Node_t * mln(Node_t * node, int ind);
Node_t * mlog(Node_t * node, int ind);
Node_t * mmul(Node_t * node, int ind);
Node_t * mdiv(Node_t * node, int ind);
Node_t * mpow(Node_t * node, int ind);
Node_t * osn(Node_t * node, int ind);
Node_t * st(Node_t * node, int ind);
Node_t * sm(Node_t * node, int ind);

#endif // DIFFERENTIATION_H
