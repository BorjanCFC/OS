# Да се напише Python скрипта која како аргумент од командна линија добива име на датотека.
# Скриптата треба да ја измине датотеката и да провери колку различни валути на пари ќе најде. Знаците
# на валутите кои што се пребаруваат се: „$“ „€“ „£“ „kn“ „kr“ „Rp“ „CHF“. Секоја валута започнува со
# знакот на валутата, по кое што следи сумата на истата (одделена со празно место). Доколку после
# валутата нема бројка, тогаш валутата не се зема во предвид. Сите валути што ќе бидат пронајдени во
# датотеката се собираат и се добива вкупна сума по валута. За секоја валута што била пронајдена во
# датотеката, му се дозволува на корисникот да внесе колку денари е истата (не се внесува за оние валути
# кои што не биле пронајдени во тековната датотека). На крајот од изминувањето на датотеката да се
# отпечати вкупната сума (во денари) што била пронајдена во датотеката. Скриптата, на крај треба да
# отпечати кои валути биле пронајдени. 

# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, имињата
# на оние датотеки кои што имаат помалку од 1000 зборови ги праќа како аргументи од командна линија
# на повик од Python скриптата (сите имиња ги праќа во еден повик). За оние датотеки кои што имаат 
# повеќе од 1000 зборови, креира посебен повик и нивните имиња ги праќа како еден аргумент од
# командна линија. 

import sys

if len(sys.argv) < 2:
    print("Nevaliden broj na argumenti")
    sys.exit(1)


dat = open(sys.argv[1], "r")

valuti = {"$": 0, "€": 0, "£": 0, "kn": 0, "kr": 0, "Rp": 0, "CHF": 0}

for line in dat:
    zborovi = line.split()

    for i in range(len(zborovi) - 1):
        if zborovi[i] in valuti and zborovi[i + 1].isdigit():
            valuti[zborovi[i]] += zborovi[i + 1]

dat.close()

vkupno = 0

for val, iznos in valuti.items():
    if iznos > 0:
        kurs = input(f"Vnesete vrednost na {val} vo denari")
        vkupno += iznos * kurs

print("Pronajdeni valuti: ")
for val, iznos in valuti.items():
    if iznos > 0:
        print(f"{val}: {iznos}")

print(f"Vkupno vo denari: {vkupno}")

# SHELL SKRIPTATA

# #!/bin/bash

# small_files=()
# large_files=()

# for file in *; do
#     if [[ -f "$file" ]]; then
#         word_count=$(wc -w < "$file")

#         if [[ $word_count -lt 1000 ]]; then
#             small_files+=("$file") 
#         else
#             large_files+=("$file") 
#         fi
#     fi
# done

# if [[ ${#small_files[@]} -gt 0 ]]; then # - broj na fajlovi, @ - za da se pristapi do site elemnti od listata
#     python3 zad4.py "${small_files[@]}"
# fi

# if [[ ${#large_files[@]} -gt 0 ]]; then
#     python3 zad4.py "${large_files[@]}"
# fi

