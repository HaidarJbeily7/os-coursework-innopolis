#!/bin/bash

# Compile the two C programs
gcc mmu.c -o mmu
gcc pager.c -o pager

# Ensure the directory exists
mkdir -p /tmp/ex2

# Run the pager in the background with 10 pages and 5 frames as an example
./pager 4 2 &
PAGER_PID=$!

# Allow pager some time to initialize and print its PID
sleep 2

# Run the MMU with a sample reference string
./mmu 4 R0 W1 R2 R3 W4 R5 W6 R7 R8 R9 $PAGER_PID

# kill -9 $PAGER_PID