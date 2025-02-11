# Да се креира Shell скрипта која ќе врши статистика на буџетот на даден корисник. Скриптата
# може да се повика со неколку знаменца во произволен редослед. Доколку се повика со знаменце
# „-m“ во продолжение треба да се наведе за кој месец се прави статистиката и да се пронајдат
# сите датотеки со екстензија „.bill“ од тој месец и да се повика Python скриптата, така што како
# аргументи ќе се пратат тие датотеки. Доколку не се наведе „-m“ знаменцето, тогаш се прави за
# сите месеци. Доколку скриптата се повика со знаменце „-d“ тогаш во продолжение се наведува
# директориумот од каде да се пребаруваат „.bill“ датотеките. Доколку не се наведе знаменце „-d“,
# тогаш се пребарува во домашниот (Home) директориум на корисникот. Доколку погрешно се
# наведат знаменцата, да се отпечати порака за грешка. 

#!/bin/bash

month=""
directory="$HOME"
argMonth=0
argDir=0

for arg in "$@"; do
    if [ "$argMonth" -eq 1 ]; then
        month="$arg"
        argMonth=0
    elif [ "$argDir" -eq 1 ]; then
        directory="$arg"
        argDir=0
    elif [ "$arg" = "-m" ]; then
        argMonth=1
    elif [ "$arg" = "-d" ]; then
        argDir=1
    else
        echo "Greshka"
        exit 1
    fi
done

if [ ! -d "$directory" ]; then
    echo "Greshka $directory ne postoi"
    exit 1
fi

if [ -n "$month" ]; then
    files=$(find "$directory" -type f -name "*$month*.bill")
else
    files=$(find "$directory" -type f -name "*.bill")
fi

if [ -z "$files" ]; then
    echo "Нема .bill датотеки за анализирање."
    exit 0
fi

python prva.py $files
