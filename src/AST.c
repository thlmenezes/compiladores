#include "AST.h"

parserNode* add_ast_node(astParam astParam) {
  parserNode *node = (parserNode *)calloc(1, sizeof(parserNode));

  switch (astParam.nodeType) {
    case enumLeftRightMiddleBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = astParam.middleBranch;
      node->rightBranch = astParam.rightBranch;
      // node->astNodeClass = astParam.astNodeClass;
      node->value = astParam.value;
      node->type = astParam.type;
      break;
    case enumLeftRightBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = NULL;
      node->rightBranch = astParam.rightBranch;
      // node->astNodeClass = astParam.astNodeClass;
      node->value = NULL;
      node->type = NULL;
      break;
    case enumLeftRightValueBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = NULL;
      node->rightBranch = astParam.rightBranch;
      // node->astNodeClass = astParam.astNodeClass;
      node->value = astParam.value;
      node->type = astParam.type;
      break;
    case enumValueLeftBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = NULL;
      node->rightBranch = NULL;
      // node->astNodeClass = astParam.astNodeClass;
      node->value = astParam.value;
      node->type = astParam.type;
      break;
    case enumValueTypeOnly:
      node->leftBranch = NULL;
      node->middleBranch = NULL;
      node->rightBranch = NULL;
      // node->astNodeClass = astParam.astNodeClass;
      node->value = astParam.value;
      node->type = astParam.type;
      break;
    default:
      break;
  }
  return node;
}

parserNode* createLiteralIntNode(char* numStr) {
  return NULL;
}
