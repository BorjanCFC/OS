// Да се напишат две програми во С кои ќе работат со IPC. Од првата програма истовремено може да се
// креира само еден процес, додека пак од втората програма истовремено (паралелно) може да се
// креираат најмногу 200 процеси. Првата програма со процесите од втората програма комуницираат преку
// заедничка меморија (shared memory) преку клучот 1234. Првата програма ја креира меморијата (од 1КВ)
// и ја прикачува во сопствениот адресен простор. Доколку првиот процес не успее да ја креира и прикачи
// меморијата во сопствениот адресен простор, тогаш пробува тоа да го направи со мапирана меморија (со
// датотека “dat.txt”). Доколку и мапираната меморија не успее да ја прикачи во сопсвениот адресен
// простор, печати порака за грешка и завршува. Доколку успее или со заедничка меморија или со
// мапирана, во меморијата запишува наредба (без аргументи, која ја добива како аргумент од командна
// линија), и завршува.
// Втората програма, без разлика колку паралелни процеси се креирани од истата, секој процес
// проверува дали може да ја прикачи заедничката меморија од првата програма (со истиот клуч) во
// сопствениот адресен простор, и притоа доколку не успее пробува со мапирана меморија (со истата
// датотека). Доколку не успее на двата начини да прикачи меморија во адресниот простор, прекинува со
// извршување. Доколку успее да прикачи меморија на еден од двата начини, чека наредбата да биде
// запишана во меморијата. Доколку не е запишана наредба, на секои 10 секунди проверува пак дали има
// запишано наредба, притоа вкупно чека 1 минута (до 6 пати). Во моментот кога ќе биде запишана
// наредбата во меморијата, се извршува и завршува. Доколку после 1 минута (6 пати по 10 секунди) се
// уште не е запишана наредбата, а притоа има прикачено заедничка меморија, следно истото го пробува и
// со мапирана меморија. Ако и во мапираната меморија нема запишано наредба, печати порака за грешка.

// PROGRAMA 1

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

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Nevaliden broj na argumenti")
        return 1;
    }

    int id, fd;
    char *mem;
    int k = 0;

    id = shmget(SHM_KEY, SHM_SIZE, 0644 | IPC_CREAT);
    if (id != -1) {
        mem = shmat(id, NULL, 0);
        if (mem == (void *)-1) {
            id = -1;
        }
    }

    if (id == -1) {
        printf("Neuspesno kreiranje na shared memory probuvam so mmap...\n");

        fd = open("dat.txt", O_RDWR | O_CREAT, 0666);
        if (fd == -1) {
            perror("File couldnt open");
            return 1;
        }

        if (ftruncate(fd, SHM_SIZE) == -1) {
            perror("Error");
            close(fd);
            return 1;
        }

        mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
            perror("Map failed");
            close(fd);
            return 1;
        }

        close(fd);
        k = 2;
    } else {
        k = 1;
    }

    memset(mem, 0, SHM_SIZE);
    mem[0] = '1'; 
    strncpy(mem + 1, argv[1], SHM_SIZE - 1);

    printf("Zapisav naredba: %s\n", argv[1]);

    if (k == 1) {
        shmdt(mem);
    } else {
        munmap(mem, SHM_SIZE);
    }

    return 0;
}

// PROGRAMA 2

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

#define SHM_KEY 1234
#define SHM_SIZE 1024

int main() {
    int id, fd, k = 0;
    char *mem;

    id = shmget(SHM_KEY, SHM_SIZE, 0644);
    if (id != -1) {
        mem = shmat(id, NULL, 0);
        if (mem == (void *)-1) {
            id = -1;
        }
    }

    if (id == -1) {
        printf("Neuspesno kreiranje na shared memory probuvam so mmap...\n");

        fd = open("dat.txt", O_RDWR | O_CREAT, 0666);
        if (fd == -1) {
            perror("File couldnt open");
            return 1;
        }

        mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
            perror("Map failed");
            close(fd);
            return 1;
        }

        close(fd);
        k = 2;
    } else {
        k = 1;
    }

    int i;

    for (i = 0; i < 6; i++) {
        if (mem[0] == '1') break;
        sleep(10);
    }

    if (i == 6 && k == 1) {
        shmdt(mem);

        fd = open("dat.txt", O_RDWR);
        if (fd == -1) {
            perror("File couldnt open");
            return 1;
        }

        mem = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (mem == MAP_FAILED) {
            perror("File couldnt open");
            close(fd);
            return 1;
        }

        close(fd);
        k = 2;
    }

    if ((i < 6 || k == 2) && strlen(mem + 1) > 0) {
        execlp(mem + 1, mem + 1, NULL);
        perror("Neuspesno izvrsena naredba");
        return 1;
    } else {
        printf("Ne pronajdov validna naredba!\n");
    }

    if (k == 1) {
        shmdt(mem);
    } else {
        munmap(mem, SHM_SIZE);
    }

    return 0;
}

