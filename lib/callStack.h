#include "ustack.h"
#include "utils.h"
#include "defines.h"

typedef struct CallStackItems {
  char** vars;
  int varCount;
  struct CallStackItems* next;
} CallStackItems;

CallStackItems* makeStack();
CallStackItems* pushContext(CallStackItems* stack, char** vars, int varCount);
char* pushContextStr(CallStackItems* stack);
char* popContextStr(CallStackItems* stack);
CallStackItems* popContext(CallStackItems* stack);
