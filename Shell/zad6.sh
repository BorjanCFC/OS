# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, зема две по
# две датотеки од листата датотеки и ги праќа како влезни аргументи на повикот на Python скриптата од
# задача 1. Резултатот од секој повик на Python Скриптата се запишува во датотека креирана од спој на
# имињата на двете датотеки кои се праќаат на Python Скриптата, притоа се запишува во продолжение
# (append)

#!/bin/bash

files=(*)

if [ "${files[@]}" -lt 2 ]; then
    echo "Nema dovolno datoteki vo tekovniot dir"
    exit 1
fi

prev_file=""

for file in "${files[@]}"; do
    if [ -n "$prev_file" ]; then
        output_file="output_${prev_file%.*}_${file%.*}.txt"
        python3 zad6.py "$prev_file" "$file" >> "$output_file"
        prev_file="" 
    else
        prev_file="$file"
    fi
done