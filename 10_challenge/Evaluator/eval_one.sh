#!/bin/bash

if [ -z "$1" ]; then
    echo "No header supplied. Usage: ./eval_one.sh [header file 'xx_yy.hpp' from 'submissions/']"
fi

HEADER=$1
HEADER_DST="headers/find14.hpp"
cp -f "submissions/${HEADER}" $HEADER_DST
mkdir -p logs

# Define the source file and output file base name
SOURCE_FILE="find_14_chars.cpp"
OUTPUT_FILE_BASE="find14"

# Define the optimization flags
FLAGS=("-O0" "-O1" "-O2" "-O3" "-Ofast")
TESTS=("256M.txt" "1G.txt" "4G.txt")

function run_tests {
    for TEST in "${TESTS[@]}"; do
        ./$1 $TEST >> "logs/${HEADER}_$1_${TEST}_log"
        sleep 5
    done

    echo $1 "Done"
}

# Loop through each flag
for FLAG in "${FLAGS[@]}"; do
    # Determine the output file name based on the flag
    if [ -z "$FLAG" ]; then
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_no_flag.out"
    else
        OUTPUT_FILE="${OUTPUT_FILE_BASE}_${FLAG//-}.out"
    fi

    g++ $FLAG -o $OUTPUT_FILE $SOURCE_FILE -fopenmp
    
    if [ $? -ne 0 ]; then
        echo "Compilation Failed. Exiting"
        break
    else
        echo "Running Tests for flag $FLAG."
    fi
    
    for TEST in "${TESTS[@]}"; do
        if [ ! -f "$TEST" ]; then
            echo "Test" $TEST "doesn't exist. Exiting."
            rm "${HEADER_DST}"
            rm "${OUTPUT_FILE}"
            exit
        fi
    done
    
    for i in {1..100}; do
        run_tests $OUTPUT_FILE
        sleep 5
    done
    
    rm "${OUTPUT_FILE}"
done

rm "${HEADER_DST}"
