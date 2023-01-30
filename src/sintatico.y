/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdlib.h>
#include <stdio.h> 
#include "../lib/AST.h"

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

// Types definitions
%type <node> input line programa bloco command single_command if_else loop_while read_command write_command declare_var base_type exp func_call argument_list lista_cmds empty_command 

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
	: command {
		$$ = $1; 
	}
	// vários comandos
	| command lista_cmds
	// regra vazia  
	|	%empty {
		$$ = NULL; 
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
	| exp ';'
	| empty_command ';'
;

if_else
	: IF '(' exp ')' command
	| IF '(' exp ')' command ELSE command
;

declare_var
	: base_type ID {
		printf("declare var");
	}
	| base_type ID '=' exp
;

base_type
	: TYPE_INT
	| TYPE_FLOAT
;

loop_while: WHILE '(' exp ')' command;
read_command: READ '(' exp ')';
write_command: LOG '(' exp ')';
empty_command: %empty;

exp
	: NUM				{;}
	| ID				{;}
	| exp exp '+'			{;}
	| func_call
;

func_call: ID '(' argument_list ')';

argument_list
	: %empty
	| exp
	| exp ',' argument_list
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
