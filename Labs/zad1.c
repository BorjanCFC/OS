#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define MSG_SIZE 256

void writer_process(int shmid);
void cleaner_process(int shmid);
void reader_process(int shmid);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <process_type> <key>\n", argv[0]);
        return 1;
    }

    int process_type = atoi(argv[1]);
    key_t key = atoi(argv[2]);

    int shmid = shmget(key, MSG_SIZE + 1, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    switch (process_type) {
        case 1:
            writer_process(shmid);
            break;
        case 2:
            cleaner_process(shmid);
            break;
        case 3:
            reader_process(shmid);
            break;
        default:
            fprintf(stderr, "Invalid process type. Use 1, 2, or 3.\n");
            return 1;
    }

    return 0;
}

void writer_process(int shmid) {
    srand(time(NULL));
    char *shared_memory = shmat(shmid, NULL, 0);

    if (shared_memory == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        if (shared_memory[0] == 0) {
            printf("Enter a message: ");
            fgets(shared_memory + 1, MSG_SIZE, stdin);
            shared_memory[0] = 1;
            int sleep_time = rand() % 10 + 1;
            printf("Writer sleeping for %d seconds...\n", sleep_time);
            sleep(sleep_time);
        } else {
            sleep(1);
        }
    }
}

void cleaner_process(int shmid) {
    char *shared_memory = shmat(shmid, NULL, 0);

    if (shared_memory == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        sleep(5);
        memset(shared_memory, 0, MSG_SIZE + 1);
        printf("Cleaner cleared the message.\n");
    }
}

void reader_process(int shmid) {
    srand(time(NULL));
    char *shared_memory = shmat(shmid, NULL, 0);

    if (shared_memory == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        if (shared_memory[0] == 1) {
            printf("Reader read: %s", shared_memory + 1);
            shared_memory[0] = 0; 
            int sleep_time = rand() % 10 + 1;
            printf("Reader sleeping for %d seconds...\n", sleep_time);
            sleep(sleep_time);
        } else {
            sleep(1);
        }
    }
}
