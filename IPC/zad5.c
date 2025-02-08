// Да се напише програма во C која како аргумент добива име на непостоечка датотека.
// Програмата најпрво креира дете процес. Родител процесот треба да генерира 1000 случајни
// броеви со помош на функцијата rand() (од библиотеката stdlib.h) и истите ги запишува на екран
// (кои со помош на редирекција се запишуваат во датотеката) и родител процесот завршува. Дете
// процесот проверува дали може да ја отвори датотеката, и ако не може заспива 1 секунда. Потоа
// проверува пак, и заспива пак 1 секунда, се додека не успее да ја отвори датотеката. Чим успее
// да ја отвори, чита 1000 броеви од тастатура (редиректирани од датотеката) и ги сместува во
// низа. Потоа, дете процесот, со помош на 4 нитки (секоја нитка пребарува по 250 броеви), го
// наоѓа најголемиот број и го печати на екран. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>

#define NUMBERS 1000
#define THREADS 4

int numbers[NUMBERS];
int max_values[THREADS];

void* find_max(void* arg) {
    int id = *(int*)arg;
    max_values[id] = numbers[id];
    for (int i = id * 250; i < id * 250 + 250; i++) {
        if (numbers[i] > max_values[id]) {
            max_values[id] = numbers[i];
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Nedovolen broj na argumenti");
        return 1;
    }

    char* fileName = argv[1];
    pid_t dete = fork();

    if (dete < 0) {
        perror("FORK FAILED");
        return 1;
    }

    if (dete == 0) {  
        FILE* file = NULL;

        while ((file = fopen(fileName, "r")) == NULL) {
            sleep(1); 
        }

        for (int i = 0; i < NUMBERS; i++) {
            fscanf(file, "%d", &numbers[i]);
        }
        fclose(file);

        pthread_t nitki[THREADS];
        int t[THREADS];
        for (int i = 0; i < THREADS; i++) {
            t[i] = i;
            pthread_create(&nitki[i], NULL, find_max, &t[i]);
        }

        for (int i = 0; i < THREADS; i++) {
            pthread_join(nitki[i], NULL);
        }

        int max = max_values[0];
        for (int i = 1; i < THREADS; i++) {
            if (max_values[i] > max) {
                max = max_values[i];
            }
        }

        printf("The maximum number is: %d\n", max);

    } else { 
        FILE* file = fopen(fileName, "w");
        if (file == NULL) {
            perror("fopen");
            return 1;
        }

        for (int i = 0; i < NUMBERS; i++) {
            int random_number = rand();
            fprintf(file, "%d\n", random_number);
        }
        fclose(file);
    }

    return 0;
}
