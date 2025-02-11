# Да се напише Python скрипта која како аргументи од командна линија добива листа од кориснички
# имиња. Секое корисничко име започнува со специјалниот знак „@“ а потоа следи листа од букви и
# бројки. Невалидните имиња од командна линија да се игнорираат. Скриптата дозволува да бидат
# внесени непознат број на имиња на датотеки од тастатура (прекинува кога ќе се внесе зборот „kraj“).
# Скриптата прави статистика за секоја една датотека, притоа статистиката вклучува секое едно
# корисничко име, во која линија од датотеката се појавува. Скриптата на екран ги печати (во секој ред
# одделно) во кој ред било пронајдено корисничкото име (бројот на редот) и кои други кориснички имиња
# биле пронајдени во истиот ред.

import sys

if len(sys.argv) < 2:
    print("Nevaliden br na argumenti")
    sys.exit(1)

valid_usernames = []

for username in sys.argv[1:]:
    if username.startswith("@") and username[1:].isalnum():
        valid_usernames.append(username)

if not valid_usernames:
    print("Nema validni usernames")
    sys.exit(1)

dats = []

while True:
    dat = input("Vnesete ime na dat (ili 'kraj' za kraj): ")
    if dat.lower() == "kraj":
        break
    dats.append(dat)

for dat in dats:
    with open(dat, "r") as file:
        line_num = 1
        for line in file:
            found_usernames = []
            words = line.split()
            for username in valid_usernames:
                if username in words:
                    found_usernames.append(username)
            
            if found_usernames:
                print(f"Vo red {line_num} se naogjaat usernames: ".join(found_usernames))
            line_num += 1

