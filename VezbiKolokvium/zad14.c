// Да се напишат две програми во C кои работат со процеси и нитки. Првата програма се
// повикува од конзола и притоа како аргумент (од командна линија) се испраќа број „N“, кој
// кажува колку броеви има во низата и број „К“ кој кажува кој број треба да се пребара.
// Потоа, првата програма ја стартува втората програма како аргумент од командна линија ги
// испраќа вредносите „N“ и „К“. Двата процеси (програми) креираат по 5 нитки кои ја
// пребаруваат вредноста „К“ во низа од „N“ цели броеви. Низата е запишана во датотека со
// име „dat.txt“ притоа се запишани цели броеви помеѓу себе оделени со празно место.
// Главните нитки на двата процеси ја исчитуваат датотеката и ги сместуваат броевите во
// низа. Потоа, секоја од дополнителните 5 нитки од двата процеси генерира случаен индекс
// од низата од N цели броеви и проверува дали е на таа позиција во низата бројот „К“.
// Доколку е, тогаш го убива другиот процес и печати дека тој (прв или втор) процес го нашол
// елементот и печати на која позиција го нашол елементот. Доколку не е тој елемент,
// генерира нов индекс и продолжува со пребарување. Секоја нитка генерира N-пати индекс, и
// доколку после тие генерирања не го најде бројот, завршува со работа. Доколку процесот
// (првиот или вториот) не го најде бројот со сите свои нитки (сите нитки му завршат без да го
// најдат бројот), завршува и тој процес (неговата главна нитка) и печати дека завршил без да
// го најде бројот.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_NITKI 5

int niza[100];
int N, K, found = 0;

void *prebaraj(void *t) {
    int id = *((int *)t);

    for(int i = 0; i < N; i++) {
        if(found) {
            pthread_exit(NULL);
        }

        int index = rand() % N;

        if(niza[index] == K) {
            printf("Proces 1: Nitkata %d go najde brojot %d na pozicija %d\n", id, K, index);
            found = 1;
            kill(0, SIGTERM);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Nedovolen broj na argumenti");
        exit(-1);
    }

    N = atoi(argv[1]);
    K = atoi(argv[2]);

    FILE *file = fopen("dat.txt", "r");
    if(!file) {
        perror("Datotekata ne postoi");
        exit(-1);
    }

    for(int i = 0; i < N; i++) {
        fscanf(file, "%d", &niza[i]);
    }
    fclose(file);

    pthread_t nitki[MAX_NITKI];
    int t[MAX_NITKI];
    int rc1;

    pid_t dete = fork();

    if(dete != 0) {
        wait(NULL);
        printf("Procesot 1 zavrsi\n");
    } else {
        execl("./zad15", "zad15", argv[1], argv[2], NULL);
        perror("Neuspesno startuvanje na vtorata programa\n");
        exit(-1);
    }

    for(int i = 0; i < MAX_NITKI; i++){
        t[i] = i;
        rc1 = pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i]);

        if(rc1) {
            printf("Neuspesno kreiranje na nitka %d \n", i);
            exit(-1);
        }
    }

    for(int i = 0; i < MAX_NITKI; i++) {
        rc1 = pthread_join(nitki[i], NULL);

        if (rc1) {
            printf("ERROR; return code from pthread_join() is %d\n", rc1);
            exit(-1);
        } 
    }

    if(!found) {
        printf("Proces 1 ne go najde brojot %d\n", K);
    }

    return 0;

}