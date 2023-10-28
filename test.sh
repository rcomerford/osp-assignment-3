#!/bin/bash

GENS=( 1000 2000 5000 10000 )

# Remove all .txt files in the current directory
rm -f *.txt

# Remove all data files
rm -f ./data/df*

# Generate new data files
echo "GENERATING DATA FILES"
for gen in "${GENS[@]}"
do
    for ((i=1; i<=3; i++))
    do
        ./generator.sh $gen >> "./data/df_${gen}_${i}"
    done
done

# Iterate over each file matching the wildcard pattern
echo "TESTING FIRST FIT"
for file in ./data/*; do
    if [ -e "$file" ]; then
        echo -e "-------------------- TEST: ${file} --------------------" >> firstfit.txt
        echo -e "" >> firstfit.txt
        ./firstfit $file >> firstfit.txt
    fi
done

# Iterate over each file matching the wildcard pattern
echo "TESTING BEST FIT"
for file in ./data/*; do
    if [ -e "$file" ]; then
        echo -e "-------------------- TEST: ${file} --------------------" >> bestfit.txt
        echo -e "" >> bestfit.txt
        ./bestfit $file >> bestfit.txt
    fi
done
