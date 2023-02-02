#!/usr/bin/env bash

echo "Rodando testes..."

OUTPUT_FILE=${1:-"./output.log"}

echo "" > "$OUTPUT_FILE"

TEST_FILES=./tests/*
for test in $TEST_FILES
do
	./run-test.sh $@ -a  "$test" "$OUTPUT_FILE"
done
