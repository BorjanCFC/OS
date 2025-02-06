# Да се напише Python скрипта која како аргумент добива име на влезна датотека. Скриптата треба да
# креира излезна датотека со исто име како и влезната, само со наставка “_out”. Скриптата треба да ја
# измине влезната датотека линија по линија, и во излезната датотека да ја запише линијата, така што
# доколку во линијата текст се наоѓа датум, датумот да дојде прв во линијата, па потоа, без менување на
# редоследот, да дојде и останатиот текст од линијата. Форматот на датумот е “DD-MM-YYYY”, притоа,
# доколку има повеќе од еден датум, се запишуваат во излезната датотека онолку линии колку што има
# различни датуми во влезната линија, притоа текстот што следи е без датуми. Доколку скриптата се
# повика со втор аргумент и доколку вториот аргумент е валиден датум, тогаш дополнително и на екран
# се печатат оние линии каде што го има тој датум. Зборовите во датотеката се одделени со празно место.
# Пример:
# Vlez.txt
# Prv den od godinata 01-01-2021 dodeka pak posleden den e 31-12-2021
# Posleden den za Fevruari e 28.02.2021 godina.
# Vlez.txt_out
# 01-01-2021 Prv den od godinata dodeka pak posleden den e
# 31-12-2021 Prv den od godinata dodeka pak posleden den e
# Posleden den za Fevruari e 28.02.2021 godina.

# Да се напише SHELL скрипта која како аргумент добива листа од имиња од датотеки. Скриптата, за
# секоја датотека (име од датотека) да провери дали ја има како датотека во тековниот директориум, или
# под-директориуми. Доколку ја има, проверува дали датотеката (во нејзината содржина) има барем еден
# валиден датум (DD-MM-YYYY), и доколку нема, ја игнорира. Доколку има валиден датум, тогаш ја
# повикува Python скриптата и како прв аргумент го праќа името на датотеката, и т.н. со сите датотеки што
# се пратени како аргументи од командна линија. За секоја датотека што ќе биде пратена
# на Python скриптата, се печати на екран нејзиното име и колку различни датуми се пронајдени.


import sys

if len(sys.argv) < 2:
    print("Nedovolen broj na argumenti")
    sys.exit()

flag = 0
dat = sys.argv[1]

if len(sys.argv) > 2:
    if len(sys.argv[2].split("-")) == 3:
        flag = 1

d = open(dat, "r")
pole = []

for l in d.readlines():
    l = l.strip()

    zborovi = l.split(" ")
    line = ""
    p = []

    for z in zborovi:
        if len(z.split("-")) == 3: 
            p.append(z)
        else:
            line = line + " " + z 

    if len(p) == 0:
        print(line)

    for z in p:
        print(z + " " + line)

    if flag == 1:
        if sys.argv[2] in p:
            pole.append(line)

if flag == 1:
    for z in pole:
        print(sys.argv[2] + " " + z)

d.close()

# SHELL SKRIPTATA

# #!/bin/bash

# if [ $# -lt 1 ]; then
#     echo "Nevaliden br na argumenti"
#     exit 1
# fi

# for dat in "$@"
# do
#     jaIma=$(find . -name "$dat")
    
#     if [ -n "$jaIma" ]; then

#        valid=$(cat $jaIma | grep [1-31]\-[1-12]\-[0-9][0-9][0-9][0-9])
        
#         if [ -n "$valid" ]; then

#             count=$(echo "$valid" | wc -l)
#             echo "Datotekata $dat sodrzi $count razlicni datumi."

#             python3 zad12.py "$jaIma"
#         fi
#     else
#         echo "Datotekata $dat ne postoi"
#     fi
# done
