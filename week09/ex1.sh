#!/bin/bash


gcc mmu.c -o mmu
gcc pager.c -o pager


mkdir -p /tmp/ex2


NUM_PAGES=999
NUM_FRAMES=40


# Ensure ex1.txt is empty before we start
> ex1.txt

# Define the algorithms to run
ALGORITHMS=("random" "nfu" "aging")

# Function to run the simulation with the given algorithm
run_simulation() {
    ALGORITHM=$1
    echo "Running simulation with $ALGORITHM replacement algorithm..."
    echo "Running simulation with $ALGORITHM replacement algorithm..." >> ex1.txt

    # Start the pager with the given algorithm
    ./pager $NUM_PAGES $NUM_FRAMES $ALGORITHM &
    PAGER_PID=$!

    # Allow pager some time to initialize
    sleep 2

    # Run the MMU with a sample reference string
    ./mmu $NUM_PAGES $PAGER_PID >> ex1.txt

    # Terminate pager
    kill -SIGUSR1 $PAGER_PID
    wait $PAGER_PID
}

# Run the simulation for each algorithm
for ALGO in "${ALGORITHMS[@]}"; do
    run_simulation $ALGO
done

echo "Simulations completed. Results are in ex1.txt."
