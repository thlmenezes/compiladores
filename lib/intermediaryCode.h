#include "AST.h"

char* make3AddrCode(ParserNode* parser_ast);

typedef struct NodeList {
  ParserNode* node;
  struct NodeList* next;
} NodeList;

NodeList* getFunctionDeclarations(ParserNode* root);
char* printFunctionDeclarations(ParserNode* root);
char* translateFunctionDeclaration(ParserNode* foo);
int isTerminalExpression(ParserNode* exprNode);
