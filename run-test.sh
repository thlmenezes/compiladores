#!/usr/bin/env bash

##### PARSING OPTS
OPTIND=1 # a POSIX variable needed for getops
# Initialize our own variables:
OPT_APPEND=0
OPT_VERBOSE=0

while getopts "av" opt; do
  case "$opt" in
    a)  OPT_APPEND=1
      ;;
    v)  OPT_VERBOSE=1
      ;;
  esac
done

# extra code for opts
shift $((OPTIND-1))
[ "${1:-}" = "--" ] && shift
###### PARSING OPTS

TEST_FILE_SUFFIX="txt"
TEST_FILENAME=$(basename "$1" ".$TEST_FILE_SUFFIX")

OUTPUT_FILE=${2:-"./output.log"}

DUMP_FOLDER=/tmp/compiladores-dump
if [ ! -d "$DUMP_FOLDER" ]; then
  mkdir "$DUMP_FOLDER" -p
fi

DUMP_FILE="$DUMP_FOLDER/$TEST_FILENAME.$TEST_FILE_SUFFIX"

# add header of result
echo "
----------------------------
Test '$TEST_FILENAME':
" > "$DUMP_FILE"

if ./parser < "$1" &>> "$DUMP_FILE" ; then
  echo "Processing \"$TEST_FILENAME\": Ok!"
  
  # if should print
  if [ $OPT_VERBOSE -eq 1 ]; then
    cat "$DUMP_FILE"
  fi

else
  echo "Processing \"$TEST_FILENAME\": ERROR!"
  cat "$DUMP_FILE"
fi

if [ $OPT_APPEND -eq 1 ]; then
  cat "$DUMP_FILE" >> "$OUTPUT_FILE"
else
  cp "$DUMP_FILE" "$OUTPUT_FILE"
fi
