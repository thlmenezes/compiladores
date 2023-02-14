#ifndef AST_H_
#define AST_H_

#include <stdio.h>
#include <stdlib.h>

enum astNodeType {
  enumLeftRightMiddleBranch, // (usa os 3 galhos da arvore)
  enumLeftRightBranch,      
  enumLeftRightValueBranch,    
  enumValueLeftBranch,        
  enumValueTypeOnly,          
}; /* definições para o Node type */

/* struct to represent the AST and nodes */
typedef struct parserNode {
  struct parserNode* rightBranch;
  struct parserNode* leftBranch;
  struct parserNode* middleBranch;
  char* value;
  char* type;
  char* astNodeClass;
  // char* tempReg;
  // char* cast;
} parserNode;

/* struct used as param during the parsing of the source code to create new nodes */
typedef struct AstParam {
  struct parserNode* rightBranch;
  struct parserNode* leftBranch;
  struct parserNode* middleBranch;
  char* value;
  char* type;
  char* astNodeClass;
  enum astNodeType nodeType;
} AstParam;

void print_parser_ast(parserNode *node, int level);

/* returns one node of the AST based on AstParam */
parserNode* add_ast_node(AstParam AstParam);
parserNode* createLiteralIntNode(char* numStr);
void printLiteralIntNode(parserNode* node);
 
#endif // AST_
