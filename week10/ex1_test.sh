#!/bin/bash

# Check if path is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path>"
    exit 1
fi

PATH_TO_WATCH=$1

# Test commands
mkdir "$PATH_TO_WATCH/folder1"
echo "Some content" > "$PATH_TO_WATCH/f1.txt"
echo "Some other content" > "$PATH_TO_WATCH/f2.txt"
ls "$PATH_TO_WATCH/folder1"
cat "$PATH_TO_WATCH/f1.txt"
echo "Hello" >> "$PATH_TO_WATCH/f2.txt"
touch "$PATH_TO_WATCH/f1.txt"
rm "$PATH_TO_WATCH/f1.txt"
chmod u=w "$PATH_TO_WATCH/folder1"
rm -r "$PATH_TO_WATCH/folder1"
