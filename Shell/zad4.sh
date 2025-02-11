# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, имињата
# на оние датотеки кои што имаат помалку од 1000 зборови ги праќа како аргументи од командна линија
# на повик од Python скриптата (сите имиња ги праќа во еден повик). За оние датотеки кои што имаат 
# повеќе од 1000 зборови, креира посебен повик и нивните имиња ги праќа како еден аргумент од
# командна линија. 

#!/bin/bash

small_files=()
large_files=()

for file in *; do
    if [[ -f "$file" ]]; then
        word_count=$(wc -w < "$file")

        if [[ $word_count -lt 1000 ]]; then
            small_files+=("$file") 
        else
            large_files+=("$file") 
        fi
    fi
done

if [[ ${#small_files[@]} -gt 0 ]]; then # - broj na fajlovi, @ - za da se pristapi do site elemnti od listata
    python3 zad4.py "${small_files[@]}"
fi

if [[ ${#large_files[@]} -gt 0 ]]; then
    python3 zad4.py "${large_files[@]}"
fi