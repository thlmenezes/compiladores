/* Compiladores 2022-2 21/12/22 */
/* - Fernanda                   */
/* - Lucas Vinicius             */
/* - Rafael                     */
/* - Thales                     */

%{
#include <stdio.h>
#define print printf
%}

id	                  [a-zA-Z_][a-zA-Z_0-9]*\n
inteiros              [+-]?[0-9]+\n
float                 [-+]?[0-9]+"."[0-9]*\n|[-+]?"."[0-9]+\n
comentario            "//".*\n
comentario_multilinha "/*"([^*]|[*]*[^*/])*[*]+[/]\n(?#se usar "*/", loop inf)
outro	                .*\n

%%
{id}                    { print("identificador\n");           }
{inteiros}              { print("inteiros\n");                }
{float}                 { print("float\n");                   }
{comentario}            { print("comentario de uma linha\n"); }
{comentario_multilinha} { print("comentario multilinha\n");   }
{outro}                 { print("outro\n");                   }
%%
/* recomendavel declarar sempre funcao yywrap() */
int yywrap ();

/* programa principal */
main() {
	print("Informe um token e tecle <enter>:\n");
	yylex(); /* scanner gerado por Flex */
}

int yywrap() {
	return 1;
}
