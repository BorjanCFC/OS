# Да се напише SHELL скрипта која како аргумент добива листа од имиња на датотеки. 
# SHELL скриптата  треба да ја повика повика Python скриптата од првата задача онолку пати колку што има датотеки од командна линија. 
# При секој повик, на Python скриптата (како аргументи) ја испраќа содржината на соодветната датотека. 
# Резултатот од повикот го запишува во истата датотека само во продолжение (append).

#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "Nevaliden broj na argumenti"
    exit 1
fi

for file in $@; do
    if [[ -f "$file" ]]; then
        content=$(cat "$file")
        
        python3 zad7.py "$content" >> "$file"
    else
        echo "Datotekata $file ne postoi"
    fi
done