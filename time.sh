#!/bin/bash

# Define the input files
input_files=(1.in 2.in 3.in 4.in 5.in 6.in 7.in)

# Define the directory containing the input files
input_dir="./data/"

# Loop through each input file
for input_file in "${input_files[@]}"; do
  # Rename the current input file to in.in
  echo "${input_dir}${input_file} ${input_dir}in.in"
  mv "${input_dir}${input_file}" "${input_dir}in.in"

  # Get the start time in microseconds
  start_time=$(date +%s%6N)

  # Run the program
  ./dp

  # Get the end time in microseconds
  end_time=$(date +%s%6N)

  # Calculate the elapsed time in microseconds
  elapsed_time=$(( end_time - start_time ))

  # Output the elapsed time for this input file
  echo "Elapsed time for ${input_file}: $elapsed_time microseconds"

  # Rename in.in back to the original input file
#  read
  mv "${input_dir}in.in" "${input_dir}${input_file}"
done
