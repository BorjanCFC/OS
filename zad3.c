#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Nedovolen broj na argumenti\n",);
        return 0;
    }

    int N;
    printf("Vnesi broj na izvrsuvanja: ");
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        pid_t dete = fork();

        if (dete < 0) {
            printf("fork failed");
            return 0;
        }
        else if (dete == 0) {  

            execvp(argv[1], argv + 1);
            
            printf("Dete failed to exec\n");
            return 0;
        }
    }

    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    printf("Zavrshuva process 0\n");
    return 0;
    
}
