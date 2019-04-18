#!/bin/bash


path=/home/lkwangh/Compaction_Simulator
num=$1
type=$2

if [ $type = "new" ]; then
    rm $path/input_data
    touch $path/input_data
fi

SEED=$(head -1 /dev/urandom | od -N 1 | awk '{ print $2 }')
RANDOM=$SEED
arr=()

for i in $(seq 1 $num); do
    arr+=($i)
done

t=`expr $num - 1`

for i in $(seq 0 $t); do
    n=$((RANDOM%$num))

    tmp=${arr[$n]}
    arr[$n]=${arr[$t]}
    arr[$t]=$tmp
done

for i in $(seq 0 $t); do
    printf "%d " ${arr[$i]} >> $path/input_data
done
