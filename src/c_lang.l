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

letra	                [a-zA-Z_]
digito								[0-9]
alpha_num							{letra}|{digito}
id	                  {letra}{alpha_num}*
sinal									[+-]
inteiros              {sinal}?{espacos}?{digito}+
float                 {sinal}?{espacos}?{digito}+"."{digito}*|{sinal}?{espacos}?"."{digito}+
comentario            "//".*
comentario_multilinha "/*"([^*]|[*]*[^*/])*[*]+[/](?#se usar "*/", loop inf)
espacos               [ \t\n]+
outro	                .

%%
{id}                    { print("identificador \"%s\".\n", yytext);return ID; }
{inteiros}              { print("inteiros\n");return NUM;     }
{float}                 { print("float\n");return NUM;        }
{comentario}            { print("comentario de uma linha\n"); }
{comentario_multilinha} { print("comentario multilinha\n");   }
{espacos}			          ;
{outro}                 { return yytext[0];  }
%%

int yywrap() {
	return 1;
}
