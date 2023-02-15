/* Compiladores 2022-2 21/12/22 */
/* - Fernanda                   */
/* - Lucas Vinicius             */
/* - Rafael                     */
/* - Thales                     */
/* Analisador Lexico            */

%{
	#include <stdio.h>
	#include "y.tab.h"
	#include "defines.h"

char tokenBuffer[MAXTOKENLEN+1];
int fake_printf(const char *format, ...){}
%}

%{/* grupos simples */ 
%}
letra	                [a-zA-Z_]
digito								[0-9]
alpha_num							{letra}|{digito}
sinal									[+-]

%{ /* palavras reservadas */ 
%}
IF      if
ELSE    else
WHILE   while
INT     int
FLOAT   float
READ	  read
LOG		  log
RETURN	return

%{
	/* ignore */
%}
comentario            "//".*
comentario_multilinha "/*"([^*]|[*]*[^*/])*[*]+[/](?#se usar "*/", loop inf)
espacos               [ \t\n]+

%{ /* complicados */ 
%}
id	                  {letra}{alpha_num}*
inteiros              {sinal}?{espacos}?{digito}+
float                 {sinal}?{espacos}?{digito}+"."{digito}*|{sinal}?{espacos}?"."{digito}+

outro	                .

%%
{comentario}            { lex_print("comentario de uma linha\n"); }
{comentario_multilinha} { lex_print("comentario multilinha\n");   }
{espacos}			          ;

{IF}    	{ lex_print("IF\n");return IF;}
{ELSE}		{ lex_print("ELSE\n");return ELSE;}
{WHILE} 	{ lex_print("WHILE\n");return WHILE;}
{INT}   	{ lex_print("INT\n");return TYPE_INT;}
{FLOAT} 	{ lex_print("FLOAT\n");return TYPE_FLOAT;}
{READ} 		{ lex_print("READ\n");return READ;}
{RETURN}	{ lex_print("RETURN\n");return RETURN;}

{LOG} { 
	lex_print("LOG\n");
	yylval.str = (char *) strdup(yytext);
	return LOG;
}

{inteiros}              { 
	lex_print("inteiros\n");
	yylval.str = (char *) strdup(yytext);
	return NUM;
}
{float}                 { 
	lex_print("float\n");
	// yylval.val = strdup(yytext);
	// yylval.length = yylen;
	// return FLOAT;        
}
{id}                    {
	lex_print("identificador \"%s\".\n", yytext);
	create_and_store_token(yytext);
	return ID;
}
{outro}                 { return yytext[0];  }
%%

int yywrap() {
	return 1;
}

void create_and_store_token(char* token_lexeme) {
	strncpy(tokenBuffer, yytext, MAXTOKENLEN);
	lex_print("yytext: \"%s\". tb: \"%s\"\n", yytext, tokenBuffer);
}
