# Да се напише Shell скрипта која што ќе провери дали во тековниот директориум се наоѓаат датотеките "poraka.txt" и "kontakti.vcf". 
# Доколку ги има, тогаш ги пребарува сите текстуални датотеки во тековниот директориум (со екстензија .txt, игнорирајќи ја датотеката "poraka.txt") 
# и секоја една датотека ја праќа на влез на Python скриптата заедно со "kontakti.vcf". Секоја наредба што ќе се врати, SHELL скриптата треба да ја изврши.

#!/bin/bash

if [[ -f "poraka.txt" && -f "kontakti.vcf" ]]; then
    for file in *.txt; do
        if [[ "$file" != "poraka.txt" ]]; then
            commands = $(python3 zad1.py "$file" "kontakti.vcf")

            for cmd in $commands; do
                $cmd 
            done
        fi
    done
else
    echo "Datotekite 'poraka.txt' ili 'kontakti.vcf' gi nema vo tekovniot direktorium"
fi
