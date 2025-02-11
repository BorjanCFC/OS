# Да се напише Shell скрипта која што ги наоѓа сите ценовници во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе ценовници, тогаш на
# крајот во првиот ценовник од листата ценовници се додава содржината на другите ценовници и се
# повикува python скриптата со името на првиот ценовник. 


#!/bin/bash

cenovnici=$(ls *.vcf)

if [ -z "$cenovnici" ]; then
    echo "Ne postoi nitu edna datoteka"
    exit 1
fi

prv_cenovnik=$(echo "$cenovnici" | head -n1)

broj_datoteki=$(echo "$cenovnici" | wc -l)

if [ $broj_datoteki -gt 1 ]; then
    
    for dat in $cenovnici; do
        if [ "$dat" != "$prv_cenovnik" ]; then
            cat "$datoteka" >> "$prv_cenovnik"
        fi
    done
fi

python3 zad2.py "$prv_cenovnik"