//  Да се напише програма во C која работи со процеси и нитки. Главната програма (главната
// нитка) како аргумент добива име на влезна датотека. Главната нитка треба да креира онолку
// нитки колку што треба, така што, секоја нитка да добие по 10 линии од влезната датотека (нема
// повеќе од 1000 линии, а притоа последната нитка може да добие и помалку од 10 линии).
// Секоја една од нитките, ги изминува своите 10 линии од датотеката и брои колку има големи а
// колку мали букви. Откако ќе завршат нитките, главната нитка печати на екран колку секоја
// нитка нашла големи и мали букви и печати колку вкупно големи и мали букви биле
// пронајдени. Не е дозволено содржината на целата датотека да биде прочитана во низа т.е.
// секоја од нитките мора да работи со FILE * покажувач за изминување на датотеката т.е. на
// линиите од датотеката.

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char dat[30];
int mali[100], golemi[100];

void *prebaraj (void *t) {
    int id = *((int *)t);
    int from = id * 10;
    int to = id * 10 + 10;
    char c;

    mali[id] = 0;
    golemi[id] = 0;

    FILE *fp = fopen(dat, "r");
    if(!fp) {
        printf("Greska pri otvoranje na datoteka");
        return 0;
    }

    while((c = fgetc(fp)) != EOF) {
        if(c == '\n') {
            if(from > 0){
                from--;
                to--;
            } else {
                to--;
                if(to == 0) {
                    break;
                }
            }
        } else {
            if(from == 0 || to > 0) {
                if(isalpha(c)) {
                    if(isupper(c)) {
                        golemi[id]++;
                    } else {
                        mali[id]++;
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t nitki[100];
    int t[100];
    char c;
    int linii = 0;

    if(argc < 2) {
        printf("Nedovolen broj na argumenti");
        return 0;
    }

    strcpy(dat, argv[1]);

    FILE *fp = fopen(dat, "r");
    if(!fp) {
        printf("Greska pri otvoranje na datoteka");
        return 0;
    }

    while((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            linii++;
        }
    }

    fclose(fp);

    int brN;
    if (linii % 10 != 0) {
        brN = (linii / 10) + 1;
    } else {
        brN = linii / 10;
    }
    printf("Vo datotekata %s ima %d linii i kje kreiram %d nitki\n", dat, linii, brN);

    for(int i = 0; i < brN; i++) {
        t[i] = i;
        pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i]);
    }

    for(int i = 0; i < brN; i++) {
        pthread_join(nitki[i], NULL);
    }

    
}