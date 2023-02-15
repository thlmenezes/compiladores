#include "defines.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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

  char ret[30] = "";
  int counter = 0, i;
  int newInt;

  for (i = 0; s[i] != '\0'; i++){
    if (s[i] != ' '){
      ret[counter] = s[i];
      counter++;
    }
  }

  newInt = atoi(ret);

  return newInt;

}

char* concatStr(char* str1, char* str2) {
  if (!str1 && !str2) return "";
  if (str1 == NULL) return str2;
  if (str2 == NULL) return str1;

  char* result = malloc(strlen(str1) + strlen(str2) + 1);

  strcpy(result, str1);
  strcpy((char*) (result + strlen(str1)), str2);
 
  //! DANGEROUS. IF THINGS GO WRONG COMMENT THIS OUT AND JUST LET MEMORY LEAKS HAPPEN
  // free(str1);
  // free(str2);

  return result;
}

char* concatStrs(int n_args, ...) {
  va_list args;
  va_start(args, n_args);

  char** strs = malloc(sizeof(char*) * n_args);
  int* strSizes = malloc(sizeof(int) * n_args);
  int strSize = 1;
  for (int count = 0; count < n_args; count++) {
    strs[count] = va_arg(args, char*);
    strSizes[count] = strlen(strs[count]);
    strSize += strSizes[count];
  }

  char* result = malloc(strSize * sizeof(char));

  for (int offset = 0, i = 0; i < n_args; offset += strSizes[i], i++) {
    if (strs[i] == NULL) continue;
    strcpy(result + offset, strs[i]);
  }

  va_end(args);

  free(strs);
  free(strSizes);
  return result;
}
