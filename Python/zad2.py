# Да се напише Python скрипта која како аргумент добива име на датотека во која се запишани 
# ценовник на производи. Во посебен ред од датотеката се наоѓа секој еден од производите, притоа во
# редот најпрво е баркодот на производот, потоа одделен со празно место е името на производот, а потоа
# исто така одделен со празно место е сместена цената на производот. Производите треба да се сместат во
# Хеш структура, каде што клуч ќе е баркодот, а како вредност ќе се чува името на производот. Треба да
# се креира и посебна Хеш структура каде како клуч ќе се земе повторно баркодот на производот, а како
# вредност ќе се чува цената на производот. Потоа, скриптата треба да дозволи да се внесуваат непознат
# број на команди од тастатура. Доколку од тастатура се внесе 1, тогаш низ ценовникот се пребарува по
# име на производ, што значи дека после тоа се внесува името на производот кој се пребарува а на екран
# се печати цената на производот (или дека производот не постои). Доколку се внесе 2, се листаат
# производите чија што цена им е помала од таа внесена од тастатура. Доколку се внесе 3, тогаш се
# печатат сите производи (секој производ во посебен ред) со името и цената на производот. Доколку се
# внесе вредност различна од 1, 2 или 3, тогаш се прекинува со командите и скриптата завршува. 

import sys

if len(sys.argv) < 2:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

dat = open(sys.argv[1], "r")

barcodeNames = {} 
barcodePrices = {} 

for line in dat:
    z = line.split() 
    if len(z) == 3:
        barcode = z[0]
        name = z[1]
        price = z[2]
        barcodeNames[barcode] = name
        barcodePrices[barcode] = price

dat.close()

print("Izberete opcija: ")
print("1 - Prebaruvanje po ime")
print("2 - Listanje na proizvodi so cena pomala od vnesenata")
print("3 - Pecatenje na site proizvodi")
print("4 - Izlez")

command = input("Vnesete ja opcijata: ")

if command == "1":
    name = input("Vnesete ime na proizvod: ")
    found = False
    for barcode, prod_name in barcodeNames.items():
        if prod_name.lower() == name.lower(): 
            price = barcodePrices[barcode]
            print(f"Proizvod: {prod_name}, Cena: {price}")
            found = True
            break
    if not found:
        print("Proizvodot ne postoi")

elif command == "2":
    cena = input("Vnesete cena: ")
    found = False
    for barcode, price in barcodePrices.items():
        if price < cena:
            name = barcodeNames[barcode]
            print(f"Proizvod: {name}, Cena: {price}")
            found = True
    if not found:
        print("Nema proizvodi so pomala cena od vnesenata")
    

elif command == "3":
    for barcode, name in barcodeNames.items():
        price = barcodePrices[barcode]
        print(f"Proizvod: {name}, Cena: {price}")

else:
    print("Nevalidna opcija")

