#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Upotreba: %s <broj_izvrsuvanja> <komanda> [argumenti]\n", argv[0]);
        return 1;
    }

    int num_executions = atoi(argv[1]);
    if (num_executions < 1) {
        printf("Broj izvrsuvanja mora da bide pozitiven broj!\n");
        return 1;
    }

    for (int i = 0; i < num_executions; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Dete proces
            execvp(argv[2], &argv[2]);
            printf("Greska pri izvrsuvanje na komanda\n");
            return 1;
        } else if (pid > 0) { // Roditel proces
            int status;
            waitpid(pid, &status, 0);
        } else { // Greshka pri kreiranje na proces
            printf("Greska pri kreiranje na proces\n");
            return 1;
        }
    }

    printf("Zavrshuva proces 0\n");
    return 0;
}
