# Да се напише Shell скрипта која што најпрво ќе ги најде датотеките „naredbi.txt“ и „vlez.txt“
# во системот и ќе ги ископира во тековниот директориум. Потоа, ќе ја повика perl скриптата
# и ќе ги изврши една по една наредбите што се враќаат како одговор од PERL скриптата.
# Резултатот од извршување на секоја една од наредбите се печати во излезна датотека
# „izlez.txt“, во која што преку прилепување (append) го додава излезот на секоја една од
# наредбите.

p = $(find / -name "naredbi.txt")
z = $(find / -name "vlez.txt")

if [[-z $p || -z $z]]; then
    echo "Ne postoi nekoja od datotekite"
else
    cp $p .
    cp $z .
    naredbi=$(perl perl.plx naredbi.txt vlez.txt)

    for n in $naredbi; do
        $n >> izlez.txt
    done
fi