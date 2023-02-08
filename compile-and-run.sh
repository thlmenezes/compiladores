#!/bin/env bash

make clean
make
./run-test.sh "./tests/definir variável.txt"
