#include "defines.h"
#include <string.h>
#include <stdlib.h>

char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    printf("Out of memory error!\n");
  else strcpy(t,s);

  // printf("copying \"%s\"\n", t);
  return t;
}

int gen_random_uniqueID() {
  int result = 0, low_num = 10000, hi_num = 89999;

  result = (rand() % hi_num) + low_num;
  return result;
}

int parseInt(char* s) {
  // TODO: implementar
  return 88;
}
