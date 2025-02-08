// Да се напише програма во C програмскиот јазик за комуникација помеѓу два процеси. Програмата како
// аргумент добива име на датотека преку која ќе комуницираат двата процеси преку метод на мемориско
// мапирање. Родител процесот креира дете процес на кого му го кажува името на датотеката преку која ќе
// комуницираат. Датотеката е организирана така што, најпрво има 1 бајт за синхронизација, 128 бајти за
// клуч и 1KB меморија. Иницијално меморијата од 1KB е кодирана. Родител процесот, од датотека
// „kluc.out“ го чита клучот со кој треба да ја декодира меморијата. Дете процесот има 3 обиди да ја kluc.out“ 
// го чита клучот со кој треба да ја декодира меморијата. Дете процесот има 3 обиди да ја
// отклучи меморијата, притоа го чита клучот за отклучување од тастатура и го запишува во меморијата, и
// му сигнализира на родител процесот да ја отклучи меморијата. Родител процесот на секоја една секунда
// проверува дали треба да ја отклучи меморијата. Доколку треба да ја дешифрира меморијата, го чита
// клучот и го праќа на функцијата otkluci(char *kluc, char *tekst, int p); каде што даден текст го шифрира
// или дешифрира врз основа на вредноста на „kluc.out“ го чита клучот со кој треба да ја декодира меморијата. 
// Дете процесот има 3 обиди да јаp“ (0 шифрира, 1 дешифрира). Доколку го дешифрира, му
// кажува на дете процесот дека е дешифриран текстот и го држи дешифриран 10 секунди. По 10 секунди,
// повторно го шифрира текстот.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>

#define SYNC_BYTE 1
#define KEY_SIZE 128
#define DATA_SIZE 1024
#define FILE_SIZE (SYNC_BYTE + KEY_SIZE + DATA_SIZE)

void otkluci(char *kluc, char *tekst, int p) {
    int k = strlen(kluc);
    int t = strlen(tekst);
    int i, sum, z = 0;

    for (i = 0; i < t; i++) {
        if (p == 0) {
            sum = kluc[z] + tekst[i];
            if (sum > 255) sum -= 255;
        } else {
            sum = kluc[z] - tekst[i];
            if (sum < 0) sum += 255;
        }
        tekst[i] = sum;
        z = (z + 1) % k;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Nedovolen br na argumenti");
        return 1;
    }

    int fd, kluc_fd, obidi = 0;
    char key[KEY_SIZE], unlock[KEY_SIZE], *mem;
    pid_t dete;

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    mem = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        perror("Error mapping memory");
        return 1;
    }

    dete = fork();

    if (dete > 0) { 
        kluc_fd = open("kluc.out", O_RDONLY);
        if (kluc_fd == -1) {
            perror("Error opening kluc.out");
            return 1;
        }

        if (read(kluc_fd, key, KEY_SIZE) != KEY_SIZE) {
            perror("Error reading key");
            return 1;
        }
        close(kluc_fd);

        mem[0] = '0';

        while (1) {
            while (mem[0] == '0') {
                sleep(1);
            }

            if (strcmp(key, mem + SYNC_BYTE) == 0) {
                otkluci(key, mem + SYNC_BYTE + KEY_SIZE, 1);
                mem[0] = '2';

                sleep(10);

                otkluci(key, mem + SYNC_BYTE + KEY_SIZE, 0);
                mem[0] = '0';
            } else {
                mem[0] = '0';
            }
        }

    } else if (dete == 0) { 
        while (obidi < 3) {
            printf("Vnesi kluc za otklucuvanje: ");
            scanf("%s", unlock);

            strcpy(mem + SYNC_BYTE, unlock);
            mem[0] = '1'; 

            while (mem[0] == '1') {
                sleep(1);
            }

            if (mem[0] == '2') {
                printf("Uspeshno otkluceno\n%s\n", mem + SYNC_BYTE + KEY_SIZE);
                break;
            } else {
                obidi++;
            }
        }

        if (obidi == 3) {
            printf("Neuspeshno otklucuvanje po 3 obida.\n");
        }
    } else {
        perror("Error creating process");
        return 1;
    }

    return 0;
}
