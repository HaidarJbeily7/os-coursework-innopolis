#!/bin/bash

# Check if input file exists
if [ ! -f "input.txt" ]; then
    echo "input.txt not found!"
    exit 1
fi

# Compile the file system creation program
gcc create_fs.c -o create_fs 
if [ $? -ne 0 ]; then
    echo "Failed to compile create_fs.c"
    exit 1
fi

# Compile the file system processing program
gcc ex2.c -o  ex2
if [ $? -ne 0 ]; then
    echo "Failed to compile ex2.c"
    exit 1
fi

# Create the file system
./create_fs disk0


# Process the input file
./ex2 < input.txt
if [ $? -ne 0 ]; then
    echo "Failed to process input.txt"
    exit 1
fi

echo "File system operations completed."
