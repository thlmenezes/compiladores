#include <stdio.h>
#include "defines.h"
#include "symbol_table.h"

Symbol* symbolTable = NULL;

void addSymbol(SymbolData newSymbolData) {
  Symbol* newSymbolPtr;
  HASH_FIND_INT(symbolTable, &newSymbolData.symbolID, newSymbolPtr);
    // printf("symbol %s already declared!\n", &newSymbolData.type);
    printf("addSymbol recieved \"%s\"\n", newSymbolData.name);
    // printf("symbol %i already declared!\n", &newSymbolData.name);
    // printf("symbol %i already declared!\n", newSymbolData.name);
  
  if (newSymbolPtr != NULL) {
   printf("symbol %s already declared!\n", newSymbolData.name);
    return;
  }

  
  // se está adicionando um parametro a uma função, atualize a função
  if (newSymbolData.symbolType == enumParameter) {
    Symbol *func;
  
    for (func = symbolTable; func != (Symbol*) NULL; func = (Symbol*)(func -> hh.next))
    if (func->name == newSymbolData.associatedFunction && func->scopeID == newSymbolData.associatedFunctionScopeId) {
        func->last_param++;
        func->params_id_list[func->last_param] = newSymbolData.symbolID;
    }
  }

  // cria o novo elemento da tabela e adiciona os dados
  newSymbolPtr = (Symbol*)malloc(sizeof(Symbol));
  newSymbolPtr->symbolID = newSymbolData.symbolID;
  newSymbolPtr->type = newSymbolData.type;
  newSymbolPtr->symbolType = newSymbolData.symbolType;
  newSymbolPtr->name = newSymbolData.name;
  
  // TODO: fix this when scope added
  // newSymbolPtr->scope = current_scope.level;
  newSymbolPtr->scopeLevel = 0;
  // newSymbolPtr->scopeID = current_scope.scopeID;
  newSymbolPtr->scopeID = 0;
  // TODO: fix this when scope added

  // newSymbolPtr->tacName = newSymbolData.tacName;
  // newSymbolPtr->line = newSymbolData.line;
  // newSymbolPtr->column = newSymbolData.column;
  // newSymbolPtr->params_id_list[0] = -1;
  newSymbolPtr->last_param = -1;

  // adiciona o novo elemento na tabela
  HASH_ADD_INT(symbolTable, symbolID, newSymbolPtr);
}

void printSymbol(Symbol* symbol) {
  printf("\t- symbol name: \"%s\"\n", symbol->name);
}

void printSymbolTable() {
  printf("==============================================\n");
  printf("symbol table:\n");
  
  Symbol* current = symbolTable;
  while (current != NULL) {
    printSymbol(current);
    current = (Symbol*) current->hh.next;
  }
  printf("==============================================\n");
}
