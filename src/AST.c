#include "AST.h"
#include "utils.h"
#include <stdlib.h>
#include "defines.h"

parserNode* add_ast_node(AstParam AstParam) {
  parserNode *node = (parserNode *)calloc(1, sizeof(parserNode));

  switch (AstParam.nodeType) {
    case enumLeftRightMiddleBranch:
      node->leftBranch = AstParam.leftBranch;
      node->middleBranch = AstParam.middleBranch;
      node->rightBranch = AstParam.rightBranch;
      // node->astNodeClass = AstParam.astNodeClass;
      node->value = AstParam.value;
      node->type = AstParam.type;
      break;
    case enumLeftRightBranch:
      node->leftBranch = AstParam.leftBranch;
      node->rightBranch = AstParam.rightBranch;
      node->astNodeClass = AstParam.astNodeClass;
      node->value = NULL;
      node->type = NULL;
      break;
    case enumValueLeftBranch:
      node->leftBranch = AstParam.leftBranch;
      node->middleBranch = NULL;
      node->rightBranch = NULL;
      node->astNodeClass = AstParam.astNodeClass;
      node->value = AstParam.value;
      node->type = AstParam.type;
      break;
    case enumValueTypeOnly:
      node->leftBranch = NULL;
      node->rightBranch = NULL;
      node->astNodeClass = AstParam.astNodeClass;
      node->value = AstParam.value;
      node->type = AstParam.type;
      break;
    default:
      break;
  }
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

void print_parser_ast(parserNode *node, int level)
{
  if (node)
  {
      for (int aux = level; aux > 0; aux--)
      {
        printf("%s", "~");
      }
      printf("> ");
      printf("class: %s - ", node->astNodeClass);
      if (node->type != NULL)
      {
        printf("type: %s - ", node->type);
      }
      if (node->value != NULL)
      {
        printf("value: %s - ", node->value);
      }
      {
        // printf("CAST: %s - ", node->cast);
      }
      printf("\n");
      print_parser_ast(node->leftBranch, level + 1);
      print_parser_ast(node->rightBranch, level + 1);
  }
}

void printLiteralIntNode(parserNode* node) {
  syn_print("TEST. value %s. type: %s\n", node->value, node->type);
}
