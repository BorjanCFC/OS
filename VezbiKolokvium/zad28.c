// Да се напише програма во C која работи со процеси и нитки. Главната програма (родител процесот)
// пополнува низа од 1000 броеви на случаен начин со помош на rand() функцијата. Потоа, креира два
// деца процеси, така што, двата деца процеси вршат пребарување на бројот 0 во низата од 1000 броеви.
// Првото дете процес, пребарувањето го прави со помош на 10 деца процеси, додека пак второто дете,
// пребарувањето го прави со 10 нитки. Секоја нитка/дете процес добива дел од низата што треба да го
// пребара бројот 0 и печати на екран колку пати е пронајден бројот кај соодветната нитка/дете процес.
// Родител процесот чека да завршат двете деца процеси и на екран печати кое од двете завршило прво.
// Доколку прво заврши дете процесот кое пребарувањето го прави со помош на деца процеси, тогаш на
// екран се печати „pobrzo zavrshi prebaruvanjeto so deca procesi“, инаку печати 
// „pobrzo zavrsi prebaruvanjetoso deca procesi“, инаку печати „pobrzo zavrsi prebaruvanjetopobrzo zavrsi so nitki“. 

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

#define MAX_NITKI 10
#define N 1000

int pole[N]; 
int najdeni[10];   

void *prebaraj(void *t) {
    int id = *((int *)t);
    int counter = 0;
    for(int i = id * 100; i < id * 100 + 100; i++){
        if(pole[i] == 0) {
            counter++;
        }
        najdeni[i] = counter;
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pid_t dete1, dete2;
    pid_t deca[10];
    pthread_t nitki[MAX_NITKI];
    int t[MAX_NITKI];
    int found[10];

    for(int i = 0; i < N; i++){
        pole[i] = rand() % 10;
    }

    dete1 = fork();
    if(dete1 == 0) {
        for(int i = 0; i < 10; i++) {
            deca[i] = fork();
            if(deca[i] == 0) {
                found[i] = 0;
                for(int j = i * 100; j < i * 100 + 100; j++) {
                    if(pole[j] == 0) {
                        found[i]++;
                    }
                }
                printf("DETE Proces %d najde %d pati 0.\n", i, found);
                exit(0);
            }
        }

        for(int i = 0; i < 10; i++) {
            wait(NULL);
        }

        int vkupno = 0;

        for(int i = 0; i < 10; i++) {
            vkupno += found[i];
        }

        printf("DETE1 gi docheka site da zavrshat. Vkupno pronajdeno %d\n", vkupno);
        exit(0);
    }

    dete2 = fork();
    if(dete2 == 0) {
        for(int i = 0; i < MAX_NITKI; i++) {
            t[i] = i;
            pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i]);
        }

        for(int i = 0; i < MAX_NITKI; i++) {
            pthread_join(nitki[i], NULL);
        }

        int vkupno = 0;

        for(int i = 0; i < 10; i++) {
            vkupno += najdeni[i];
        }

        printf("DETE2 gi docheka site nitki da zavrshat. Vkupno pronajdeno %d\n", vkupno);
        exit(0);

    }

    wait(NULL);
    wait(NULL);

    printf("Roditelot doceka dvete deca da zavrshat.\n");

    return 0;


}