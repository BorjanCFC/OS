// Да се напише програма во С која што ќе креира К нитки (К се внесува од тастатура), кои што ќе
// пребаруваат зборови во датотека. Главната нитка ги креира К-те нитки и притоа на секоја една од
// нитките и кажува кој збор да го пребарува. Тоа значи дека пред да се креира нитката, треба од тастатура
// да се внесе зборот и истиот да и се прати на нитката за пребарување. Секоја нитка го пребарува својот
// збор во датотеката и печати на екран колку пати го нашла зборот. Изминувањето на датотеката да се
// прави преку мемориско мапирање на истата, притоа, секоја една од нитките и кажува на главната нитка
// колку пати го има пронајдено зборот. Оваа информација, нитките ја споделуваат со главната нитка
// преку заедничка меморија. Главната нитка, откако ќе завршат сите останати нитки, на екран печати
// колку вкупно зборови биле најдени (од сите К зборови) во датотеката. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
    char word[20];
    int count;
    char *data;
    size_t sizeDat;
} info_t;

void *prebaraj(void *t) {
    info_t *info = (info_t *) t;
    char *ptr = info->data;
    int count = 0;
    int word_len = strlen(info->word);

    for(int i = 0; i <= info->sizeDat - word_len; i++) {
        if(strncmp(ptr + i, info->word, word_len) == 0) {
            count++;
        }
    }
    info->count = count;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    char fileName[100];
    int K;

    printf("Vnesete ime na datoteka: ");
    scanf("%s", fileName);

    printf("Vnesete broj na nitki (K): ");
    scanf("%d", &K);

    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        perror("Ne mozhe da se otvori datotekata");
        return -1;
    }
    
    int datSize = lseek(fd, 0, SEEK_END);
    if (datSize == -1) {
        perror("lseek failed");
        close(fd);
        return -1;
    }
    lseek(fd, 0, SEEK_SET);

    char *fileData = mmap(NULL, datSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fileData == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }
    close(fd);

    pthread_t nitki[K];
    info_t t[K];

    for(int i = 0; i < K; i++) {
        printf("Vnesete zbor: ");
        scanf("%s", t[i].word);

        t[i].data = fileData;
        t[i].sizeDat = datSize;
        t[i].count = 0;

        pthread_create(&nitki[i], NULL, prebaraj, (void *)&t[i]);

    }

    int vkupno = 0;
    for (int i = 0; i < K; i++) {
        pthread_join(nitki[i], NULL);
        printf("Zborot '%s' e najden %d pati.\n", t[i].word, t[i].count);
        vkupno += t[i].count;
    }

    printf("Vkupno zborovi najdeni: %d\n", vkupno);

    munmap(fileData, datSize);

    return 0;

}