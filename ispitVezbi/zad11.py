# Да се напише Python скрипта која како аргумент добива име на датотека во која се запишани 
# контакти од даден именик. Во посебен ред од датотеката се наоѓа секој еден од контактите, притоа во
# редот најпрво е редниот број на контактот, потоа одделен со празно место е телефонскиот број, а потоа
# исто така одделен со празно место се сместени името и презимето. Контактите треба да се сместат во
# Хеш структура, каде што клуч ќе е телефонскиот број, а како вредност ќе се чува името и презимето на
# контактот. Потоа, скриптата треба да дозволи да се внесуваат непознат број на команди од тастатура.
# Доколку од тастатура се внесе 1, тогаш низ именикот се пребарува по телефонски број, што значи дека
# после тоа се внесува телефонскиот број кој се пребарува а на екран се печати името и презимето на тој
# телефонски број (или дека контактот не постои). Доколку се внесе 2, тогаш се пребарува по име и
# презиме и на екран се печати телефонскиот број за дадено име и презиме внесени од тастатура. Доколку
# се внесе 3, се печати целиот именик со броевите придружени со име и презиме. Доколку се внесе
# вредност различна од 1, 2 или 3, тогаш се прекинува со командите и скриптата завршува. 

# Да се напише Shell скрипта која што ги наоѓа сите именици во тековниот директориум т.е. ги
# бара сите датотеки кои што завршуваат на екстензија .vcf. Доколку има повеќе именици, тогаш на крајот
# во првиот именик од листата именици се додава содржината на другите именици и се повикува Python
# скриптата со името на првиот именик. 

import sys

if len(sys.argv) < 2:
    print("Nevaliden broj na argumenti")
    sys.exit(1)

dat = open(sys.argv[1], "r")

tel_broevi = {}

for line in dat:
    zborovi = line.split()

    if len(zborovi) >= 3:
        telefon = zborovi[1]
        if len(telefon) == 9 and telefon.isdigit() and telefon.startswith("07"):
            ime_prezime = " ".join(zborovi[2:])
            tel_broevi[telefon] = ime_prezime

dat.close()

while True:
    print("1 - Prebaruvanje po tel broj")
    print("2 - Prebaruvanje po ime i prezime")
    print("3 - Pecatenje na celiot imenik}")

    komanda = input("Izberi opcija:" )

    if komanda == "1":
        telefon = input("Vnesi tel broj: ")
        if telefon in tel_broevi:
            print(f"Ime i prezime: {tel_broevi[telefon]}")
        else:
            print("Tel broj ne postoi.")
    
    elif komanda == "2":
        ime_prezime = input("Vnesi ime i prezime: ")
        found = False
        for telefon, ime in tel_broevi.items():
            if ime == ime_prezime:
                print(f"Tel broj: {telefon}")
                found = True
                break
        if not found:
            print("Tel broj ne postoi")
    
    elif komanda == "3":
        if tel_broevi:
            for telefon, ime in tel_broevi.items():
                print(f"Tel broj: {telefon}, Ime i prezime: {ime}")
    
    else:
        print("Nevalidna opcija")
        break

# SHELL SKRIPTATA

# #!/bin/sh

# imenici=$(ls | grep '\.vcf$')
# kolku=$(echo "$imenici" | wc -l)

# if [ $kolku -gt 1 ]; then
#     prv=$(echo "$imenici" | head -n 1) 

#     for imenik in $(echo "$imenici" | tail -n +2); do
#         cat "$imenik" >> "$prv" 
#     done
# fi

# python3 zad11.py "$prv"
