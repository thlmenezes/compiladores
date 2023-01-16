/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdio.h> 
%}

%token NUM
%token ID

%%
/* Regras definindo a GLC e acoes correspondentes */
/* neste nosso exemplo quase todas as acoes estao vazias */
input:    /* empty */
	| input line
;
line
	: '\n'
  | programa '\n'  { printf ("Programa sintaticamente correto!\n"); }
;

programa
	:	'{' lista_cmds '}'		{;}
;

lista_cmds
	:	cmd				{;}
	| cmd ';' lista_cmds		{;}
;

cmd
	:		ID '=' exp			{;}
;

exp
	:	NUM				{;}
	| ID				{;}
	| exp exp '+'			{;}
;

%%

main () 
{
	yyparse ();
}

yyerror (s) /* Called by yyparse on error */
	char *s;
{
	printf ("Problema com a analise sintatica!\n", s);
}
