#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include "uthash.h" /* https://troydhanson.github.io/uthash/userguide.html#_hash_operations */

enum symbolType {
  enumFunction = 'F',
  enumVariable = 'V',
  enumParameter = 'P' 
};

typedef struct symbol {
    int symbolID;
    char symbolType; /* F para funcao, V para variavel e P para paramentro */
    char *type; /* tipo do ID (int, float, etc) */
    char *name; /* nome literal */
    int scope; /* nivel de escopo do simbolo no programa (0 é o nivel mais alto, 1 se tiver em um if especifico, e etc) */
    int scopeID; /* ID do scopo */
    int params_list[253]; /* lista de param. se for uma funcao(253 = maximum amount of params in C specification) */
    UT_hash_handle hh; /* hash de terceiros que sera util */
}symbol;

#endif // SYMBOL_TABLE_