#include "intermediaryCode.h"
#include "utils.h"
#include "string.h"

char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter);
char* make3AddrCode(ParserNode* parser_ast) {
  int varCounter = 0;
  return concatStr(
    printFunctionDeclarations(parser_ast),
    _make3AddrCode(parser_ast, &varCounter)
  );
}

int isTerminalExpression(ParserNode* exprNode) {
  return (!strcmp(exprNode->astNodeClass, "LIT_INT")) || (!strcmp(exprNode->astNodeClass, "USE_VAR_EXP"));
}

char* cleanUpTerminal(ParserNode* terminalExpNode) {
  if (!strcmp(terminalExpNode->astNodeClass, "LIT_INT")) return intToStr(parseInt(terminalExpNode->value));
  return copyString(terminalExpNode->value);
}

static int loopCounter = 0;
char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter) {

  if (parser_ast == NULL) return NULL;

  char* nodeClass = parser_ast->astNodeClass;
  if (!strcmp(nodeClass, "COMMANDS_LIST")) {
    // printf("types|%s|\n", parser_ast->leftBranch->astNodeClass);
    char* firstCommand = _make3AddrCode(parser_ast->leftBranch, tempVarCounter);
    char* rest = _make3AddrCode(parser_ast->rightBranch, tempVarCounter);

    return concatStr(firstCommand, rest);
  } else if (!strcmp(nodeClass, "FUNCTION_DECL")) {
    char* warningStart = concatStr("// function declaration of \"", parser_ast->value);
    char* commentWarning = concatStr(warningStart, "\" was here\n");
    free(warningStart);
    return commentWarning;
  } else if (!strcmp(nodeClass, "MATH_EXPRESSION")) {
    if (isTerminalExpression(parser_ast->leftBranch) && isTerminalExpression(parser_ast->rightBranch)) {
      char temp[100];
      sprintf(temp, "t%d = %s %s %s", *tempVarCounter, cleanUpTerminal(parser_ast->leftBranch), parser_ast->value, cleanUpTerminal(parser_ast->rightBranch));
      *tempVarCounter += 1;
      return concatStrs(2, temp, "\n");
    }
  } else if (!strcmp(nodeClass, "WHILE")) {
    char* checkCondition = concatStrs(15,
      "// start of while loop\n",
      "WHILE_LOOP_", intToStr(loopCounter++), ":\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter),
      "// jump to end if condition == 0\n",
      "jeq0 t", intToStr(*tempVarCounter), "\n",
      "// start of while loop body\n",
      _make3AddrCode(parser_ast->rightBranch, tempVarCounter),
      "// jump to start of while loop\n",
      "j WHILE_LOOP_", intToStr(loopCounter), "\n"
    );
    return checkCondition;
  } else if (!strcmp(nodeClass, "ASSIGN_VAR")) {
    char temp[200];
    sprintf(temp, "// value to insert in var is in \"t%d\"\n", *tempVarCounter);
    char* calcVarValue = concatStrs(12,
      "// start calculating value to insert in var \"", parser_ast->value, "\"\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter),
      "// end calculating value to insert in var \"", parser_ast->value, "\"\n", temp,
      parser_ast->value, " = t", intToStr(*tempVarCounter),"\n"
    );
    // *tempVarCounter += 1;
    return calcVarValue;
  } else {
    char* left = _make3AddrCode(parser_ast->leftBranch, tempVarCounter);
    char* middle = _make3AddrCode(parser_ast->middleBranch, tempVarCounter);
    char* right = _make3AddrCode(parser_ast->rightBranch, tempVarCounter);
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
