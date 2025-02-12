# Да се напише Shell скрипта која ќе ја користи претходно дефинираната Python скрипта. Скриптата
# shell како аргументи добива влезна датотека и непознат број на излезни датотеки. Скриптата
# треба да ја подели влезната датотека на еднаков број на линии во излезните датотеки. Доколку
# влезната датотека има K линии и има N излезни датотеки, тогаш секоја од излезните датотеки
# ќе добие K/N дел од влезната датотека. 

#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Nedovolen broj na argumenti"
    exit 1
fi

input_file=""
output_files=()

for file in "$@"; do
    if [ -n "$input_file" ]; then
        output_files+=("$file")
    else
        input_file="$file"
    fi
done

vk_linii=$(wc -l < "$input_file")

br_izlez=$(($# - 1))
linii=$((vk_linii / br_izlez))


start=1
for file in "${output_files[@]}"; do
    end=$((start + linii - 1))
    
    # ??????

    start=$((end + 1))
done


