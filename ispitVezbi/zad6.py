# Да се напише Python скрипта која како аргументи од командна линија добива имиња на датотеки.
# Скриптата треба да проверува две по две датотеки (секоја датотека со секоја останата датотека) и да
# споредува колку исти Хаштагови има (збор составен со букви и бројки кој започнува со специјалниот
# знак #). Истите Хаштагови и вкупниот број на Хаштагови се запишува во излезна датотека со име
# креирано од спојување на имињата на двете датотеки кои што се споредуваат. На крај, на екран се
# печати кој е најчесто користениот Хаштаг во сите датотеки.

# Да се напише SHELL скрипта која ги листа сите датотеки од тековниот директориум. Потоа, зема две по
# две датотеки од листата датотеки и ги праќа како влезни аргументи на повикот на Python скриптата од
# задача 1. Резултатот од секој повик на Python Скриптата се запишува во датотека креирана од спој на
# имињата на двете датотеки кои се праќаат на Python Скриптата, притоа се запишува во продолжение
# (append)

import sys

if len(sys.argv) < 3:
    print("Nedovolen broj na argumenti")
    sys.exit(1)

all_hashtags = []

dats = sys.argv[1:]

for dat in dats:
    with open(dat, "r") as file:
        text = file.read()
        words = text.split()
        hashtags = []
        for word in words:
            if word.startswith("#"):
                hashtags.append(word)
        all_hashtags.append(hashtags)

for i in range(len(all_hashtags)):
    for j in range(i + 1, len(all_hashtags)):
        file1_hash = all_hashtags[i]
        file2_hash = all_hashtags[j]

        common_hashtags = []
        for hashtag in file1_hash:
            if hashtag in file2_hash and hashtag not in common_hashtags:
                common_hashtags.append(hashtag)

        with open("izlez.txt", "w") as output_file:
            output_file.write(f"Istite hashtags megju {dats[i]} i {dats[j]}: ")
            for hashtag in common_hashtags:
                output_file.write(f"{hashtag}")

hashtag_count = {}
for hashtags in all_hashtags:
    for hashtag in hashtags:
        if hashtag not in hashtag_count:
            hashtag_count[hashtag] = 1
        else:
            hashtag_count[hashtag] += 1


most_common_hashtag = None
max_count = 0
for hashtag, count in hashtag_count.items():
    if count > max_count:
        max_count = count
        most_common_hashtag = hashtag

print(f"Najcesto koristen hashtag e {most_common_hashtag} se pojavuva {max_count} pati")

# SHELL SKRIPTATA

# #!/bin/bash

# files=(*)

# if [ ${#files[@]} - lt 2]; then # - broj na fajlovi, @ - za da se pristapi do site elemnti od listata
#     echo "Nema dovolno datoteki vo tekovniot dir"
#     exit 1
# fi

# for((i=0; i<${#files[@]}; i++)); do
#     for (( j=i+1; j<${#files[@]}; j++ )); do
#         file1="${files[$i]}"
#         file2="${files[$j]}"

#         if [[ $file1 == *.txt && $file2 == *.txt]]; then
#             output_file="output_${file1%.*}_${file2%.*}.txt"
#             python3 zad6.py "$file1" "$file2" >> "$outpu_file"
#         fi
#     done
# done