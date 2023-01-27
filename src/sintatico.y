/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdlib.h>
#include <stdio.h> 
#include "symbol_table.h"

%}

%token IF
%token ELSE
%token WHILE
%token READ
%token LOG

%token TYPE_INT
%token TYPE_FLOAT

%token NUM
%token ID

%%
/* Regras definindo a GLC e acoes correspondentes */
/* neste nosso exemplo quase todas as acoes estao vazias */
input:    /* empty */
	| input line
;
line: programa  { printf ("Programa sintaticamente correto!\n"); }
;

programa
	:	bloco		{;}
;

command
	: bloco
	| single_command
;

bloco
	: '{' lista_cmds '}'
;

BASE_TYPE
	: TYPE_INT
	| TYPE_FLOAT
;

lista_cmds
	// regra vazia  
	:	%empty
	// comando único
	| command
	// vários comandos
	| command lista_cmds
;

single_command
	: if_else
	| loop_while
	| read_command ';'
	| write_command ';'
	| declare_var ';'
	| exp ';'
	| ';'
;

if_else
	: IF '(' exp ')' command
	| IF '(' exp ')' command ELSE command
;

declare_var
	: BASE_TYPE ID
	| BASE_TYPE ID '=' exp
;

loop_while: WHILE '(' exp ')' command;

read_command: READ '(' exp ')';
write_command: LOG '(' exp ')';

exp
	:	NUM				{;}
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

main () 
{
	return yyparse ();
}

yyerror (s) /* Called by yyparse on error */
	char *s;
{
	printf ("Problema com a analise sintatica! %s\n", s);
}
