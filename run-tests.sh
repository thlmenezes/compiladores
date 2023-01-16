#!/usr/bin/env bash

echo "Rodando testes..."

TEST_FILES=./tests/*
for test in $TEST_FILES
do
	./run-test.sh "$test"
done
