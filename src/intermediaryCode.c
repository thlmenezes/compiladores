#include "intermediaryCode.h"
#include "utils.h"
#include "symbol_table.h"
#include "string.h"
#include "callStack.h"

int getVarCount(Symbol* sTable) {
  Symbol* current = sTable;
  int varCount = 0;
  while (current != NULL) {
    if (current->symbolType == enumVariable)
      varCount++;
    current = (Symbol*) current->hh.next;
  }

  return varCount;
}

char** makeContextList(Symbol* sTable, int tempVarCount) {
  int varCount = getVarCount(sTable);
  char** stringArr = malloc((varCount + tempVarCount) * sizeof(char*));
  int curString = 0;
  Symbol* current = sTable;
  while (current != NULL) {
    if (current->symbolType == enumVariable) {
      stringArr[curString] = current->name;
      // printf("saving var %s (%d)\n", stringArr[curString], curString);
    }
    current = (Symbol*) current->hh.next;
  }

  for (int tVar = 0; tVar < tempVarCount; tVar++) {
    stringArr[varCount + tVar] = concatStr("t", intToStr(tVar));
    // printf("saving tVar |%s| (%d)\n", stringArr[varCount + tVar], varCount + tVar);
  }

  return stringArr;
}

char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter, int* loopCounter, CallStackItems* callStack);
char* make3AddrCode(ParserNode* parser_ast) {
  int varCounter = 0;
  int loopCounter = 0;
  CallStackItems* tmp;
  CallStackItems* callStack = makeStack();
  int count;
  STACK_COUNT(callStack, tmp, count);

  return concatStr(
    printFunctionDeclarations(parser_ast),
    _make3AddrCode(parser_ast, &varCounter, &loopCounter, callStack)
  );
}

int isTerminalExpression(ParserNode* exprNode) {
  return (!strcmp(exprNode->astNodeClass, "LIT_INT")) || (!strcmp(exprNode->astNodeClass, "USE_VAR_EXP"));
}

char* cleanUpTerminal(ParserNode* terminalExpNode) {
  if (!strcmp(terminalExpNode->astNodeClass, "LIT_INT")) return intToStr(parseInt(terminalExpNode->value));
  return copyString(terminalExpNode->value);
}

char* subExpression3Add(ParserNode* subExprNode, int* tempVarCounter, int* loopCounter, CallStackItems* callStack) {
  if (!isTerminalExpression(subExprNode)) {
    return _make3AddrCode(subExprNode, tempVarCounter, loopCounter, callStack);
  }
  *tempVarCounter += 1;
  return concatStrs(5, "t", intToStr(*tempVarCounter - 1), " = ", cleanUpTerminal(subExprNode), "\n");
}

char* _make3AddrCode(ParserNode* parser_ast, int* tempVarCounter, int* loopCounter, CallStackItems* callStack) {

  if (parser_ast == NULL) return "";

  char* nodeClass = parser_ast->astNodeClass;
  if (!strcmp(nodeClass, "COMMANDS_LIST")) {
    // printf("types|%s|\n", parser_ast->leftBranch->astNodeClass);
    char* firstCommand = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack);
    char* rest = _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter, callStack);

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

    char* subExprL = subExpression3Add(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack);
    int subExprLResultVar = *tempVarCounter - 1;
    char* subExprR = subExpression3Add(parser_ast->rightBranch, tempVarCounter, loopCounter, callStack);
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
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
      parser_ast->value, " = ", assignValue, "\n"
    );
  } else if (!strcmp(nodeClass, "READ_COMMAND")) {
    return concatStrs(3,
      "read ", parser_ast->value, "\n"
    );
  } else if (!strcmp(nodeClass, "LOG_COMMAND")) {
    return concatStrs(4,
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
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
      loadResultExprResultToReg = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack);
    }

    return concatStrs(4,
      loadResultExprResultToReg,
      "//TODO: pop parâmetros da função\n",
      "return t", intToStr(returnReg), "\n"
    );

  } else if (!strcmp(nodeClass, "FUNC_CALL")) {
    callStack = pushContext(callStack, makeContextList(symbolTable, *tempVarCounter), *tempVarCounter + getVarCount(symbolTable));
    char* pushContextString = pushContextStr(callStack);
    char* functionReturnLabel = concatStrs(4,
      "CALL_", parser_ast->value, "_", intToStr(*loopCounter)
    );
    *loopCounter += 1;

    Symbol* funcSymbol = getSymbol(parser_ast->value);

    // printf("param total %d |%s|", funcSymbol->last_param,getSymbolById(funcSymbol->params_id_list[0])->name);

    char* popContextString = popContextStr(callStack);
    callStack = popContext(callStack);
    return concatStrs(20,
      "// SALVA CONTEXTO\n",
      pushContextString,
      "// CARREGA ENDEREÇO DE RETORNO\n",
      "push ", functionReturnLabel, "\n",
      "// CARREGA QUANTIDADE DE PARAMETROS\n",
      "push ", intToStr(funcSymbol->last_param), "\n",
      "// CARREGA PARAMETROS\n",
      "// TODO: carregar parametros\n",
      // _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
      "// JUMP PRA FUNÇÃO\n",
      "jump FUNCTION_", funcSymbol->name, "\n",
      functionReturnLabel, ":\n",
      "// POP CONTEXTO DEPOIS DO RETORNO\n",
      popContextString
    );
  } else if (!strcmp(nodeClass, "USE_VAR_EXP")) {
    return concatStrs(5, "t", intToStr(*tempVarCounter), " = ", parser_ast->value, "\n");
  } else if (!strcmp(nodeClass, "WHILE")) {
    char* checkCondition = concatStrs(19,
      "// start of while loop\n",
      "WHILE_LOOP_", intToStr(*loopCounter++), ":\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
      "// jump to end if condition == 0\n",
      "jeqz t", intToStr(*tempVarCounter), " WHILE_LOOP_END_", intToStr(*loopCounter),"\n",
      "// start of while loop body\n",
      _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter, callStack),
      "j WHILE_LOOP_", intToStr(*loopCounter), "\n",
      "WHILE_LOOP_END_", intToStr(*loopCounter), ":\n"
    );
    return checkCondition;
  } else if (!strcmp(nodeClass, "IF_THEN")) {
    return concatStrs(13,
      "// if condition\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
      "// jump over then block if condition == 0\n",
      "jeqz t", intToStr(*tempVarCounter),  " IF_THEN_END_", intToStr(loopCounter++), "\n",
      "// start of then block\n",
      _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter, callStack),
      "IF_THEN_END_", intToStr(loopCounter), ":\n"
    );

  } else if (!strcmp(nodeClass, "ASSIGN_VAR")) {
    char temp[200];
    sprintf(temp, "// value to insert in \"%s\" is in \"t%d\"\n", parser_ast->value, *tempVarCounter);
    char* calcVarValue = concatStrs(12,
      "// start calculating value to insert in var \"", parser_ast->value, "\"\n",
      _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack),
      "// end calculating value to insert in var \"", parser_ast->value, "\"\n", temp,
      parser_ast->value, " = t", intToStr(*tempVarCounter),"\n"
    );
    // *tempVarCounter += 1;
    return calcVarValue;
  } else {
    char* left = _make3AddrCode(parser_ast->leftBranch, tempVarCounter, loopCounter, callStack);
    char* middle = _make3AddrCode(parser_ast->middleBranch, tempVarCounter, loopCounter, callStack);
    char* right = _make3AddrCode(parser_ast->rightBranch, tempVarCounter, loopCounter, callStack);
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

  return concatStr(acc, "\n");
}

char* translateFunctionDeclaration(ParserNode* foo) {
  return concatStrs(7,
    "// definition of ", foo->value, " goes here\n",
    "FUNCTION_", foo->value, ":\n",
    "// TODO: pegar corpo da função e incluir aqui\n"
  );
}
