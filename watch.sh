

defaultWatch() {
  local FILE="$1"
  local DIR="$2"
  echo "The file '$FILE' in directory '$DIR' was edited"
}

WATCH_PATH=${1:-"./path"}
COMMAND=${2:-defaultWatch}

inotifywait -m -e create --exclude y.tab.c -e modify $WATCH_PATH |
  while read dir action file; do
    # do something with the file
    $COMMAND "$file" "$dir"
  done
