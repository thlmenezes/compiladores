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

id	                  [a-zA-Z_][a-zA-Z_0-9]*
inteiros              [+-]?[0-9]+
float                 [-+]?[0-9]+"."[0-9]*\n|[-+]?"."[0-9]+
comentario            "//".*
comentario_multilinha "/*"([^*]|[*]*[^*/])*[*]+[/](?#se usar "*/", loop inf)
espacos               [ \t]+
outro	                .|\n

%%
{id}                    { print("identificador\n");return ID; }
{inteiros}              { print("inteiros\n");return NUM;     }
{float}                 { print("float\n");                   }
{comentario}            { print("comentario de uma linha\n"); }
{comentario_multilinha} { print("comentario multilinha\n");   }
{espacos}			          ;
{outro}                 { print("outro\n");return yytext[0];  }
%%
/* recomendavel declarar sempre funcao yywrap() */
int yywrap() {
	return 1;
}
