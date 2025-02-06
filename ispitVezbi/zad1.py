# Да се напише Python скрипта која како аргументи добива имиња на две датотеки. 
# Во првата датотека е сместен текст кој треба да се обработи, додека пак, во втората датотека е сместен именик со контакти. 
# Скриптата треба да ја измине првата датотека и да ги најде сите е-маил адреси (формат: kime@firma.com) и да креира команда 
# спремна за испраќање на маил со конкретна порака. Командата се формира во следниов формат:
# mail -s "poraka" kime@firma.com -u Ime Prezime

# За секоја емаил адреса што ќе се пронајде во влезната датотека, се креира ваква наредба, каде на местото од kime@firma.com се сместува 
# конкретната најдена емаил адреса. Во втората датотека, чие што име се добива како влез на скриптата, е сместен именик со познати емаил адреси, 
# така што во секој ред е сместена посебна емаил адреса, заедно со името и презимето на корисникот на емаил адресата (одделени со празно место). 
# Скриптата треба да ја провери секоја пронајдената емаил адреса од првата датотека дали ја има во именикот креиран од втората датотека. 
# Доколку пронајдената емаил адреса ја има во втората датотека, тогаш Ime и Prezime се заменуваат со името и презимето 
# пронајдени во именикот за таа емаил адреса. Инаку, се игнорираат од наредбата. Параметарот "poraka" е текстуална 
# порака чиј што текст е сместен во датотека со име "poraka.txt". Доколку емаил адресата ја има во именикот, тогаш 
# пред текстот од пораката се додава следниот текст: "Pocituvan Ime Prezime,", каде име и презиме се земаат исти од претходно. 
# Доколку емаил адресата ја нема во именикот, тогаш "poraka" останува непроменета. 
# Секоја ваква формирана наредба се печати на екран, притоа не се печати повеќе пати ако се пронајде истата емаил адреса повеќе пати.
# Да се напише Shell скрипта која што ќе провери дали во тековниот директориум се наоѓаат датотеките "poraka.txt" и "kontakti.vcf". 
# Доколку ги има, тогаш ги пребарува сите текстуални датотеки во тековниот директориум (со екстензија .txt, игнорирајќи ја датотеката "poraka.txt") 
# и секоја една датотека ја праќа на влез на Python скриптата заедно со "kontakti.vcf". Секоја наредба што ќе се врати, SHELL скриптата треба да ја изврши.

import sys

if len(sys.argv) < 3:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

dat1 = open(sys.argv[1], "r")  
emails = []

for line in dat1: 
    zborovi = line.split() 
    for z in zborovi:
        if "@" in z and "." in z:
            parts = z.split("@")
            if len(parts) == 2 and "." in parts[1]: 
                emails.append(z)

dat1.close() 

dat2 = open(sys.argv[2], "r")
imenik = []

for line in dat2:
    zborovi = line.split()
    if len(zborovi) == 3: 
        email = zborovi[0]
        ime = zborovi[1]
        prezime = zborovi[2]
        imenik.append((email, ime, prezime))

dat2.close()

poraka_dat = open("poraka.txt", "r")
poraka = poraka_dat.read()
poraka_dat.close()

seen_emails = []

for email in emails:
    if email not in seen_emails: 
        seen_emails.append(email)
        
        found = False
        for z in imenik:
            if z[0] == email:
                ime = z[1]
                prezime = z[2]
                msg = f"Pocituvan {ime} {prezime},\n{poraka}"
                print(f"mail -s \"{msg}\" {email} -u {ime} {prezime}")
                found = True
                break
        
        if not found:
            print(f"mail -s \"poraka\" {email} -u Ime Prezime")


# SHELL SKRIPTATA

# #!/bin/bash

# if [[ -f "poraka.txt" && -f "kontakti.vcf" ]]; then
#     for file in *.txt; do
#         if [[ "$file" != "poraka.txt" ]]; then
#             commands = $(python3 zad1.py "$file" "kontakti.vcf")

#             for cmd in $commands; do
#                 eval $cmd 
#             done
#         fi
#     done
# else
#     echo "Datotekite 'poraka.txt' ili 'kontakti.vcf' gi nema vo tekovniot direktorium"
# fi

