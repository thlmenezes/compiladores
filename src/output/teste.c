#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parseInt(char* s) {

  char ret[30] = "";
  int counter = 0, i;
  int newInt;

  for (i = 0; s[i] != '\0'; i++){
    if (s[i] != ' '){
      s[counter] = s[i];
      counter++;
    }
  }

  newInt = atoi(ret);

  return newInt;

}

int main () {
  char* send;
  strcpy(send, "- 4");

  int check = parseInt(send);
  printf("recebi %d\n", check);

  return 0;
}