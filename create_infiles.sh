#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Wrong Usage"
    exit -1
fi

input=$1
country=$2

while IFS= read -r line
do
    fileCounter=0
    mkdir -p $3/$line
    id=2
    while [ $fileCounter -lt $4 ]
    do
        day=$((1 + RANDOM % 29))
        if [ "$day" -le 9 ]; then
            day=0$day
        fi

        month=$((1 + RANDOM % 11))
        if [ "$month" -le 9 ]; then
            month=0$month
        fi

        date=$day-$month-$((RANDOM % 120 + 1900))

        entryCounter=0
        while [ $entryCounter -lt $5 ]
        do
            # id=2
            state="EXIT"
            ran=$((RANDOM % 10))
            if [ "$ran" -le 6 ]; then
                state="ENTER"
            fi

            entry=$id" "$state" "$(head /dev/urandom | tr -dc A-Za-z | head -c 13)" "$(head /dev/urandom | tr -dc A-Za-z | head -c 13)" "$(shuf -n 1 $country)" "$((RANDOM % 120))
            # echo $entry >> $3/$line/$date.txt
            echo $entry >> $3/$line/$date
            ((entryCounter++))
            ((id++))
        done
        ((fileCounter++))
    done
done < "$input"