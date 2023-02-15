#include "AST.h"
#include "utils.h"
#include <stdlib.h>
#include "defines.h"
#include "types.h"

ParserNode* add_ast_node(AstParam astParam) {
  ParserNode *node = (ParserNode *)calloc(1, sizeof(ParserNode));

  switch (astParam.nodeType) {
    case enumLeftRightMiddleBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = astParam.middleBranch;
      node->rightBranch = astParam.rightBranch;
      node->astNodeClass = copyString(astParam.astNodeClass);
      node->value = copyString(astParam.value);
      node->type = astParam.type;
      break;
    case enumLeftRightBranch:
      node->leftBranch = astParam.leftBranch;
      node->rightBranch = astParam.rightBranch;
      node->astNodeClass = copyString(astParam.astNodeClass);
      node->value = astParam.value;
      node->type = astParam.type;
      break;
    case enumValueLeftBranch:
      node->leftBranch = astParam.leftBranch;
      node->middleBranch = NULL;
      node->rightBranch = NULL;
      node->astNodeClass = copyString(astParam.astNodeClass);
      node->value = copyString(astParam.value);
      node->type = astParam.type;
      break;
    case enumValueTypeOnly:
      node->leftBranch = NULL;
      node->rightBranch = NULL;
      node->astNodeClass = copyString(astParam.astNodeClass);
      node->value = copyString(astParam.value);
      node->type = astParam.type;
      break;
    default:
      break;
  }
  return node;
}

ParserNode* createLiteralIntNode(char* numStr) {
  ParserNode *node = (ParserNode *)calloc(1, sizeof(ParserNode));
  
  node->leftBranch = NULL;
  node->middleBranch = NULL;
  node->rightBranch = NULL;

  node->value = copyString(numStr);
  node->type = INT_TYPE;
  node->astNodeClass = "LIT_INT";

  return node;
}

void print_parser_ast(ParserNode *node, int level, Side side) {
  if (node)
  {
      for (int aux = level; aux > 0; aux--)
      {
        printf("%s", "~");
      }
      printf("> ");
      if (side == left) printf("(L) ");
      if (side == middle) printf("(M) ");
      if (side == right) printf("(R) ");

      printf("class: %s - ", node->astNodeClass);
      if (node->type != NULL)
      {
        printf("type: %s - ", node->type);
      }
      if (node->value != NULL)
      {
        printf("value: \"%s\" - ", node->value);
      }
      {
        // printf("CAST: %s - ", node->cast);
      }
      printf("\n");
      print_parser_ast(node->leftBranch, level + 1, left);
      if (node->middleBranch != NULL) print_parser_ast(node->middleBranch, level + 1, middle);
      print_parser_ast(node->rightBranch, level + 1, right);
  }
}

void printLiteralIntNode(ParserNode* node) {
  syn_print("TEST. value %s. type: %s\n", node->value, node->type);
}
