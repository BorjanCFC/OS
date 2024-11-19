# Да се напише PERL скрипта која како аргумент добива име на влезна датотека. 
# Скриптата треба да креира излезна датотека со исто име како и влезната, само со наставка "_out". 
# Скриптата треба да ја измине влезната датотека линија по линија и во излезната датотека да ја 
# запише линијата, така што доколку во линијата текст се наоѓа датум, датумот да дојде прв во линијата, па 
# потоа без менување на редоследот да дојде и останатиот текст од линијата. Форматот на датумот е "DD-MM-YYYY", 
# при што, доколку има повеќе од еден датум, се запишуваат во излезната датотека онолку линии 
# колку шшто има различни датуми во влезната датотека притоа текстот што следи е без датуми. Доколку скриптата се повика
# со втор аргумент и доколку тој е валиден датум, тогаш дополнително да се испечатат линиите кои го имаат тој датум.
# Пример: 
# Vlez.txt 
# Prv den od godinata 01-01-2021 dodeka pak posleden den e 31-12-2021 
# Posleden den za Fevruari e 28.02.2021 godina. 

# Vlez.txt_out 
# 01-01-2021 Prv den od godinata dodeka pak posleden den e 
# 31-12-2021 Prv den od godinata dodeka pak posleden den e 
# Posleden den za Fevruari e 28.02.2021 godina.

#!/usr/bin/python3

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