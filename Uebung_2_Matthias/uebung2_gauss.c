#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Anzahl der Threads 10
#define NUMTHRDS 10
// Feldgroesse pro Thread
#define CHUNKSIZE 10000

/*
Um das Programm sauber zu Programmieren muss eine in den Threads
geteilte Variable, die geschrieben wird, mit einem Mutex versehen
werden (in diesem Fall: 'sum'). 
Das array der Zahlen 'arr' sollte dies eigentlich nicht benoetigen,
da nur gelesen wird -> Zugriffsreihenfolge egal.
*/

pthread_mutex_t mutex; // <== NEU

int *arr;
long sum=0;

/*************************************************************/
void *gausswasbetteratthis(void *threadid)
{
    long start = (long)threadid*CHUNKSIZE;
    long end = start+CHUNKSIZE-1;

    long local_sum; // <== NEU
    for (long i=start; i<=end; i++){ 
    local_sum += arr[i];
    }

    // <== NEU, SYNC. DAS SCHREIBEN DER GLOBALEN VAR.
    errno = pthread_mutex_lock(&mutex);
    if (errno != 0) {
        perror("Mutex lock failed");
    }
    sum += local_sum;
    errno = pthread_mutex_unlock(&mutex);
    if (errno != 0) {
        perror("Mutex unlock failed");
    }
    // <== NEU ENDE

    pthread_exit((void*) 0);
}

/*************************************************************/
int main()
{
    // <== NEU
    errno = pthread_mutex_init(&mutex, NULL);
    if (errno != 0) {
        perror("Mutex init failed");
    }
    // <== NEU ENDE

    long i;
    void* status;
    pthread_t threads[NUMTHRDS];
    arr = (int*) malloc (CHUNKSIZE*NUMTHRDS*sizeof(int));

    for (i=0; i<CHUNKSIZE*NUMTHRDS; i++)
    arr[i] = i+1;

    //Create 10 Threads for summing up numbers.
    for(i=0; i<NUMTHRDS; i++) 
    pthread_create(&threads[i], NULL, gausswasbetteratthis, (void *)i); 


    for(i=0; i<NUMTHRDS; i++)
    pthread_join(threads[i], &status);

    printf ("Summe der ersten %d Zahlen ist %li\n", NUMTHRDS*CHUNKSIZE, sum);
    free (arr);

    // <== NEU
    errno = pthread_mutex_destroy(&mutex);
    if (errno != 0) {
        perror("Mutex destroy failed");
    }
    // <== NEU ENDE

    pthread_exit(NULL);
}
/*************************************************************/