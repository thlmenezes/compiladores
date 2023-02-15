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
typedef struct ParserNode {
  struct ParserNode* rightBranch;
  struct ParserNode* leftBranch;
  struct ParserNode* middleBranch;
  char* value;
  char* type;
  char* astNodeClass;
  // char* tempReg;
  // char* cast;
} ParserNode;

/* struct used as param during the parsing of the source code to create new nodes */
typedef struct AstParam {
  struct ParserNode* rightBranch;
  struct ParserNode* leftBranch;
  struct ParserNode* middleBranch;
  char* value;
  char* type;
  char* astNodeClass;
  enum astNodeType nodeType;
} AstParam;

typedef enum Side {
  left,
  middle,
  right,
  none
} Side;

void print_parser_ast(ParserNode *node, int level, Side side);

/* returns one node of the AST based on AstParam */
ParserNode* add_ast_node(AstParam AstParam);
ParserNode* createLiteralIntNode(char* numStr);
void printLiteralIntNode(ParserNode* node);
 
#endif // AST_
