# Да се напише SHELL скрипта која што ќе ја повикува Python скриптата од задача 1. SHELL
# скриптата треба да најде каде се наоѓа во системот датотеката „datoteki.dat“ и датотеката
# „argumenti.dat“. Во датотеката „datoteki.dat“ во секоја линија се сместени датотеките кои треба
# да се пратат на влез на Python скриптата. Во датотеката „argumenti.dat“, во секоја линија
# одделно се сместени аргументите (зборовите) кои што треба да се пратат на Python скриптата.
# SHELL скриптата треба да ја повика Python скриптата за секоја една датотека од листата на
# датотеки во „datoteki.dat“ и да ги прати во продолжение аргументите од „argumenti.dat“. 

#!/bin/bash

if [ ! -f datoteki.dat ] || [ ! -f argumenti.dat ]; then
    echo "Ne se pronajdeni datoteki.dat ili argumenti.dat"
    exit 1
fi

argumenti=$(cat argumenti.dat)

for file in $(cat datoteki.dat); do
    python3 prva.py3 "$file" $argumenti
done