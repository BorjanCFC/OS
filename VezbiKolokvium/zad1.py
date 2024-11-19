#!/usr/bin/python3

# Да се напише Python скрипта која ќе ги испечати првите линии од
# низа датотеки чии имиња се наведени на командна линија. Да
# се промени делимитерот за крај на ред, така што секоја линија
# да завршува со точка.

import sys

if len(sys.argv) == 1:
    print ("Nema datoteki za pecatenje")
    sys.exit()

print("Printing with default delimiter")
for dat in sys.argv[1:]:
    d = open(dat, "r")
    print(d.readlines())
    d.close()

for dat in sys.argv[1:]:
    d = open(dat, "r")
    s = ("".join(d.read())).split('.')
    print(s[0])
    d.close()

