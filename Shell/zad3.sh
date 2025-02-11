# Да се напише Shell скрипта која како аргумент од командна линија добива еден телефонски број
# (низа од 9 цифри која започнува со 07). Скриптата треба да ја повика Python скриптата од претходно со
# секоја една датотека што ќе ја најде во тековниот директориум со екстензија „.рр“. Скриптата треба да
# провери за секоја една датотека дали воопшто го има телефонскиот број што го добива како аргумент и
# доколку го има, да го отпечати името на корисникот на тој телефонски број.


!/bin/bash

if [[ $# -ne 1]]; then
    echo "Nemate vneseno tel broj"
    exit 1
fi

if [[ echo "$1" | grep -v "^07[0-9][0-9][0-9][0-9][0-9][0-9][0-9]$" ]]; then
    echo "Nevaliden tel broj"
    exit 1

tel_broj=$1
found=0

for file in *.rr; do
    if [[ -f "$file" ]]; then
        result = $(python3 zad3.py "$file" | grep "$tel_broj")

        if [[ -n "$result" ]]; then
            echo "Tel broj: $tel_broj e pronajden vo dat $file"
            echo "$result"
            found=1
        fi
    fi
done

if [[ $found -eq 0] ]; then
    echo "Tel broj $tel_broj ne e pronajden"
fi