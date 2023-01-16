#!/bin/bash

TEST_FILE_SUFFIX="txt"
TEST_FILENAME=$(basename "$1" ".$TEST_FILE_SUFFIX")

DUMP_FOLDER=/tmp/compiladores-dump
if [ ! -d "$DUMP_FOLDER" ]; then
  mkdir "$DUMP_FOLDER" -p
fi

DUMP_FILE="$DUMP_FOLDER/$TEST_FILENAME.$TEST_FILE_SUFFIX"

if cat "$1" | ./parser &> "$DUMP_FILE"; then
  echo "Processing \"$TEST_FILENAME\": Ok!"
else
  echo "Processing \"$TEST_FILENAME\": ERROR!"
  cat "$DUMP_FILE"
fi
