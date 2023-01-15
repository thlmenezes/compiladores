#!/bin/bash

flex c_lang.l
gcc lex.yy.c -o lex.yy.out
./lex.yy.out
