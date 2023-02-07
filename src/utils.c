#include "defines.h"

char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    printf("Out of memory error at line %d\n");
  else strcpy(t,s);

  printf("copying \"%s\"\n", t);
  return t;
}
