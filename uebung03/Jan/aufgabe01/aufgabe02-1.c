#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

char* shmbuffer;

void intHandler(int dummy) {
    printf("Exit\n");
    exit(EXIT_SUCCESS);
}


void readHandler(int dummy) {
    //printf("Reading\n");
    //printf("%p\n", shmbuffer);
    //printf("%s", shmbuffer);

    do {
        printf("%c", *shmbuffer);

        shmbuffer += sizeof(char);
    } while (*shmbuffer != '\n');
}

int main() {

    signal(SIGINT, intHandler);
    signal(SIGUSR1, readHandler);

    int key = 100;

    int shmid;
    
    while((shmid = shmget(key, 1024*sizeof(char), IPC_CREAT | IPC_EXCL | 0777)) == -1) {
        key++;
    }

    printf("PID: %d\n", getpid());
    printf("SHMID: %d\n", shmid);
    printf("Key: %d\n", key);
    
    shmbuffer = (char*) shmat(shmid, 0, 0);

    while(1) {
        pause();
    }

    return 0;
}
