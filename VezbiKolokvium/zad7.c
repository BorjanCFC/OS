// Да се напише програма која како аргумент добива команда која
// треба да ја изврши.
// • Програмата треба да изврши замена на стандардниот знак „&“
// со „%“ за започнување на програма како позадински или преден
// процес.

#include <stdio.h>
#include <unistd.h> 
#include <string.h>
// #include <sys/wait.h>
#define MSGSIZE 16 

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("Nedovolen broj na argumenti\n");
        return 0;
    }

    int len = strlen(argv[argc - 1]);

    int towait = 1;
    if(len == 1 && argv[argc - 1][0] == '%') {
        argv[argc - 1][0] = 0;
        towait = 0;
    } else if (argv[argc - 1][len - 1] == '%'){
        argv[argc - 1][len - 1] = 0;
        towait = 0;
    }

    pid_t dete = fork();
    if(dete == 0) {
        execvp(argv[1], &argv[1]);
        printf("Dete failed to execute\n");
    }

    if(towait) {
        wait(NULL);
    }

    return 0;
}