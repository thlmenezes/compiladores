/* Verificando a sintaxe de programas segundo nossa GLC-exemplo */
/* considerando notacao polonesa para expressoes */

%{
#include <stdlib.h>
#include <stdio.h> 
#include "../lib/AST.h"
#include "../lib/symbol_table.h"
#include "defines.h"
#include "utils.h"

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
%type <node> input line programa bloco command single_command if_else loop_while read_command write_command declare_var expression func_call argument_list lista_cmds use_var_expression declare_func return_command declar_argument
%type <str> DATA_TYPE identifier
%%
/* Regras definindo a GLC e acoes correspondentes */
/* neste nosso exemplo quase todas as acoes estao vazias */


entryPoint: input {
  parser_ast = $1;
  syn_print("Program entry point\n");
}
;

input: input line 
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
	: IF '(' expression ')' command %prec LOWER_THAN_ELSE {}
	| IF '(' expression ')' command ELSE command
;

declare_var
	: DATA_TYPE identifier
		'=' expression {

		if (symbolExists($2)) {
			syn_error("ERROR: re-declaring \"%s\"\n", $2);
			YYABORT;
		}

		SymbolData newVar = {
			.symbolID = globalCounterOfSymbols++,
			.symbolType=enumFunction,
			.type = $1,
			.name = $2,
		};
		addSymbol(newVar);
	}
;

DATA_TYPE
	: TYPE_INT { $$ = "INT"; }
	| TYPE_FLOAT { $$ = "FLOAT"; }
;

loop_while: WHILE '(' expression ')' command;
read_command: READ '(' identifier ')';

write_command: LOG '(' expression
	{
		$<str>3 = copyString(tokenBuffer);
	} ')' {
		printf("yoooooooooo tokenBuffer is %s/n", tokenBuffer);
		/* syn_print("~~~~~~test write \"%s\"\n", $<str>3); */
		$$ = NULL;
	};

expression
	: NUM				{;}
	| use_var_expression
	| expression expression '+'			{;}
	| func_call
;

use_var_expression: identifier  {
		if (!symbolExists(tokenBuffer)) {
			syn_error("ERROR: using non-declared symbol \"%s\"\n", $1);
			YYABORT;
		}

		// TODO: create AST node and return it
		$$ = NULL;
	}
;

func_call: identifier '(' argument_list ')';

argument_list
	: %empty
	| expression
	| expression ',' argument_list
;

declar_argument_list
	: %empty
	| declar_argument_list declar_argument {}
	| declar_argument_list ',' declar_argument {} 
; 

declar_argument:
	DATA_TYPE identifier {
	}
;

declare_func:
	DATA_TYPE identifier '(' declar_argument_list ')' bloco {
    create_new_scope_level();
		$$ = NULL;
	}
;

return_command:
	RETURN expression { $$ = NULL; }
;

identifier:
	ID  { $<str>$ = copyString(tokenBuffer); }
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
	syn_print ("ERROR: Problema com a analise sintatica! %s\n", s);
}
