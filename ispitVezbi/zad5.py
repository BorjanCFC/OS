# Да се напише Python скрипта која како аргументи од командна линија добива 2 датотеки (имиња на
# датотеки). Доколку има помалку од 2 датотеки, тогаш треба да се испише порака за грешка и да заврши
# скриптата. Првата датотека содржи парови зборови, по два зборови во секој ред посебно, втората
# датотека е датотека со случаен текст. Паровите зборови на првата датотека се граничници т.е. првиот
# збор е почетната граница а вториот збор е крајот. Скриптата треба да ги најде и отпечати сите зборови
# од втората датотека кои се наоѓаат од првиот граничен збор до вториот и да ги отпечати на екран. Ова
# треба да се направи за сите парови гранични зборови од првата датотека. Доколку некој од граничните
# зборови го нема во втората датотека или првиот не е пред вториот, во тој случај не се печати за тој пар
# гранични зборови ништо.

# Пример:
# Dat1.txt:
# prv vtor
# most koj
# tri kraj
# Dat2.txt
# Sekoj prv den sedime na most pokraj reka, I sekoj vtor od nas e koj od koj od elitata. Na kraj site se vratija na
# rabota

# На екран:
# prv den sedime na most pokraj reka, I sekoj vtor
# most pokraj reka, I sekoj vtor od nas e koj

# Да се напише Shell скрипта која како аргументи од командна линија добива имиња на два директориуми,
# притоа доколку се повика без аргументи, скриптата испишува порака за грешка, додека пак, доколку се
# повика со еден аргумент, вториот се смета дека е тековниот директориум. Скриптата треба да ја земе првата
# текстуална датотека од двата директориуми (со екстензија .txt) и да ја повика Python скриптата, со тоа што
# како прв аргумент ќе се прати датотеката од првиот директориум, додека како втор аргумент датотеката од
# вториот директориум. Доколку двата директориуми се исти, тогаш вториот директориум се заменува со
# тековниот директориум. На крај да се отпечати резултатот од извршување на Python скриптата.

import sys

if len(sys.argv) < 3:
    print("Greshka")
    sys.exit(1)

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")

tekst = dat2.read().split()
words = []

for line in dat1:
    zborovi = line.split()

    if len(zborovi) < 2:
        continue

    start = zborovi[0]
    end = zborovi[1]

    if start in tekst and end in tekst:
        start_index = tekst.index(start)
        end_index = end.index(end)

        if start_index < end_index:
            linija = tekst[start_index:end_index + 1]
            words.append(" ".join(linija))

for w in words:
    print(w)

# SHELL SKRPITATA 

# #!/bin/sh
# prvD=''
# vtorD=''
# if [ $# -eq 0 ] then
# exit 'poraka za greska'
# elif [ $# -eq 1 ] then
# prvD=$1
# vtorD='.'// $PWD
# else 
# prvD=$1
# vtorD=$2
# fi
# if [ $prvD -eq $vtorD ] then
# vtorD='.'// $PWD
# fi
# dat1 = $(ls -al $prvD | grep *.txt | head -1)
# dat2 = $(ls -al $vtorD | grep *.txt | head -1)

# echo $(zad5.py $dat1 $dat2)