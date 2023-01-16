#!/bin/bash

./compile_lexic.sh

yacc -d -v sintatico.y
