#!/usr/bin/env bash

TEST_FILE_SUFFIX="txt"
TEST_FILENAME=$(basename "$1" ".$TEST_FILE_SUFFIX")

DUMP_FOLDER=/tmp/compiladores-dump
if [ ! -d "$DUMP_FOLDER" ]; then
  mkdir "$DUMP_FOLDER" -p
fi

DUMP_FILE="$DUMP_FOLDER/$TEST_FILENAME.$TEST_FILE_SUFFIX"

if ./parser < "$1" &> "$DUMP_FILE"; then
  echo "Processing \"$TEST_FILENAME\": Ok!"
  # cat "$DUMP_FILE"
else
  echo "Processing \"$TEST_FILENAME\": ERROR!"
  cat "$DUMP_FILE"
fi
