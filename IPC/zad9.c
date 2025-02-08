// Да се напише програма во C која како аргумент од командна линија добива имиња на влезни текстуални
// датотеки. Програмата треба да го најде поединечниот и апсолутниот просек на бројот на букви кои се
// појавуваат во низа од 100 знаци. Поединечниот просек се прави за секоја датотека одделно, додека пак
// апсолутниот просек се преметува како просек од поединечните просеци од секоја датотека.
// Поединечните просеци се прават така што се земаат 100 по 100 бајти од влезната датотека (додека не
// стигне до крај на датотеката) и проверува колку букви имало во секој од тие 100 бајти. На крај од
// датотеката се прави просек од пронајдените букви. Читањето да се прави преку заедничка меморија.

#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>

#define BLOCK_SIZE 100
#define SHM_SIZE 1024  

int broj_bukvi(char *niza, int size) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (isalpha(niza[i])) {
            count++;
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Nevaliden broj na argumenti\n");
        return -1;
    }

    key_t kluc = 1234;
    int id = shmget(kluc, SHM_SIZE, IPC_CREAT | 0666);
    if (id == -1) {
        printf("Nepravilno kreirana memorija\n");
        return -1;
    }
    
    float *mem = (float *)shmat(id, NULL, 0);
    if (mem == (float *)-1) {
        printf("Nepravilno alociranje na memorija\n");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        pid_t dete = fork();
    
        if (dete == 0) {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                printf("Nepravilno otvorena datoteka\n");
                return -1;
            }

            char niza[BLOCK_SIZE];
            int bytes_read;
            int vkupno_bukvi = 0;
            int blok_br = 0;

            while ((bytes_read = read(fd, niza, BLOCK_SIZE)) > 0) {
                vkupno_bukvi += broj_bukvi(niza, bytes_read);
                blok_br++;
            }

            close(fd);

            if (blok_br > 0) {
                mem[i - 1] = (float) vkupno_bukvi / (float) blok_br;
            }

            shmdt(mem);
            return 0;
        }
    }

    for (int i = 1; i < argc; i++) {
        wait(NULL);
    }

    float apsoluten_prosek = 0.0;
    for (int i = 1; i < argc; i++) {
        printf("Datotekata %s ima poedinecen prosek: %.2f\n", argv[i], mem[i - 1]);
        apsoluten_prosek += mem[i - 1];
    }

    printf("Apsolutniot prosek e: %.2f\n", apsoluten_prosek / (argc - 1));

    shmctl(id, IPC_RMID, 0);
    return 0;
}
