// Да се напишат две програми во C кои работат со процеси и нитки. Првата програма се повикува од
// конзола и притоа како аргумент (од командна линија) се испраќа број „N“ кој кажува кој број треба да
// се избрише. Потоа, првата програма ја стартува втората програма како аргумент од командна линија ѝ ја
// испраќа вредноста „N“ за бришење. Двата процеси креираат по 5 нитки кои ја пребаруваат вредноста N
// во мемориски мапирана датотека (со име „dat.txt“) во која има 100 цели броеви. Секоја од нитките на
// двата процеси генерира случаен индекс од низата од 100 цели броеви и проверува дали е на таа позиција
// во датотеката бројот „N“. Доколку е, тогаш таа нитка што ја нашла вредноста им сигнализира на сите
// останати нитки (во двата процеси) да прекинат со пребарување додека конкретната нитка не ја избрише
// вредноста од низата (со поместување на сите елементи во лево). Кога ќе заврши со бришење, им кажува
// на другите нитки дека може да продолжат со пребарување. Секоја нитка пробува да ја најде вредноста
// „N“ 10 пати. На крај главните процеси печатат на екран колку пати била пронајдена и избришана
// вредноста „N“ од низата (вкупно од секоја нитка кај двата процеси).
// Генерирањето на случајни броеви се прави со помош на функцијата rand( )*100. 

// PROGRAMA 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Nedovolen broj na argumenti\n");
        return 1;
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
#include <string.h>
#include <errno.h>

#define NUM_THREADS 5
#define ARRAY_SIZE 100

int *data;  
int N;     

typedef struct {
    int thread_id;
    int found_counter;
} info_t;

void *prebaraj(void *arg) {
    info_t *tdata = (info_t *)arg;  
    int found_count = 0;

    for (int i = 0; i < 10; i++) {
        int idx = rand() % (ARRAY_SIZE - 1) + 1; 

        while (data[0] == 1) {
            sleep(10); 
        }

        data[0] = 1;

        if (data[idx] == N) {
            found_count++;

            for (int j = idx; j < ARRAY_SIZE - 1; j++) {
                data[j] = data[j + 1];
            }
            data[ARRAY_SIZE - 1] = 0; 
        }

        data[0] = 0;

        sleep(1); 
    }

    tdata->found_counter = found_count;  
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Nedovolen broj na argumenti\n");
        return 1;
    }

    N = atoi(argv[1]);
    char *process_role = argv[2];

    int fd = open("dat.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        return 1;
    }

    int size = ARRAY_SIZE * sizeof(int);

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    data[0] = 0;

    pthread_t nitki[NUM_THREADS];
    info_t infos[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        infos[i].thread_id = i;
        infos[i].found_counter = 0;
        pthread_create(&nitki[i], NULL, prebaraj, (void*)&infos[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(nitki[i], NULL);
    }

    int totalDeleted = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        totalDeleted += infos[i].found_counter;
    }

    printf("Process %s deleted N %d times in total\n", process_role, totalDeleted);

    munmap(data, 1024);
    close(fd);
    return 0;
}
