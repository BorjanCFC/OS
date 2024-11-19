// Татко процесот ќе ги креира деца процесите. Секое дете процес
// ќе испише на екран кој му е татко процес, и кое му е
// сопственото ID. Татко процесот чека секое од децата процеси да
// завршат и на крај печати дека и тој се уништува. 

#include <stdio.h>
#include <unistd.h> 
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int i;
    pid_t dete;

    for (i = 0; i < 3; i++) {
        dete = fork();

        if(dete == 0) break;
    }

    if(dete == 0) {
        printf("Jas sum dete so pid %d, tatko mi e so pid %d\n", getpid(), getppid());
    } else {
        for(i = 0; i < 3; i++){
            wait(NULL);
            printf("Jas sum roditel so pid %d\n", getppid());
        }
    }

    return 0;

}
