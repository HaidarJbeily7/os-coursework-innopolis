#!/bin/bash

gcc mmu.c -o mmu
gcc pager.c -o pager

# Ensure the directory exists
mkdir -p /tmp/ex2

./pager 4 2 &
PAGER_PID=$!

# Allow pager some time to initialize and print its PID
sleep 2

# Run the MMU with a sample reference string
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $PAGER_PID