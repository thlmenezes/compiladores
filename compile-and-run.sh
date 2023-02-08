#!/bin/env bash

run=./run-tests.sh
# run="./run-test.sh \"./tests/declarar função.txt\""
make clean
make

$run | grep ERROR
