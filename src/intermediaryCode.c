#include "intermediaryCode.h"
#include "utils.h"
#include "string.h"

char* _make3AddrCode(ParserNode* parser_ast);
char* make3AddrCode(ParserNode* parser_ast) {
  return concatStr(
    printFunctionDeclarations(parser_ast),
    _make3AddrCode(parser_ast)
  );
}
char* _make3AddrCode(ParserNode* parser_ast) {

  if (parser_ast == NULL) return NULL;

  char* nodeClass = parser_ast->astNodeClass;
  if (!strcmp(nodeClass, "COMMANDS_LIST")) {
    // printf("types|%s|\n", parser_ast->leftBranch->astNodeClass);
    char* firstCommand = _make3AddrCode(parser_ast->leftBranch);
    char* rest = _make3AddrCode(parser_ast->rightBranch);

    return concatStr(firstCommand, rest);
  } else if (!strcmp(nodeClass, "FUNCTION_DECL")) {
    char* warningStart = concatStr("// function declaration of \"", parser_ast->value);
    char* commentWarning = concatStr(warningStart, "\" was here\n");
    free(warningStart);
    return commentWarning;
  } else {
    char* left = _make3AddrCode(parser_ast->leftBranch);
    char* middle = _make3AddrCode(parser_ast->middleBranch);
    char* right = _make3AddrCode(parser_ast->rightBranch);
    char* temp1 = concatStr(left, middle);
    char* result = concatStr(temp1, right);
    // free(left);
    // free(middle);
    // free(right);
    // free(temp1);
    return result;
  }

  return concatStr("TEST ", "2\n");
}

NodeList* concatLists(NodeList* l1, NodeList* l2) {
  if (l1 == NULL) return l2;
  if (l2 == NULL) return l1;

  NodeList* l1Tail = l1;
  while (l1Tail->next != NULL) l1Tail = l1Tail->next;

  l1Tail->next = l2;
  return l1;
}

NodeList* getFunctionDeclarations(ParserNode* root) {
  if (root == NULL) return NULL;

  NodeList* leftFunctions = getFunctionDeclarations(root->leftBranch);
  NodeList* middleFunctions = getFunctionDeclarations(root->middleBranch);
  NodeList* rightFunctions = getFunctionDeclarations(root->rightBranch);

  NodeList* subtreeFunctions = concatLists(leftFunctions, middleFunctions);
  subtreeFunctions = concatLists(subtreeFunctions, rightFunctions);
  
  if (!strcmp(root->astNodeClass, "FUNCTION_DECL")) {
    NodeList* newHead = (NodeList*) malloc(sizeof(NodeList));
    newHead->node = root;
    newHead->next = subtreeFunctions;
    return newHead;
  }

  return subtreeFunctions;
}

char* printFunctionDeclarations(ParserNode* root) {
  NodeList* functionDeclarations = getFunctionDeclarations(root);

  char* acc = "";
  while (functionDeclarations != NULL) {
    acc = concatStrs(2, acc, translateFunctionDeclaration(functionDeclarations->node));
    functionDeclarations = functionDeclarations->next;
  }

  return concatStr(acc, "\n\n");
}

char* translateFunctionDeclaration(ParserNode* foo) {
  return concatStrs(3,
    "// definition of ",
    foo->value,
    "\n"
  );
}
