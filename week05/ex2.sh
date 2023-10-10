#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number of threads>"
    exit 1
fi

num_threads="$1"

gcc -pthread ex2.c -o ex2 && ./ex2 "$num_threads"


rm -f ex2

echo "Execution completed."