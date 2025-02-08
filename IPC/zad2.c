// Да се напише програма во C програмскиот јазик каде ќе се овозможи внесување на низа од цели броеви
// како аргументи на програмата. Потоа, програмата треба да дозволи внесување на непознат број на цели
// броеви за пребарување во низата. За секој внесен број, програмата креира нова нитка, преку која го
// пребарува тој број во низата и печати на екран колку пати се појавува тој број и колку има броеви во
// низата кои што се делители на тој број. Печатењето на екран не го прават нитките што го прават
// пребарувањето туку тие резултатот го препраќаат до друг процес преку мемориско мапирање.
// Внесувањето на броевите за пребарување прекинува кога ќе се внесе од тастатура нешто различно од
// цел број. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int numbers[100];
int num_Numbers = 0;

typedef struct {
    int broj;
    int povtoruvanja;
    int deliteli;
} rezultat;

rezultat *rezultati;

void *prebaraj(void *arg) {
    int number = *((int *)arg);
    int count = 0;
    int deliteli = 0;

    for(int i = 0; i < num_Numbers; i++) {
        if(numbers[i] == number) {
            count++;
        }

        if(number % numbers[i] == 0) {
            deliteli++;
        }
    }

    rezultati[number].broj = number;
    rezultati[number].povtoruvanja = count;
    rezultati[number].deliteli = deliteli;

    return NULL;
}

int main(int argc, char *argv[]) {
    char input[20];

    int size = 100 * sizeof(rezultat);

    rezultati = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED);

    if (rezultati == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    while (1) {
        printf("Vnesi broj: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }

        if (strncmp(input, "end", 3) == 0) {
            break;
        }

        if (isdigit(input[0])) {
            numbers[num_Numbers] = atoi(input);
            num_Numbers++;
        }
    }

    pthread_t nitki[100];
    int thread_count = 0;

    while (1) {
        printf("Vnesi broj za prebaruvanje: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if(isdigit(input[0])) {
            int num = atoi(input);
            pthread_create(&nitki[thread_count], NULL, prebaraj, (void *)&num);
            thread_count++;
        } else {
            break;
        }
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(nitki[i], NULL);
    }

    for (int i = 0; i < thread_count; i++) {
        if (rezultati[i].povtoruvanja != 0) {
            printf("Number: %d, Occurrences: %d, Divisors: %d\n", rezultati[i].broj, rezultati[i].povtoruvanja, rezultati[i].deliteli );
        }
    }

    return 0;
}