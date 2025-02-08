// Да се напише С програма која работи со процеси и меѓупроцесна комуникација. Главниот процес, како
// аргументи од командна линија, добива наредба со аргументи која треба да ја изврши. Потоа, преку
// тастатура се внесува колку пати треба да се изврши соодветната наредба Z. Главниот процес креира Z деца
// процеси колку што треба да се изврши наредбата, притоа секој еден од деца процесите ја извршува
// наредбата еднаш. Иако деца процесите се креираат и се извршуваат паралелно, сепак треба да се овозможи
// механизам каде деца процесите ќе се чекаат помеѓу себе т.е. ќе се извршуваат сериски. Главниот процес
// треба да брои колку време им требало на деца процесите да завршат со извршување. Комуникацијата
// помеѓу деца процесите и синхронизацијата за да се постигне сериско извршување да се изведе со помош на
// заедничка (shared) меморија. 

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Koristenje: %s komanda [argumenti]\n", argv[0]);
        return 1;
    }

    int i, Z, id;
    key_t kluc = 1234;
    int *mem;
    int sekundi = 0; 

    printf("Vnesi ja vrednosta na Z: ");
    scanf("%d", &Z);

    id = shmget(kluc, sizeof(int), 0644 | IPC_CREAT);
    if (id == -1) {
        perror("Neuspesno dobivanje pristap do memorijata");
        return 1;
    }

    mem = (int *)shmat(id, NULL, 0);
    if (mem == (void *)-1) {
        perror("Neuspesno prikacuvanje na memorijata");
        return 1;
    }

    *mem = 0; 
    pid_t deca[Z];

    for (i = 0; i < Z; i++) {
        deca[i] = fork();
        if (deca[i] == -1) {
            perror("Neuspesno kreiranje na proces");
            return 1;
        }

        if (deca[i] == 0) {  
            id = shmget(kluc, sizeof(int), 0644);
            if (id == -1) {
                perror("Neuspesno dobivanje pristap do memorijata vo dete");
                return 1;
            }

            mem = (int *)shmat(id, NULL, 0);
            if (mem == (void *)-1) {
                perror("Neuspesno prikacuvanje na memorijata vo dete");
                return 1;
            }

            while (*mem != i) {
                sleep(1);  
            }

            execlp(argv[1], argv[1], NULL);
            perror("Neuspesno izvrsuvanje na komandata");
            return 1;
        }
    }

    for (i = 0; i < Z; i++) {
        *mem = i;  /
        while (waitpid(deca[i], NULL, WNOHANG) != deca[i]) {
            sleep(1);
            sekundi++; 
        }
    }

    printf("Na decata im trebaa %d sekundi da zavrshat\n", sekundi);

    shmdt(mem);
    shmctl(id, IPC_RMID, NULL);

    return 0;
}
