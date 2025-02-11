# Да се напише Shell скрипта која што ги наоѓа сите именици во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе именици, тогаш на крајот
# во првиот именик од листата именици се додава содржината на другите именици и се повикува Python
# скриптата со името на првиот именик. 

# #!/bin/sh

imenici=$(ls *.vcf)
kolku=$(echo "$imenici" | wc -l)

if [ $kolku -gt 1 ]; then
    prv=$(echo "$imenici" | head -n 1) 

    for imenik in $(echo "$imenici" | tail -n +2); do
        cat "$imenik" >> "$prv" 
    done
fi

python3 zad11.py "$prv"