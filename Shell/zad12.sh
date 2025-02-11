# Да се напише SHELL скрипта која како аргумент добива листа од имиња од датотеки. Скриптата, за
# секоја датотека (име од датотека) да провери дали ја има како датотека во тековниот директориум, или
# под-директориуми. Доколку ја има, проверува дали датотеката (во нејзината содржина) има барем еден
# валиден датум (DD-MM-YYYY), и доколку нема, ја игнорира. Доколку има валиден датум, тогаш ја
# повикува Python скриптата и како прв аргумент го праќа името на датотеката, и т.н. со сите датотеки што
# се пратени како аргументи од командна линија. За секоја датотека што ќе биде пратена
# на Python скриптата, се печати на екран нејзиното име и колку различни датуми се пронајдени.

# #!/bin/bash

if [ $# -lt 1 ]; then
    echo "Nevaliden br na argumenti"
    exit 1
fi

for dat in "$@"; do
    jaIma=$(find . -name "$dat")
    
    if [ -n "$jaIma" ]; then

       valid=$(cat $jaIma | grep [1-31]\-[1-12]\-[0-9][0-9][0-9][0-9])
        
        if [ -n "$valid" ]; then

            count=$(echo "$valid" | wc -l)
            echo "Datotekata $dat sodrzi $count razlicni datumi."

            python3 zad12.py "$jaIma"
        fi
    else
        echo "Datotekata $dat ne postoi"
    fi
done