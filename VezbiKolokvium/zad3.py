# Да се напише Python сктипта која како аргументи добива имиња на две датотеки.
# Скриптата треба да ја измине втората датотека, да ги детектира сите наредби кои што се наоѓаат во
# втората датотека и секоја наредба заедно со аргументите да ја испише во посевна линија на екран.
# Секоја наредба започнува со името на наредбата, после кое следат низад аргументи (или нема ниту еден аргуемнт).
# Имињата на наредбите се сместени во првата датотека т.е датотеката што се испраќа како прв аргумент. Секоја
# наредба е сместена во нов ред во првата датотека.

#!/usr/bin/python3

import sys

if len(sys.argv) < 3:
    print("Nedovolen broj na argumenti")
    sys.exit()

dat = open(sys.argv[1], "r")
naredbi = []

for l in dat.readlines():
    naredbi.append(l.strip())

dat.close()

com = ""

d = open(sys.argv[2], "r")

for l in d.readlines():
    l.strip()

    zborovi = l.split(" ")

    for z in zborovi:
        if z in naredbi:
            if com:
                print(com)
            com = z
        else:
            com = com + " " + z

print(com)

d.close()