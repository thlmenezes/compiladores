/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdlib.h>
#include <stdio.h> 
#include "AST.h"
#include "symbol_table.h"
#include "defines.h"
#include "utils.h"
#include "types.h"

extern int yylex();

int globalCounterOfSymbols = 1;

parserNode* parser_ast = NULL;

/* lexeme of identifier or reserved word */
char tokenBuffer[MAXTOKENLEN+1];
%}

%union {
  char* str;
  struct parserNode* node;
}

%token<str> IF
%token<str> ELSE
%token<str> WHILE
%token<str> READ
%token<str> LOG
%token<str> RETURN

%token<str> TYPE_INT
%token<str> TYPE_FLOAT

%token<str> NUM
%token<str> ID

// Parsing entry point start
%start entryPoint

/* Tokens to solve shift reduce conflicts (https://stackoverflow.com/questions/1737460/how-to-find-shift-reduce-conflict-in-this-yacc-file) */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

// Types definitions
%type <node> input line programa bloco command single_command if_else loop_while read_command write_command declare_var expression literal_expression func_call argument_list lista_cmds use_var_expression declare_func return_command declar_argument declar_argument_list math_expression
%type <str> DATA_TYPE identifier math_operator
%%
/* Regras definindo a GLC e acoes correspondentes */
/* neste nosso exemplo quase todas as acoes estao vazias */


entryPoint: input {
  parser_ast = $1;
  syn_print("Program entry point\n");
}
;

input: input line {
	AstParam astP = { .leftBranch = $1, .rightBranch = $2, .nodeType = enumLeftRightBranch, .astNodeClass="INPUT_STATEMENT" };
    $$ = add_ast_node(astP);
	}
	| %empty {
		$$ = NULL; 
	}
;

line: programa  { 
	$$ = $1;
	syn_print ("Programa sintaticamente correto!\n"); 
	printSymbolTable();
}
;

programa
	:	bloco	{
		$$ = $1; // (o primeiro nó da AST vai ser o proprio programa)
	}
;

bloco
	: '{' {
		create_new_scope_level();
	} lista_cmds '}' {
		$$ = $3;
		decrease_scope_level();
	}
;

lista_cmds
	: %empty {
		$$ = NULL; 
	}
	| command lista_cmds {
		AstParam astP = { .leftBranch = $1, .rightBranch = $2, .nodeType = enumLeftRightBranch, .astNodeClass="COMMANDS_LIST" };
    	$$ = add_ast_node(astP);
	}
;

command
	: bloco {
		$$ = $1;
	}
	| single_command {
		$$ = $1;
	}
;

single_command
	: if_else { $$ = $1; }
	| loop_while { $$ = $1; }
	| declare_func { $$ = $1; }
	| read_command ';' { $$ = $1; }
	| write_command ';' { $$ = $1; }
	| declare_var ';' { $$ = $1; }
	| expression ';' { $$ = $1; }
	| return_command ';' { $$ = $1; }
	| ';' { $$ = NULL; }
;

if_else
	: IF '(' expression ')' command %prec LOWER_THAN_ELSE {
		AstParam nodeParam = {
			.nodeType = enumLeftRightBranch,
			.astNodeClass = "IF_THEN",
			.leftBranch = $3,
			.rightBranch = $5,
		};
		$$ = add_ast_node(nodeParam);
	}
	| IF '(' expression ')' command ELSE command {
		AstParam nodeParam = {
			.nodeType = enumLeftRightMiddleBranch,
			.astNodeClass = "IF_THEN_ELSE",
			.leftBranch = $3,
			.middleBranch = $5,
			.rightBranch = $7,
		};
		$$ = add_ast_node(nodeParam);
	}
;

declare_var: DATA_TYPE identifier '=' expression {

		if (symbolExists($2)) {
			syn_error("ERROR: re-declaring \"%s\"\n", $2);
			YYABORT;
		}

    ScopeInfo current_scope = get_current_scope();
		SymbolData newVar = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType=enumVariable,
			.type = $1,
			.name = $2,
			.scopeID = current_scope.scopeID,
			.scopeLevel = current_scope.level
		};
		addSymbol(newVar);

		AstParam astP = { .type=$1, .value = $2, .leftBranch=$4, .nodeType = enumValueLeftBranch, .astNodeClass="DECLARE_VARIABLE" };
    	$$ = add_ast_node(astP);
	}
;

DATA_TYPE
	: TYPE_INT { $$ = INT_TYPE; }
	| TYPE_FLOAT { $$ = "FLOAT"; }
;

loop_while: WHILE '(' expression ')' command;
read_command: READ '(' identifier ')' {
	AstParam astP = { .type=$1, .value = $3, .nodeType = enumValueTypeOnly, .astNodeClass="READ_COMMAND" };
    $$ = add_ast_node(astP);
	// printf("yoooooooooo tokenBuffer is %s/n", $1);
};

write_command: LOG '(' expression ')' {
	AstParam astP = { .type=$1, .value = $3, .nodeType = enumValueTypeOnly, .astNodeClass="LOG_COMMAND" };
	$$ = add_ast_node(astP);
	// printf("yoooooooooo 2 tokenBuffer is %s/n", $1);
	};

expression
	: literal_expression
	| use_var_expression
	| math_expression
	| func_call
;

math_expression:
	expression expression math_operator {
		AstParam nodeParam = {
			.nodeType = enumLeftRightBranch,
			.astNodeClass = "MATH_EXPRESSION",
			.type = INT_TYPE,
			.value = $3, // operador
			.leftBranch = $1,
			.rightBranch = $2,
		};

		$$ = add_ast_node(nodeParam);
	}
;
math_operator
	: '+' { $$ = "+"; }
	| '-' { $$ = "-"; }
	| '*' { $$ = "*"; }
	| '/' { $$ = "/"; }
;

literal_expression 
	: NUM 			{
		// parserNode* node = createLiteralIntNode($1);
		// printLiteralIntNode(node);
		// $$ = node;
		$$ = createLiteralIntNode($1);
	}

use_var_expression: identifier  {
		if (!symbolExists(tokenBuffer)) {
			syn_error("ERROR: using non-declared symbol \"%s\"\n", $1);
			YYABORT;
		}

		AstParam nodeParam = {
			.nodeType = enumValueTypeOnly,
		  .astNodeClass = "USE_VAR_EXP",
			.value = $1,
			.type = INT_TYPE,
		};
		$$ = add_ast_node(nodeParam);
	}
;

func_call: identifier '(' argument_list ')' { $$ = NULL; };

argument_list
	: %empty
	| expression
	| expression ',' argument_list
;

declar_argument_list
	: %empty { $$ = NULL; }
	| declar_argument { $$ = $1; }
	| declar_argument_list ',' declar_argument {
		AstParam nodeParam = {
			.nodeType = enumLeftRightBranch,
			.astNodeClass = "DEC_ARG_LIST",
			.leftBranch = $1,
			.rightBranch = $3,
		};
		$$ = add_ast_node(nodeParam);
	} 
; 

declar_argument:
	DATA_TYPE identifier {
    ScopeInfo current_scope = get_current_scope();
		SymbolData newParam = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType=enumParameter,
			.type = $1,
			.name = $2,
			.scopeID = current_scope.scopeID,
			.scopeLevel = current_scope.level
		};
		addSymbol(newParam);

		AstParam nodeParam = {
			.nodeType = enumValueTypeOnly,
			.astNodeClass = "DEC_ARG",
			.type = $1,
			.value = $2
		};
		$$ = add_ast_node(nodeParam);
	}
;

declare_func:
	DATA_TYPE identifier '('  {
		// get scope
    ScopeInfo current_scope = get_current_scope();

		// add symbol
		SymbolData newFunc = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType = enumFunction,
			.type = $1,
			.name = $2,
			.scopeID = current_scope.scopeID,
			.scopeLevel = current_scope.level
		};
		addSymbol(newFunc);

		// create scope for block and arglist
		create_new_scope_level();
	} declar_argument_list ')' bloco {

		decrease_scope_level();

		AstParam nodeParam = {
			.nodeType = enumLeftRightBranch,
			.astNodeClass = "FUNCTION_DECL",
			.type = $1,
			.value = $2,
			.leftBranch = $5,
			.rightBranch = $7,
		};

		$$ = add_ast_node(nodeParam);
	}
;

return_command:
	RETURN expression {
		AstParam nodeParam = {
			.nodeType = enumValueLeftBranch,
		  .astNodeClass = "RETURN_CMD",
			.leftBranch = $2,
		};
		$$ = add_ast_node(nodeParam);
	}
;

identifier:
	ID  { $<str>$ = copyString(tokenBuffer); }
;

%%

#include "lex.yy.c"

int main() {
	parser_ast = NULL;

	yyparse();

	printf("\n=================== Parser AST ====================\n\n");
    print_parser_ast(parser_ast, 0);
	return 0;
}

yyerror (s) /* Called by yyparse on error */
	char *s;
{
	syn_print ("ERROR: Problema com a analise sintatica! %s\n", s);
}
