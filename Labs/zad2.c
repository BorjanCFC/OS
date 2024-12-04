#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define MSG_SIZE 256

void writer_process(const char *filename);
void cleaner_process(const char *filename);
void reader_process(const char *filename);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Nedovolen broj na argumenti\n");
    }

    int process_type = atoi(argv[1]);
    const char *filename = argv[2];

    switch (process_type) {
        case 1:
            writer_process(filename);
            break;
        case 2:
            cleaner_process(filename);
            break;
        case 3:
            reader_process(filename);
            break;
        default:
            printf("Nevalidno vnesen tip na proces.\n");
    }

    return 0;
}

void writer_process(const char *filename) {
    srand(time(NULL));

    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        printf("Greshka\n");
        exit(1);
    }

    ftruncate(fd, MSG_SIZE + 1);

    char *mapped_memory = mmap(NULL, MSG_SIZE + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_memory == MAP_FAILED) {
        printf("Greshka\n");
        close(fd);
        exit(1);
    }
    close(fd);

    while (1) {
        if (mapped_memory[0] == 0) {
            printf("Vnesi poraka: ");
            fgets(mapped_memory + 1, MSG_SIZE, stdin);
            mapped_memory[0] = 1;
            int sleep_time = rand() % 10 + 1;
            printf("Proces 1 spie za %d sekundi\n", sleep_time);
            sleep(sleep_time);
        } else {
            sleep(1);
        }
    }
}

void cleaner_process(const char *filename) {
    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("Greshka\n");
        exit(1);
    }

    char *mapped_memory = mmap(NULL, MSG_SIZE + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_memory == MAP_FAILED) {
        printf("Greshka\n");
        close(fd);
        exit(1);
    }
    close(fd);

    while (1) {
        sleep(5);
        memset(mapped_memory, 0, MSG_SIZE + 1);
        printf("Proces 2 ja izbrisa porakata.\n");
    }
}

void reader_process(const char *filename) {
    srand(time(NULL));

    int fd = open(filename, O_RDWR);
    if (fd == -1) {
        printf("Greshka!\n");
        exit(1);
    }

    char *mapped_memory = mmap(NULL, MSG_SIZE + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_memory == MAP_FAILED) {
        printf("Greshka!\n");
        close(fd);
        exit(1);
    }
    close(fd);

    while (1) {
        if (mapped_memory[0] == 1) {
            printf("Proces 3 ja cita porakata: %s", mapped_memory + 1);
            mapped_memory[0] = 0;
            int sleep_time = rand() % 10 + 1;
            printf("Proces 3 spie za %d sekundi\n", sleep_time);
            sleep(sleep_time);
        } else {
            sleep(1);
        }
    }
}
