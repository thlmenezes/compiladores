#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include "uthash.h" /* https://troydhanson.github.io/uthash/userguide.html#_hash_operations */

enum SymbolType
{
  enumFunction = 'F',
  enumVariable = 'V',
  enumParameter = 'P'
};

// comentei alguns atributos pq não entendi a utilidade deles ainda. quando forem necessários só descomentar.
typedef struct SymbolData
{
  int symbolID;
  char symbolType; /* F para funcao, V para variavel e P para paramentro */
  char *type;      /* tipo do ID (int, float, etc) */
  char *name;      /* nome literal */

  int scopeID;
  // nivel de escopo do simbolo no programa (0 é o nivel mais alto, 1 se tiver em um if especifico, e etc)
  int scopeLevel;

  // if param
  char *associatedFunction; // nome da função da qual o parametro pertence (se for parametro)
  int associatedFunctionScopeId;
  // int paramIndex; // número de qual parametro da função esse é
} SymbolData;

typedef struct Symbol
{
  int symbolID;
  char symbolType; /* F para funcao, V para variavel e P para paramentro */
  char *type;      /* tipo do ID (int, float, etc) */
  char *name;      /* nome literal */

  int scopeID;
  // nivel de escopo do simbolo no programa (0 é o nivel mais alto, 1 se tiver em um if especifico, e etc)
  int scopeLevel;

  // if function
  int params_id_list[253];
  int last_param;

  // if param
  char *associatedFunction;
  int associatedFunctionScopeId;

  // chave de hash
  UT_hash_handle hh;
} Symbol;

// global symbol table
Symbol *symbolTable;

void addSymbol(SymbolData newSymbolData);
void printSymbolTable();
int symbolExists(char* symbolName);

// SCOPE
typedef struct ScopeStack {
  int level; /* numeric representation of scope level */
  int scopeID; /* unique ID of scope */
  struct ScopeStack *next;
} ScopeStack; /* element of each scope */

typedef struct ScopeInfo {
  int level; /* numeric representation of scope level */
  int scopeID; /* unique ID of scope */
} ScopeInfo;

/* returns the scope stack head, that is the current scope */
ScopeInfo get_current_scope();

/* add one level to current scope counting */
void create_new_scope_level();

/* removes one level of scope stack */
void decrease_scope_level();

#endif // SYMBOL_TABLE_
