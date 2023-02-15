#include "callStack.h"

CallStackItems* makeStack() {
  // return malloc(sizeof(CallStackItems));
  CallStackItems* stack = malloc(sizeof(CallStackItems));
  stack->next = NULL;
  stack->varCount = 0;
  stack->vars = NULL;
  return stack;
}

CallStackItems* pushContext(CallStackItems* stack, char** vars, int varCount) {
  CallStackItems* newHead = malloc(sizeof(CallStackItems));
  newHead->varCount = varCount;
  newHead->vars = vars;
  newHead->next = stack;

  return newHead;
}

char* pushContextStr(CallStackItems* stack) {
  char* str = "";
  for (int i = 0; i < stack->varCount; i++) {
    // char* tmp = concatStrs(2, str, "push ", stack->vars[i], "\n");
    // free(str);
    // str = tmp;

    str = concatStrs(4, str, "push ", stack->vars[i], "\n");
  }

  return str;
}

char* popContextStr(CallStackItems* stack) {
  char* str = "";
  for (int i = stack->varCount - 1; i >= 0; i--) {
    str = concatStrs(4, str, "pop ", stack->vars[i], "\n");
  }

  return str;
}

CallStackItems* popContext(CallStackItems* stack) {
  CallStackItems* tmp = stack->next;
  free(stack);
  return tmp;
}
