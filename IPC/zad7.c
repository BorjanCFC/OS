// Да се напишат две програми во C кои работат со процеси и нитки. Првата програма се повикува од
// конзола и притоа како аргумент (од командна линија) се испраќа број „N“ кој кажува кој број треба да
// се пребара. Потоа, првата програма ја стартува втората програма како аргумент од командна линија ѝ ја
// испраќа вредноста „N“ за пребарување. Двата процеси креираат по 5 нитки кои ја пребаруваат
// вредноста N во мемориски мапирана датотека (со име „dat.txt“) во која има 100 цели броеви. Секоја од
// нитките на двата процеси генерира случаен индекс од низата од 100 цели броеви и проверува дали е на
// таа позиција во датотеката бројот „N“. Доколку е, тогаш ја менува неговата вредност со зголемување за
// 1 кај првиот процес и намалување за 1 кај вториот процес (каде што ќе го пронајде „N“ поставува
// вредност „N+1“ или „N-1“ соодветно) и продолжува понатаму. Пребарувањето трае 5 секунди (и за
// двата процеси), потоа на крај двата процеси печатат колку пати ја пронашле вредноста „N“ вкупно од
// сите 5 нитки. 

// PROGRAMA 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Nedovolen broj na argumenti");
    }

    int N = atoi(argv[1]);
    pid_t dete = fork();

    if (dete < 0) {
        perror("fork failed");
        return 1;
    } else if (dete == 0) { 
        execlp("./programa2", "programa2", argv[1], "1", NULL);
        perror("execlp failed");
        return 1;
    } else { 
        wait(NULL);
    }
}

// PROGRAMA 2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <errno.h>

#define NUM_THREADS 5
#define ARRAY_SIZE 100

int *data;
int N;
char proces[2];

typedef struct {
    int thread_id;
    int found_counter;
} info_t;

void *prebaraj(void *arg) {
    info_t *tdata = (info_t *)arg;  
    int found_count = 0;

    for (int i = 0; i < 5; i++) {
        int idx = rand() % ARRAY_SIZE;

        if (data[idx] == N) {
            found_count++;
            if (strcmp(proces, "1") == 0) {
                data[idx] = N + 1;
            } else {
                data[idx] = N - 1; 
            }
        }
        sleep(1);  
    }

    tdata->found_counter = found_count;  
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Nedovolen broj na argumenti");
    }

    N = atoi(argv[1]);

    proces = argv[2];

    int fd = open("dat.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    data = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (data[i] == 0) {
            data[i] = rand() % 100;  
        }
    }

    pthread_t nitki[NUM_THREADS];
    info_t infos[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) {
        infos[i].thread_id = i;
        infos[i].found_counter = 0;
        
        pthread_create(&nitki[i], NULL, prebaraj, (void*)&infos[i]);

    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    int maxCount = 0;
    for(int i = 0; i < NUM_THREADS; i++) {
        maxCount += infos[i].found_counter;
    }

    printf("Process %s found N %d times in total\n", proces, maxCount);

    munmap(data, ARRAY_SIZE * sizeof(int));
    close(fd);
}