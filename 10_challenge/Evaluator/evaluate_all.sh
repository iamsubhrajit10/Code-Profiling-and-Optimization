#!/bin/bash

for F in $(find "submissions" -type f -name "*.hpp"); do
    ./eval_one.sh $(basename "$F")
done
