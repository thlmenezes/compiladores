/* Compiladores 2022-2 21/12/22 */
/* - Fernanda                   */
/* - Lucas Vinicius             */
/* - Rafael                     */
/* - Thales                     */
/* Analisador Lexico            */

%{
	#include <stdio.h>
	#define print printf
  #include "y.tab.h"
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
ELSE   else
WHILE   while
INT     int
FLOAT   float

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
{comentario}            { print("comentario de uma linha\n"); }
{comentario_multilinha} { print("comentario multilinha\n");   }
{espacos}			          ;

{IF}    { print("IF\n");return IF;}
{ELSE}    { print("ELSE\n");return ELSE;}
{WHILE} { print("WHILE\n");return WHILE;}
{INT}   { print("INT\n");return TYPE_INT;}
{FLOAT} { print("FLOAT\n");return TYPE_FLOAT;}

{inteiros}              { print("inteiros\n");return NUM;     }
{float}                 { print("float\n");return NUM;        }
{id}                    { print("identificador \"%s\".\n", yytext);return ID; }
{outro}                 { return yytext[0];  }
%%

int yywrap() {
	return 1;
}
