#ifndef GET_TREE_H
#define GET_TREE_H

TreeErr getG(char * s, Tree ** tree);
Node_t * getE(char ** s, char ** vars);
Node_t * getT(char ** s, char ** vars);
Node_t * getP(char ** s, char ** vars);
Node_t * getN(char ** s);
Node_t * getF(char ** s, char ** vars);
Node_t * getV(char ** s, char ** vars);

#endif
