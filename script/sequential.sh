#!/bin/bash

path=/home/lkwangh/Compaction_Simulator
num=$1
type=$2

if [ $type = "new" ]; then
    rm $path/input_data
    touch $path/input_data
fi

for i in $(seq 1 $num); do
    printf "%d " $i >> $path/input_data
done
