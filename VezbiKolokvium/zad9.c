// Да се напише програма која ќе креира 2
// нитки и на двете нитки ќе им испрати реден
// број. Да се прикаже редоследот на
// извршување на нитките.

#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void *funkcija(void *t) {
    int i = *((int *)t); 
    printf("Jas sum nitka %lu, dobiv broj %d\n", pthread_self(), i);
    pthread_exit(NULL); // Завршување на нитката
}

int main(int argc, char *argv[]) {
    pthread_t threads[10]; 
    int rc1; 
    long t; 
    void *status; 
    int b[5]; 

    for (t = 0; t < 5; t++) {
        printf("Main: kreiranje nitka %ld\n", t);
        b[t] = t; 
        rc1 = pthread_create(&threads[t], NULL, funkcija, (void *)&b[t]); 
        if (rc1) { 
            printf("ERROR; return od pthread_create() is %d\n", rc1); 
            exit(-1); 
        }
    }

    for (t = 0; t < 5; t++) {
        rc1 = pthread_join(threads[t], &status); 
        if (rc1) { 
            printf("ERROR; return code from pthread_join() is %d\n", rc1);
            exit(-1);
        }
        printf("Main: završi priklucuvanje so nitka %ld so status %ld\n", t, (long)status);
    }

    pthread_exit(NULL); 
    
}
