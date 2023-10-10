#!/bin/bash

# Compile the program
gcc -o ex4 ex4.c -pthread

# Values of n and m to test
n=10000000
m_values=(1 2 4 10 100)

# Run the program for each value of m and measure execution time
echo "m | Execution Time (seconds)" > ex4_res.txt
for m in "${m_values[@]}"; do
    echo -n "$m | " >> ex4_res.txt
    { time ./ex4 $n $m; } 2>&1 | grep real | cut -f2 >> ex4_res.txt
done

# Clean up
rm -f ex4

echo "Execution completed."
