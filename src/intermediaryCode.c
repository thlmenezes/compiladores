#include "intermediaryCode.h"
#include "utils.h"
#include "string.h"

char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter, int* loopCounter);
char* make3AddrCode(ParserNode* parser_ast) {
  int varCounter = 0;
  int loopCounter = 0;
  return concatStr(
    printFunctionDeclarations(parser_ast),
    _make3AddrCode(parser_ast, &varCounter, loopCounter)
  );
}

int isTerminalExpression(ParserNode* exprNode) {
  return (!strcmp(exprNode->astNodeClass, "LIT_INT")) || (!strcmp(exprNode->astNodeClass, "USE_VAR_EXP"));
}

char* cleanUpTerminal(ParserNode* terminalExpNode) {
  if (!strcmp(terminalExpNode->astNodeClass, "LIT_INT")) return intToStr(parseInt(terminalExpNode->value));
  return copyString(terminalExpNode->value);
}

char* subExpression3Add(ParserNode* subExprNode, int* tempVarCounter, int* loopCounter) {
  if (!isTerminalExpression(subExprNode)) {
    return _make3AddrCode(subExprNode, tempVarCounter, loopCounter);
  }
  *tempVarCounter += 1;
  return concatStrs(5, "t", intToStr(*tempVarCounter - 1), " = ", cleanUpTerminal(subExprNode), "\n");
}

char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter, int* loopCounter) {

  if (parser_ast == NULL) return "";

  char* nodeClass = parser_ast->astNodeClass;
  if (!strcmp(nodeClass, "COMMANDS_LIST")) {
    // printf("types|%s|\n", parser_ast->leftBranch->astNodeClass);
    char* firstCommand = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter);
    char* rest = _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter);

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

    char* subExprL = subExpression3Add(parser_ast->leftBranch, tempVarCounter, loopCounter);
    int subExprLResultVar = *tempVarCounter - 1;
    char* subExprR = subExpression3Add(parser_ast->rightBranch, tempVarCounter, loopCounter);
    int subExprRResultVar = *tempVarCounter - 1;

    char temp[30];
    sprintf(temp, "t%d = t%d %s t%d\n", *tempVarCounter, subExprLResultVar, parser_ast->value, subExprRResultVar);
    *tempVarCounter += 1;

    return concatStrs(6,
      "// calculate sub-expr L\n",
      subExprL, 
      "// calculate sub-expr R\n",
      subExprR,
      "// operate on sub_exp results\n",
      temp
    );

  } else if (!strcmp(nodeClass, "DECLARE_VARIABLE")) {
    char* assignValue = isTerminalExpression(parser_ast->leftBranch)
      ? cleanUpTerminal(parser_ast->leftBranch)
      : concatStrs(2, "t", intToStr(*tempVarCounter));

    return concatStrs(5,
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter),
      parser_ast->value, " = ", assignValue, "\n"
    );
  } else if (!strcmp(nodeClass, "READ_COMMAND")) {
    return concatStrs(3,
      "read ", parser_ast->value, "\n"
    );
  } else if (!strcmp(nodeClass, "LOG_COMMAND")) {
    return concatStrs(4,
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter),
      "log t", intToStr(*tempVarCounter), "\n"
    );
  } else if (!strcmp(nodeClass, "RETURN_CMD")) {
    char* loadResultExprResultToReg;
    int returnReg = *tempVarCounter;
    if (!strcmp(parser_ast->leftBranch->astNodeClass, "LIT_INT")) {
      loadResultExprResultToReg = concatStrs(5,
        "t", intToStr(*tempVarCounter), " = ", cleanUpTerminal(parser_ast->leftBranch), "\n"
      );
      *tempVarCounter += 1;
    } else {
      loadResultExprResultToReg = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter);
    }

    return concatStrs(4,
      loadResultExprResultToReg,
      "return t", intToStr(returnReg), "\n"
    );

  } else if (!strcmp(nodeClass, "FUNC_CALL")) {
    return "// CHAMANDO FUNÇÃO\n";
  } else if (!strcmp(nodeClass, "USE_VAR_EXP")) {
    return concatStrs(5, "t", intToStr(*tempVarCounter), " = ", parser_ast->value, "\n");
  } else if (!strcmp(nodeClass, "WHILE")) {
    char* checkCondition = concatStrs(19,
      "// start of while loop\n",
      "WHILE_LOOP_", intToStr(loopCounter++), ":\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter),
      "// jump to end if condition == 0\n",
      "jeqz t", intToStr(*tempVarCounter), " WHILE_LOOP_END_", intToStr(loopCounter),"\n",
      "// start of while loop body\n",
      _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter),
      "j WHILE_LOOP_", intToStr(loopCounter), "\n",
      "WHILE_LOOP_END_", intToStr(loopCounter), ":\n"
    );
    return checkCondition;
  } else if (!strcmp(nodeClass, "IF_THEN")) {
    return concatStrs(13,
      "// if condition\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter),
      "// jump over then block if condition == 0\n",
      "jeqz t", intToStr(*tempVarCounter),  " IF_THEN_END_", intToStr(loopCounter++), "\n",
      "// start of then block\n",
      _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter),
      "IF_THEN_END_", intToStr(loopCounter), ":\n"
    );

  } else if (!strcmp(nodeClass, "ASSIGN_VAR")) {
    char temp[200];
    sprintf(temp, "// value to insert in \"%s\" is in \"t%d\"\n", parser_ast->value, *tempVarCounter);
    char* calcVarValue = concatStrs(12,
      "// start calculating value to insert in var \"", parser_ast->value, "\"\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter),
      "// end calculating value to insert in var \"", parser_ast->value, "\"\n", temp,
      parser_ast->value, " = t", intToStr(*tempVarCounter),"\n"
    );
    // *tempVarCounter += 1;
    return calcVarValue;
  } else {
    char* left = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter);
    char* middle = _make3AddrCode(parser_ast->middleBranch, tempVarCounter, loopCounter);
    char* right = _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter);
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
