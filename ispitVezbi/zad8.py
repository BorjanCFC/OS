# # Да се напише Python скрипта со име „prva.py3“ која како аргументи добива две влезни датотеки. Првата
# датотека е код напишан во Python, додека пак втората датотека е документација која треба да се вметне
# во кодот. Документацијата е организирана така што секоја линија од датотеката најпрво има број, а
# потоа текст. Скриптата треба да ги спои двете датотеки, така што втората датотека (документацијата)
# треба да ја додаде во програмскиот код (првата датотека). Додавањето се прави така што бројката на
# почеток на секоја линија во документацијата кажува на која линија во кодот треба да биде истата
# вметната, а потоа, на крајот на линијата во кодот, се додава линијата од документацијата, започнувајќи
# со #. Новоформираниот код се печати на екран.
# Пример:
# ./prg dat.py3 dok.co
# Dat.py3:
# for i in range(len(X)):
#  for j in range(len(X[0])):
#  result[i][j] = X[i][j] + Y[i][j]
# for r in result:
#  print(r)
# dok.co
# 6 #print the result
# 2 #iterate columns
# 1 #iterate rows
# 3 #sum up
# На екран:
# for i in range(len(X)):#iterate tows
#  for j in range(len(X[0])):#iterate columns
#  result[i][j] = X[i][j] + Y[i][j]#sum up
# for r in result:
#  print(r)#print the result

# Да се напише SHELL скрипта која треба да ги најде сите датотеки (во тековниот директориум и
# поддиректориумите) кои имаат наставка (екстензија) „.py3“. Потоа, скриптата треба за секоја
# датотека со наставка „.py3“ да најде датотека со исто име како и датотеката само со екстензија 
# „.со“. (Пример, ако најде датотека со име prvadat.py3, пребарува дали има датотека со име
# prvadat.co (во тековниот директориум и поддиректориумите). Скриптата, за секој пар датотеки
# што ќе ги најде датотека со наставка „.py3“ и датотека со исто име само со наставка „.co“, ја 
# повикува Python скриптата од првата задача и како аргументи им ги праќа ове две датотеки.


import sys

if len(sys.argv) < 3:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")

komentari = {}

for line in dat2:
    zborovi = line.split()

    if len(zborovi) > 1:
        broj = int(zborovi[0])
        tekst = " ".join(zborovi[1:])
        komentari[broj] = tekst



kod_linii = dat1.readlines()

for br_line in range(1, len(kod_linii) + 1):
    line = kod_linii[br_line - 1].strip()

    if br_line in komentari:
        line += komentari[br_line]

    print(line)

dat1.close()
dat2.close()

# SHELL SKRIPTATA

# #!/bin/bash

# for py3_file in $(find . -type f -name "*.py3"); do
#     base_name=$(basename "$py3_file" .py3)

#     co_file="./$base_name.co"
    
#     if [ -f "$co_file" ]; then
#         python3 prva.py3 "$py3_file" "$co_file"
#     fi
# done