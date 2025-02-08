// Да се напише програма во C програмскиот јазик каде како аргументи (од командна линија) добива
// најпрво бројка Х, а потоа листа од команди (без аргументи). Програмата треба да ги изврши во посебни
// процеси секоја една од командите. Доколку бројот на команди е помал од 5, тогаш се извршуваат сериски
// (прво првата, па откако ќе заврши втората се извршува третата и т.н.) притоа првата команда чека Х
// секунди пред да почне да се извршува, останатите не чекаат туку веднаш по претходната почнуваат со
// извршување. Доколку бројот на команди е 5 или повеќе, тогаш сите команди почнуваат паралелно да се
// извршуваат, така што секоја од командите чека Х секунди пред да се изврши. Без разлика како ќе отпочнат
// со извршување командите, излезот од секоја команда да се редиректира во датотека со име исто како и
// бројот на процесот што се извршува командата со наставка .txt (пример: доколку бројот на процесот е 1567,
// тогаш тој процес излезот го редиректира кон датотека со име “1567.txt”). На крај, главниот процес ја печати
// содржината од датотеките што се креирани од процесите на екран. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("Nedovolen broj na argumenti");
        exit(-1);
    }

    int X = atoi(argv[1]);

    if(X < 0) {
        printf("Nevaliden broj za X");
        exit(-1);
    }

    int br_komandi = argc - 2;

    if (br_komandi < 5) {
        for(int i = 0; i < br_komandi; i++) {
            pid_t dete = fork();
            if(pid < 0) {
                perror("FORK FAILED");
            } else if (dete == 0) {

                if(i == 0) {
                    sleep(X);
                }

                char fileName[20];
                sprintf(fileName, "%d.txt", getpid());

                int fd = open(fileName, O_CREAT | O_WRONLY, 0666 );
                if(fd < 0) {
                    perror("FILE CANNOT OPEN");
                }

                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);

                execlp(argv[i + 2], argv[i + 2], NULL);
            } else {
                wait(NULL);
            }
        }
    } else {
        for(int i = 0; i < br_komandi; i++) {
            pid_t dete = fork();
            if(pid < 0) {
                perror("FORK FAILED");
            } else if(dete == 0){
                sleep(X);

                char fileName[20];
                sprintf(fileName, "%d.txt", getpid());

                int fd = open(fileName, O_CREAT | O_WRONLY, 0666 );
                if(fd < 0) {
                    perror("FILE CANNOT OPEN");
                }

                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);
                close(fd);

                execlp(argv[i + 2], argv[i + 2], NULL);
            }
        }
        while (wait(NULL) > 0);
    }

    for(int i = 0; i < br_komandi; i++) {
        char fileName[20];
        sprintf(filename, "%d.txt", getpid()); 
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("FILE CANNOT OPEN");
            continue;
        }

        char ch;
        while ((ch = fgetc(file)) != EOF) {
            printf("%c", ch);
        }
        fclose(file);
    }
}