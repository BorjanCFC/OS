// Да се напише програма за пребарување на
// најголемиот елемент во низа од 1000
// случајно генерирани броеви. Програмата
// треба да креира 10 нитки кои што најпрво ќе
// генерираат 1000 броеви (секоја по 100).
// Потоа, треба да се креираат нови 10 нитки
// кои што ќе ги изминат тие 1000 (секоја
// своите 100) и ќе го најдат најголемиот
// елемент. На крај, главната програма треба да
// го испише најголемиот генериран број. 

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
 
int pole[1000], maxpole[10];

void *generiraj(void *t) {
    int id = *((int *)t);

    for(int i = id * 100; i < id * 100 + 100; i++) {
        pole[i] = rand() % 5000;
    }

    pthread_exit(NULL);
}

void *prebaraj(void *t) {
    int id = *((int *)t);
    int max = pole[id * 100];

    for(int i = id * 100; i < id * 100 + 100; i++) {
        if(pole[i] > max) {
            max = pole[i];
        }
    }

    maxpole[id] = max;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t nitki[MAX_NITKI];
    int t[10];
    int rc1;

    for(int i = 0; i < MAX_NITKI; i++){
        t[i] = i;
        rc1 = pthread_create(&nitki[i], NULL, generiraj, (void *)&t[i]);
        if(rc1) {
            printf("Neuspesno kreiranje na nitka %d \n", i);
            exit(-1);
        }
    }

    for(int i = 0; i < MAX_NITKI; i++){
        rc1 = pthread_join(nitki[i], NULL);
        if (rc1) {
            printf("ERROR; return code from pthread_join() is %d\n", rc1);
            exit(-1);
        } 
    }

    for(int i = 0; i < MAX_NITKI; i++){
        t[i] = i;
        rc1 = pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i]);
        if(rc1) {
            printf("Neuspesno kreiranje na nitka %d \n", i);
            exit(-1);
        }
    }

    for(int i = 0; i < MAX_NITKI; i++){
        rc1 = pthread_join(nitki[i], NULL);
        if (rc1) {
            printf("ERROR; return code from pthread_join() is %d\n", rc1);
            exit(-1);
        } 
    }

    int max = maxpole[0];
    for(int i = 1; i < MAX_NITKI; i++) {
        if(maxpole[i] > max) {
            max = maxpole[i];
        }
    }

    printf("Najgolemiot generiran broj e %d \n", max);
    return 0;

}