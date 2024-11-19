#!/usr/bin/python3

# Да се напише Python скрипта која од командна линија добива три
# датотеки. Скриптата треба да ги испреплети двете датотеки и
# излезот да го запише во трета датотека. Најпрво ќе се смести
# првата линија од првата датотека, па првата линија од втората и
# т.н. Дополнително, паралелно да се изврши печатење на екран
# излезот што се запишува во третата датотека.

import sys

if len(sys.argv) < 4:
    print ("Nedovolno argumenti")
    sys.exit()

dat1 = open(sys.argv[1], "r")
dat2 = open(sys.argv[2], "r")
dat3 = open(sys.argv[3], "w")

d1 = 0;
d2 = 0;

pole1 = []
pole2 = []

for i in dat1:
    d1 = d1 + 1
    pole1.append(i)

for i in dat2:
    d2 = d2 + 1
    pole2.append(i)

if d1 > d2:
    min = pole2
    max = pole1
else:
    min = pole1
    max = pole2

j = 0

for i in min:
    print(i)
    print(max[j])
    dat3.write(i)
    dat3.write(max[j])
    j = j + 1

for i in range(j, len(max)):
    print(max[i])
    dat3.write(max[i])

dat1.close()
dat2.close()
dat3.close()
