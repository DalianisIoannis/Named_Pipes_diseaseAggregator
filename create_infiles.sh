#!/bin/bash

if [ "$#" -ne 5 ]; then
    echo "Wrong Usage"
    exit -1
fi

input=$1
country=$2

FN[0]="Lisa"
FN[1]="Asa"
FN[2]="Valentina"
FN[3]="Riley"
FN[4]="Lana"
FN[5]="Abella"
FN[6]="Mia"
FN[7]="Mia"
FN[8]="Brandi"
FN[9]="Nicole"
FN[10]="Cory"
FN[11]="Adriana"
FN[12]="Alexis"
FN[13]="Kendra"
FN[14]="Kelsi"

SN[0]="Ann"
SN[1]="Akira"
SN[2]="Nappi"
SN[3]="Reid"
SN[4]="Rhodes"
SN[5]="Danger"
SN[6]="Malkova"
SN[7]="Khalifa"
SN[8]="Love"
SN[9]="Aniston"
SN[10]="Chase"
SN[11]="Chechik"
SN[12]="Texas"
SN[13]="Lust"
SN[14]="Monroe"

lenght=${#FN[@]}

id=2
while IFS= read -r line
do
    fileCounter=0
    mkdir -p $3/$line
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

            idOfentry=$id

            state="EXIT"
            ran=$((RANDOM % 10))
            if [ "$ran" -le 6 ]; then
                state="ENTER"
            fi

            if [ $state == "EXIT" ]; then
                idOfentry=$((RANDOM % id))
            fi

            entry=$idOfentry" "$state" "${FN[$(($RANDOM % $lenght))]}" "${SN[$(($RANDOM % $lenght))]}" "$(shuf -n 1 $country)" "$((RANDOM % 120))
            echo $entry >> $3/$line/$date
            ((entryCounter++))
            ((id++))
        done
        ((fileCounter++))
    done
done < "$input"