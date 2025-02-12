# Да се напише shell скрипта која како аргументи од командна линија добива влезна датотека и
# речник. Скриптата треба да ги пронајде сите речници (датотеки) со тоа име во рамки на целиот
# систем. Потоа, скриптата треба да изврши онолку преводи колку што има пронајдено речници во
# системот користејќи ја Python скриптата prva.py. Сите преводи треба да се сместат во датотека со име
# prevodYY каде YY е редниот број на преводот почнувајќи од 01.

#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Nepravilen broj na argumenti"
    exit 1
fi

input_file=$1
dictionary=$2


dicts=$(find / -type f -name "$dictionary")

if [ -z "$dicts" ]; then
    echo "Ne e pronajdeno nisto"
    exit 1
fi

counter=1

for dict in $dicts; do

    output_file="prevod$counter"

    python3 prva.py "$input_file" "$dict" > "$output_file"

    counter=$((counter + 1))
done
