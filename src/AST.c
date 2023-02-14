#include "AST.h"
#include "utils.h"
#include <stdlib.h>
#include "defines.h"

parserNode* add_ast_node(astParam astParam) {
  parserNode *node = (parserNode *)calloc(1, sizeof(parserNode));

  // switch (astParam.nodeType) {}
  return node;
}

parserNode* createLiteralIntNode(char* numStr) {
  parserNode *node = (parserNode *)calloc(1, sizeof(parserNode));
  
  node->leftBranch = NULL;
  node->middleBranch = NULL;
  node->rightBranch = NULL;

  node->value = copyString(numStr);
  node->type = LITERAL_INT_TYPE;

  return node;
}

void printLiteralIntNode(parserNode* node) {
  syn_print("TEST. value %s. type: %s\n", node->value, node->type);
}
