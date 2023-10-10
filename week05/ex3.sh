#!/bin/bash

gcc  ex3.c -o ex3 -pthread

n=10000000
m_values=(1 2 4 10 100)

echo "m | Execution Time (seconds)" > ex3_res.txt
for m in "${m_values[@]}"; do
    echo -n "$m | " >> ex3_res.txt
    { time ./ex3 $n $m; } 2>&1 | grep real | cut -f2 >> ex3_res.txt
done

# Clean up
rm -f ex3

echo "Execution completed."
