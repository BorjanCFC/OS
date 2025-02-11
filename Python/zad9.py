# Да се напише Python скрипта со име „prva.py3“ која како аргументи добива име на влезна датотека и
# опционален втор аргумент. Во влезната датотека има мени, така што во секој ред одделно има по едно
# јадење од менито во формат reden_broj ime_na_jadenje kategorija_na_jadenje cena. Секој дел е одделен
# помеѓу себе со празно место. Доколку корисникот не внесе втор аргумент, тогаш се печати целото мени,
# но доколку внесе аргумент, тогаш ги печати само оние јадења чија категорија е иста со вториот
# аргумент (ги филтрира по категорија внесена како втор аргумент од командна линија). Потоа скриптата
# дозволува да се внесе од тастатура три јадења од менито преку внесување на нивниот реден број. На
# крај скриптата треба да ги отпечати имињата на одбраните јадења од корисникот (само оние кои ги има
# на менито што е претходно отпечатено) и вкупната сума за наплата (вкупната цена).
# Пример:
# ./python3 meni.py3 dorucek
# На екран:
# 1 omlet dorucek 150
# 7 poshirani dorucek 120
# 20 mekici dorucek 70
# 25 angliski dorucek 350
# Од тастатура:
# 1 10 25
# На екран:
# Omlet
# Angliski
# Vkupno: 500 

import sys

if len(sys.argv) < 2:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

category_filter = None
if len(sys.argv) == 3:
    category_filter = sys.argv[2]

dat = open(sys.argv[1], "r")

ceni = {}
meni = {}

for line in dat:
    zborovi = line.split()
    if len(zborovi) < 4:
        continue

    rbr = zborovi[0]
    ime = zborovi[1]
    category = zborovi[2]
    cena = int(zborovi[3])

    meni[rbr] = ime
    ceni[rbr] = cena

    if category_filter is None or category == category_filter:
        print(f"{rbr} {ime} {category} {cena}")


selected_items = input("Izberete 3 jadenja (vnesete nivni redni br)").split()

vkupno = 0

chosen_dishes = []

for item in selected_items:
    if item in meni:
        chosen_dishes.append(meni[item])
        vkupno += ceni[item]

for dish in chosen_dishes:
    print(dish)

print(f"Vkupno: {vkupno}")

