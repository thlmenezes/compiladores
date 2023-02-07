#ifndef __DEFINES_H__
#define __DEFINES_H__

#define TRUE 1
#define FALSE 0
#define NULL 0

#define MAXTOKENLEN 10000
/* tokenString array stores the lexeme of each token */
extern char tokenBuffer[MAXTOKENLEN+1];

// fake print with same function type as printf
int fake_printf(const char *format, ...);
// // for lex to print, use this define
#define lex_print printf
#define syn_print printf
// for lex to not print, use this define
// #define lex_print fake_printf

#endif /* __DEFINES_H__ */
