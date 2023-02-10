#ifndef __DEFINES_H__
#define __DEFINES_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#define MAXTOKENLEN 10000
/* tokenString array stores the lexeme of each token */
extern char tokenBuffer[MAXTOKENLEN+1];

// fake print with same function type as printf
int printf(const char *format, ...);
// // for lex to print, use this define
#define lex_print printf
#define syn_print printf
#define syn_error printf
// for lex to not print, use this define
// #define lex_print fake_printf

#endif /* __DEFINES_H__ */
