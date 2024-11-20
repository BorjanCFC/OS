#Да се напише Python скрипта која добива низа од аргументи од командна линија. Скриптата
#мора да се повика со најмалку два аргументи, инаку се печати порака за грешка.
#Аргументите се имиња на текстуални датотеки. Скриптата треба да ја измине содржината
#на секоја една од датотеките и да направи замена на првиот со последниот, вториот со
#претпоследниот и т.н. Доколку зборовите што се заменуваат се исти (истиот збор) тогаш да
#не се прави замена. Зборовите се одделени еден од друг со празно место. Замената се
#враќа назад во датотеките.

#!/usr/bin/python3

import sys

if len(sys.argv) < 2:
    print("Nedovolen broj na argumenti")
    sys.exit()

for file_path in sys.argv[1:]:
    dat = open(file_path, "r");

    lines = []

    for line in dat.readlines():
        line = line.strip()
        lines.append(line.split(" "))
    
    dat.close();

    modified_lines = []
    
    for z in lines:
        modified_lines.append(" ".join(reversed(z)))
    
    dat = open(file_path, "w")
    dat.write("\n".join(modified_lines))
    dat.close()
