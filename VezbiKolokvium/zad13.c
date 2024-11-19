// Да се напише програма во C која работи со процеси и нитки. Главната програма (родител
// процесот) добива низа од наредби кои треба да ги изврши како аргументи од командна линија.
// Родител процесот треба, најпрво, да креира онолку деца процеси колку што има наредби
// наведено од командна линија (наредбите се без аргументи). Потoа, треба да креира онолку нитки,
// колку што има наредби, така што, секоја нитка ќе чека и ќе брои колку секунди му било потребно
// на соодветниот процес да заврши. Тоа значи дека, првата нитка ќе биде задолжена за првата
// наредба т.е. за првиот процес, втората за вториот и т.н. Секоја нитка брои колку време се
// извршувал нејзиниот процес (наредба) и кога ќе заврши кажува колку вкупно секунди му требало
// да заврши, а потоа и самата нитка завршува. Откако ќе завршат сите процеси/нитки, тогаш
// главниот процес/нитка печати на екран колку време и требало на секоја наредба да се изврши.

// БОНУС: главниот процес/нитка печати на екран колку време и требало на секоја наредба да се
// изврши во растечки редослед според времето на завршување, најпрво да се отпечати која
// наредба и требало најмалку време да заврши, па потоа следната наредба и т.н.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t deca[100];
int vreme[100];

void *f(void *t) {
    int id = *((int *)t);
    int s = 0;

    while(waitpid(deca[id], NULL, WNOHANG) != deca[id]){
        sleep(1);
        s++;
    }

    vreme[id] = s;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t nitki[100];
    int N = argc - 1;
    int i;
    int rc1;

    for(i = 0; i < N; i++) {
        deca[i] = fork();
        if(deca[i] == 0) {
            break;
        }
    }

    if(i < N) {
        execlp(argv[i + 1], argv[i + 1], NULL);
    } else {
        int t[100];
        for(i = 0; i < N; i++) {
            t[i] = i;
            rc1 = pthread_create(&nitki[i], NULL, f, (void *)&t[i]);

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
    }

    for(i = 0; i < N; i++) {
        printf("%s trebase %d vreme\n", argv[i + 1], vreme[i]);
    }

    return 0;
}