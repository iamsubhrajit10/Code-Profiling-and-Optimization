#!/bin/bash

for F in $(find "submissions" -type f -name "*.hpp"); do
    echo "Evaluating" $(basename "$F")
    ./eval_one.sh $(basename "$F")
done
