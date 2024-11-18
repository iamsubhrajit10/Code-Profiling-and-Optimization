#!/bin/bash

# Define the source file and output file base name
SOURCE_FILE="sample_perf_util2.c"
mkdir -p executables
OUTPUT_FILE_BASE="executables/sample_perf_util2"

# Define the optimization flags
FLAGS=("" "-O0" "-O1" "-O2" "-O3" "-Ofast")


# Execute the compiled output files one by one
for FLAG in "${FLAGS[@]}"; do
    # Determine the output file name based on the flag
    if [ -z "$FLAG" ]; then
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_no_flag"
    else
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_${FLAG//-}"
    fi

    # Compile the source file with the current flag
    echo "Compiling with flag: $FLAG"
    gcc $FLAG -o $OUTPUT_FILE $SOURCE_FILE perf_utils.c -I.

    # Execute the output file
    echo "Executing: $OUTPUT_FILE"
    sudo ./$OUTPUT_FILE

    echo "----------------------------------------"
done