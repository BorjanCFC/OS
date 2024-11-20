# Да се напише SHELL скрипта која како аргумент добива име на Python скрипта, 
# збор за пребарување и листа од имиња на датотеки. Доколку не добие како прв 
# аргумент име на Python скрипта (аргумент што завршува на .py3), тогаш зема 
# како име на скрипта „prva.py3“. SHELL скриптата треба да провери дали има 
# Python скрипта со такво име во тековниот директориум или поддиректориумите 
# и доколку нема, завршува SHELL скриптата со извршување. Доколку постои Python 
# скрипта со такво име (тековниот директориум или поддиректориумите), тогаш SHELL 
# скриптата треба со помош на Python скриптата, да најде и отпечати на екран, колку 
# пати ќе го пронајде зборот (што се праќа како втор аргумент на SHELL) за сите датотеки 
# што се праќаат како аргументи (од трет аргумент па натаму). 
# На крај скриптата треба да отпечати колку вкупно бил пронајден зборот во сите датотеки.

# Пример:
# ./prva.sh prva.py3 will dat1.txt dat2.txt dat3.txt
# dat1.txt 7
# dat2.txt 0
# dat3.txt 12
# Vkupno: 19

#!/bin/bash

dali=$(echo $1 | grep .py3)
dat=""
zbor=""

if [ -z "$dali" ] 
then
    dat="prva.py3"
    zbor=$1
    shift
else
    dat=$1
    zbor=$2
    shift
    shift
fi

kolku=0
for d in $@
do
    k=$(./$dat $zbor f $d)
    kolku=$(expr $kolku + $k)
done

echo $kolku
