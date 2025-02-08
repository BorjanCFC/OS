// Да се напише програма во C која како аргумент од командна линија добива имиња на влезни текстуални
// датотеки. Програмата треба да го најде поединечниот и апсолутниот просек на должина на низа
// најблиску поголема од 100 бајти во која ќе нема пресекување на низа од букви. Поединечниот просек се
// прави за секоја датотека одделно, додека пак апсолутниот просек се преметува како просек од
// поединечните просеци од секоја датотека. Поединечните просеци се прават така што се земаат 100 по
// 100 бајти од влезната датотека (додека не стигне до крај на датотеката) и проверува дали на крајот од
// 100-те бајти има букви или не. Доколку 100-те бајти завршуваат на знак различен од бува, тогаш се
// земаат 100 бајти дека се доволни за тој дел, но доколку 100-те бајти завршуваат на буква, тогаш на тие
// 100 бајти се додаваат останатите букви додека не се стигне до знак различен од буква. На крај од
// датотеката се прави просек од должините детектирани за целата датотека. Читањето да се прави преку мемориско мапирање на датотеките.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define BLOCK_SIZE 100

int presmetaj_dolzina(char *data, int size) {
    int i = BLOCK_SIZE;
    while (i < size && isalpha(data[i])) {
        i++;
    }
    return i;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return -1;
    }

    float apsoluten_prosek = 0.0;

    for (int i = 1; i < argc; i++) {
        pid_t dete = fork();
        
        if (dete == 0) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                perror("Nepravilno otvorena datoteka");
                return -1;
            }

            int file_size = lseek(fd, 0, SEEK_END);
            if (file_size == -1) {
                perror("lseek failed");
                close(fd);
                return -1;
            }
            lseek(fd, 0, SEEK_SET);

            char *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (data == MAP_FAILED) {
                perror("mmap failed");
                close(fd);
                return -1;
            }

            int vkupno_dolzina = 0, blok_br = 0;
            for (int j = 0; j < file_size; j += BLOCK_SIZE) {
                int dolzina = presmetaj_dolzina(data + j, file_size - j);
                vkupno_dolzina += dolzina;                                      
                blok_br++;
            }

            munmap(data, file_size);
            close(fd);

            if (blok_br > 0) {
                printf("Datotekata %s ima poedinecen prosek: %.2f\n", argv[i], (float)vkupno_dolzina / blok_br);
            } 
        }
    }


    printf("Apsolutniot prosek e: %.2f\n", apsoluten_prosek / (argc - 1));

    return 0;
}
