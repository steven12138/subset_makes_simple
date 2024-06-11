#!/bin/bash

# Get the start time in microseconds
start_time=$(date +%s%6N)

# Run the program
./cmake-build-debug/dfs

# Get the end time in microseconds
end_time=$(date +%s%6N)

# Calculate the elapsed time in microseconds
elapsed_time=$(( end_time - start_time ))

echo "Elapsed time: $elapsed_time microseconds"
