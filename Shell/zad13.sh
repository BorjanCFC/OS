# Да се напише SHELL скрипта која како аргумент од командна линија добива име на влезна датотека.
# Доколку не се прати аргумент на командна линија, тогаш се пребарува датотека со име „naredbi.txt“ во
# тековниот директориум и неговите поддиректориуми и доколку се најде, тогаш се копира таа датотека
# во тековниот директориум (доколку истата не е во тековниот директориум). Потоа, скриптата пребарува
# во тековниот директориум и неговите поддиректориуми датотека со име „аctions.txt“. Доколку најде, ја
# повикува Python скриптата од првата задача, каде како прв аргумент се праќа датотеката „naredbi.txt“, а
# како втор аргумент се праќа датотеката „actions.txt“, притоа втората датотека се праќа со апсолутната
# патека каде ќе биде пронајдена. Секоја една наредба што ќе се врати од извршувањето на Python
# скриптата се извршува од страна на SHELL скриптата. Доколку наредбата врати некаков текст од
# извршувањето, тогаш се печати името на наредбата и грешка во продолжение. Доколку не врати текст,
# тогаш се печати името на наредбата и текстот „ОК“ во продолжение.

# #!/bin/bash

if [ $# -lt 1]; then
    dat=$(find . -name "naredbi.txt")
    if [ -n "$dat"]; then
        $(cp "$dat" .)
        actions=$(find . -name "actions.txt")
        if [ -n "$actions"]; then
            execute=$(./prva.py3 naredbi.txt "$actions")
            for command in $execute; do
                tekst=$($command)
                if [ -n "$tekst"]; then
                    echo "OK $command"
                else
                    echo "ERROR $command"
                fi
            done
        fi
    fi
else 
    execute=$(./prva.py3 naredbi.txt "$1")
    for command in $execute; do
                tekst=$($command)
                if [ -n "$tekst"]; then
                    echo "OK $command"
                else
                    echo "ERROR $command"
                fi
            done
fi