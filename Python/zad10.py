# Да се напише Python скрипта со име „prva.py“ која како аргументи добива најпрво збор кој ќе го
# пребарува, опција (знаменце) и име на датотека во која ќе се прави пребарувањето. Скриптата треба да
# го пребара зборот (што се праќа како прв аргумент) во зависност од опцијата (што се праќа како втор
# аргумент) во датотеката (чие име се праќа како трет аргумент). Опцијата може да биде „l“, „w“ или „f“,
# притоа опцијата „l“ значи дека треба да се вратат броевите од линиите во кои е пронајден зборот,
# „w“ како резултат враќа колку зборови биле пронајдени пред зборот што се бара, и опцијата „f“ враќа
# колку пати е пронајден зборот во датотеката. Доколку зборот го нема во датотеката, опцијата „l“ враќа -
# 1, додека пак опциите „w“ и „f“ враќаат соодветно вредност 0. Зборовите се одделени еден од друг со
# празно место. Доколку добие невалидна опција, скриптата прекинува со извршување.
# Пример:
# dat.txt:
# Also when you use plain Lorem ipsum text, your design will look like a million other designs out there.
# With Random Text Generator your designs will look more unique while still containing text which truly means
# nothing.
# perl prva.py will l dat.txt
# Резултат: 1 2
# perl prva.py will w dat.txt
# Резултат: 10 24
# perl prva.py will f dat.txt
# Резултат: 2

import sys

if len(sys.argv) < 4:
    print("Nevaliden br na argumenti")
    sys.exit(1)

zbor = sys.argv[1]
zname = sys.argv[2]

if zname not in ["l", "w", "f"]:
    print("Nevalidno zname")
    sys.exit(1)

l = 0
w = 0
f = 0

dat = open(sys.arg[3], "r")

for line in dat:
    line = line.strip()
    zborovi = line.split()

    for z in zborovi:
        if zbor == z:
            if zname == "l":
                print(l)
            elif zname == "w":
                print(w)
            else:
                f += 1
        w += 1
    l += 1

dat.close()

if zname == "f":
    print(f)

if (zname == "l") and (f == 0):
    print(-1)
elif (zname == "w") and (f == 0):
    print(0)
