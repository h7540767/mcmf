#!/bin/bash
for i in $(seq 0 8)
do
    ./bin/cdn ./c/case${i}.txt ./result.txt
    echo -e "\n"
done
