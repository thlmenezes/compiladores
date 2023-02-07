/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdlib.h>
#include <stdio.h> 
#include "../lib/AST.h"
#include "../lib/symbol_table.h"

int globalCounterOfSymbols = 1;

parserNode* parser_ast = NULL;

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
%type <node> input line programa bloco command single_command if_else loop_while read_command write_command declare_var DATA_TYPE expression func_call argument_list lista_cmds

%%
/* Regras definindo a GLC e acoes correspondentes */
/* neste nosso exemplo quase todas as acoes estao vazias */


entryPoint: input {
  parser_ast = $1;
  printf("Program entry point\n");
}
;

input: input line 
	| %empty {
		$$ = NULL; 
	}
;

line: programa  { 
	$$ = $1;
	printf ("Programa sintaticamente correto!\n"); 
}
;

programa
	:	bloco	{
		$$ = $1; // (o primeiro nó da AST vai ser o proprio programa)
	}
;

bloco
	: '{' lista_cmds '}' {
		$$ = $2;
	}
;

lista_cmds
	: %empty {
		$$ = NULL; 
	}
	| command lista_cmds {
		$$ = $1;
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
	: if_else
	| loop_while
	| read_command ';'
	| write_command ';'
	| declare_var ';'
	| expression ';'
	| ';'
;

if_else
	: IF '(' expression ')' command   %prec LOWER_THAN_ELSE
	| IF '(' expression ')' command ELSE command
;

declare_var
	: DATA_TYPE ID {
		printf("declare var \"%s\"\n", $1);

		SymbolData newVar = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType=enumFunction,
			// .type = $1->value,
			.type = "INT",
			.name = $2,
		};
		addSymbol(newVar);
	}
	| DATA_TYPE ID '=' expression {
		printf("declare var \"%s\"\n", $1);

		SymbolData newVar = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType=enumFunction,
			// .type = $1->value,
			.type = "INT",
			.name = $2,
		};
		addSymbol(newVar);
	}
;

DATA_TYPE
	: TYPE_INT
	| TYPE_FLOAT
;

loop_while: WHILE '(' expression ')' command;
read_command: READ '(' expression ')';
write_command: LOG '(' expression ')';


expression
	: NUM				{;}
	| ID				{;}
	| expression expression '+'			{;}
	| func_call
;

func_call: ID '(' argument_list ')';

argument_list
	: %empty
	| expression
	| expression ',' argument_list
;

%%

#include "lex.yy.c"

int main() {
	parser_ast = NULL;
	return yyparse ();
}

yyerror (s) /* Called by yyparse on error */
	char *s;
{
	printf ("Problema com a analise sintatica! %s\n", s);
}
