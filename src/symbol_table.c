#include <stdio.h>
#include "defines.h"
#include "utils.h"
#include "ustack.h"
#include "symbol_table.h"

Symbol* symbolTable = NULL;

ScopeStack* scopeStackHead = NULL;

void addSymbol(SymbolData newSymbolData) {
  Symbol* newSymbolPtr;
  HASH_FIND_INT(symbolTable, &newSymbolData.symbolID, newSymbolPtr);
  // printf("addSymbol recieved \"%s\"\n", newSymbolData.name);

  if (newSymbolPtr != NULL) {
    printf("symbol %s already declared!\n", newSymbolData.name);
    return;
  }

  // se está adicionando um parametro a uma função, atualize a função
  if (newSymbolData.symbolType == enumParameter) {
    Symbol *func;

    for (func = symbolTable; func != (Symbol*) NULL; func = (Symbol*)(func -> hh.next)) {
      // printf("AAAAAA |%s||%s|\n", func->name, newSymbolData.associatedFunction);
      if (func->name == newSymbolData.associatedFunction && func->scopeID == newSymbolData.associatedFunctionScopeId) {
        func->last_param++;
        func->params_id_list[func->last_param] = newSymbolData.symbolID;
      }
    }
  }

  // cria o novo elemento da tabela e adiciona os dados
  newSymbolPtr = (Symbol*)malloc(sizeof(Symbol));
  newSymbolPtr->symbolID = newSymbolData.symbolID;
  newSymbolPtr->type = newSymbolData.type;
  newSymbolPtr->symbolType = newSymbolData.symbolType;
  newSymbolPtr->name = newSymbolData.name;
  
  // ScopeInfo current_scope = get_current_scope();
  newSymbolPtr->scopeLevel = newSymbolData.scopeLevel;
  newSymbolPtr->scopeID = newSymbolData.scopeID;
  
  // newSymbolPtr->tacName = newSymbolData.tacName;
  // newSymbolPtr->line = newSymbolData.line;
  // newSymbolPtr->column = newSymbolData.column;
  // newSymbolPtr->params_id_list[0] = -1;
  newSymbolPtr->last_param = -1;

  newSymbolPtr->associatedFunction = newSymbolData.associatedFunction;    

  // adiciona o novo elemento na tabela
  HASH_ADD_INT(symbolTable, symbolID, newSymbolPtr);
}

void printSymbol(Symbol* symbol) {
  if (symbol->symbolType == enumParameter) {
    printf("\t- ID: %d. func %s. symbol name: \"%s\". type: \"%s\". kind: \"%s\". scopeLvl: %i. scope: %i\n", symbol->symbolID, symbol->associatedFunction, symbol->name, symbol->type, &symbol->symbolType, symbol->scopeLevel, symbol->scopeID);
  } else if (symbol->symbolType == enumFunction) {
    printf("\t- ID: %d. params %d. symbol name: \"%s\". type: \"%s\". kind: \"%s\". scopeLvl: %i. scope: %i\n", symbol->symbolID, symbol->last_param, symbol->name, symbol->type, &symbol->symbolType, symbol->scopeLevel, symbol->scopeID);
  } else {
    printf("\t- ID: %d. symbol name: \"%s\". type: \"%s\". kind: \"%s\". scopeLvl: %i. scope: %i\n", symbol->symbolID, symbol->name, symbol->type, &symbol->symbolType, symbol->scopeLevel, symbol->scopeID);
  }
}

void printSymbolTable() {
  printf("==============================================================\n");
  printf("symbol table:\n");
  
  Symbol* current = symbolTable;
  while (current != NULL) {
    printSymbol(current);
    current = (Symbol*) current->hh.next;
  }
  printf("==============================================================\n");
}

Symbol* getSymbolById(int id) {
  for (Symbol* current = symbolTable; current != NULL; current = (Symbol*) current->hh.next) {
    if (id == current->symbolID) {
      return current;
    }
  }

  return NULL;
}
Symbol* getSymbol(char* symbolName) {
  for (Symbol* current = symbolTable; current != NULL; current = (Symbol*) current->hh.next) {
    if (strcmp(symbolName, current->name) == 0) {
      return current;
    }
  }

  return NULL;
}

int symbolExists(char* symbolName) {
  int exists = FALSE;

  Symbol* current = symbolTable;
  while (current != NULL) {
    if (strcmp(symbolName, current->name) == 0) {
      exists = TRUE;
      break;
    }
    current = (Symbol*) current->hh.next;
  }

  return exists;
}


void create_new_scope_level() {
  ScopeStack *tmpScope;
  tmpScope = (ScopeStack*) malloc(sizeof(ScopeStack));

  if (STACK_EMPTY(scopeStackHead)) {
    tmpScope->level = 0;
    // printf("is empty");
  } else {
    // printf("is not empty");
    ScopeInfo current_scope = get_current_scope();
    tmpScope->level = current_scope.level+1;
  }
  tmpScope->scopeID = gen_random_uniqueID();
  STACK_PUSH(scopeStackHead, tmpScope);
}

void decrease_scope_level() {
  ScopeStack *tmpStack;
  STACK_POP(scopeStackHead, tmpStack);
  free(tmpStack);
}

ScopeInfo get_current_scope() {
  int current_scope = 0;
  int uniqueID;
  ScopeInfo scopeData;
  if (!STACK_EMPTY(scopeStackHead)) { /* if scope stack is not empty, return current scope, return 0 otherwise */
    ScopeStack *tmpStack;
    STACK_POP(scopeStackHead, tmpStack);
    current_scope = tmpStack->level;
    uniqueID = tmpStack->scopeID;
    STACK_PUSH(scopeStackHead, tmpStack);
  } else {
    uniqueID = gen_random_uniqueID();
  }
  scopeData.level = current_scope;
  scopeData.scopeID = uniqueID;
  return scopeData;
}
