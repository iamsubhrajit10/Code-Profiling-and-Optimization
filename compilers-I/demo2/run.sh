#!/bin/bash

# Define the source file and output file base name
SOURCE_FILE="demo2.c"
mkdir -p objdumps
OUTPUT_FILE_BASE="objdumps/demo2"
HEADER_DIR="../"  # Directory where mytime.h is located

# Define the optimization flags
FLAGS=("" "-O0" "-O1" "-O2" "-O3" "-Ofast")

# Loop through each flag
for FLAG in "${FLAGS[@]}"; do
    # Determine the output file name based on the flag
    if [ -z "$FLAG" ]; then
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_no_flag"
    else
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_${FLAG//-}"
    fi

    # Compile the source file with the current flag and -fverbose-asm
    echo "Compiling with flag: $FLAG"
    gcc $FLAG -fverbose-asm -I$HEADER_DIR -o $OUTPUT_FILE $SOURCE_FILE

    # Dump the object file
    OBJ_FILE="${OUTPUT_FILE}.o"
    echo "Dumping object file: $OBJ_FILE"
    objdump -d $OUTPUT_FILE > "${OBJ_FILE}.dump"

    echo "----------------------------------------"
done

# Execute the compiled output files one by one
for FLAG in "${FLAGS[@]}"; do
    # Determine the output file name based on the flag
    if [ -z "$FLAG" ]; then
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_no_flag"
    else
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_${FLAG//-}"
    fi

    # Execute the output file
    echo "Executing: $OUTPUT_FILE"
    ./$OUTPUT_FILE

    echo "----------------------------------------"
done