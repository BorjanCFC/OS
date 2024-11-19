// Да се напише програма која како аргумент добива команда која треба
// да ја изврши.
// • Татко процесот треба на екран да ги испишува секундите поминати од
// започнување на програмата до крајот.

#include <stdio.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <string.h>
#define MSGSIZE 16 

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Nedovolen broj na argumenti\n");
        return 0;
    }

    pid_t dete = fork();
    if (dete == 0) {
        execvp(argv[1], &argv[1]);
        printf("Dete failed to exec\n");
    }

    int s = 0;
    while (waitpid(dete, NULL, WNOHANG) != dete) {
        s++;
        sleep(1);
    }

    printf("Pominaa %d sekundi\n", s);
    return 0;
}
