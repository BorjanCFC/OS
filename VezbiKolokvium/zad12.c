// Да се напише програма во C која работи со процеси и нитки. Главната програма
// (родител процесот) треба да креира дете процес, на кого ќе му прати низа од 100
// цели броеви. Дете процесот најпрво треба да ја пополни низата од 100 цели
// броеви со нули. Потоа, дете процесот треба да креира N нитки (N се внесува од
// тастатура во родител процесот), притоа на секоја нитка дете процесот и испраќа
// (како аргумент) случаен позитивен цел број „К“ (најмногу 500). Секоја нитка
// прави „К“ промени во низата и потоа завршува со работа. Секоја една промена
// во низата значи случајно одбирање на еден елемент од низата и менување на
// неговата вредност. Првата половина од нитките ја менуваат вредноста на
// елементот со зголемување на неговата вредност за 1, додека пак втората
// половина на нитките ја намалуваат вредноста на елементот за 1. Откако ќе
// завршат со работа сите нитки, главната нитка (дете процесот) печати на екран
// колку елементи од низата ја имаат променето својата вредност (т.е. не се повеќе
// нула). Родител процесот завршува откако дете процесот ќе заврши. Генерирањето
// на случајни броеви се прави со помош на функцијата rand( ).

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MAX_NITKI 10

int niza[100];

typedef struct {
    int id;
    int k;
    int z;

} info_t;

void *generiraj(void *t) {
    info_t id = *((info_t *)t);
    for(int i = 0; i < id.k; i++) {
        int index = rand() % 100;
        if(id.z) {
            niza[index]--;
        } else {
            niza[index]++;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    int N;
    int rc1;
    pid_t dete;
    printf("Vnesi go N\n");
    scanf("%d", &N);
    dete = fork();

    if(dete != 0) {
        wait(NULL);
        printf("Zavrsuva roditel procesot\n");
    } else {
        for(int i = 0; i < 100; i++) {
            niza[i] = 0;
        }

        pthread_t nitki[N];
        info_t t[N];

        for(int i = 0; i < N; i++) {
            t[i].id = i;
            t[i].k = rand() % 501;

            if(i < N / 2) {
                t[i].z = 1;
            } else {
                t[i].z = 0;
            }

            rc1 = pthread_create(&nitki[i], NULL, generiraj, (void *)&t[i]);

            if(rc1) {
                printf("Ne uspea da se kreira nitkata %d\n", i);
                return 0;
            }
        }

        for(int i = 0; i < N; i++) {
            rc1 = pthread_join(nitki[i], NULL);
            if (rc1) {
                printf("ERROR; return code from pthread_join() is %d\n", rc1);
                exit(-1);
            } 
        }

        int kolku = 0;

        for(int i = 0; i < 100; i++) {
            printf("%d\t", niza[i]);

            if(niza[i] != 0) {
                kolku++;
            }
        }

        printf("\n Ima %d takvi broevi\n", kolku);
    }

    return 0;

}