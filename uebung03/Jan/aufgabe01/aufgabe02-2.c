#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define N 1024

int main(int argc, char *argv[]) {
    if (argc != 4) exit(EXIT_FAILURE);

    int pid = atoi(argv[1]);
    int shmkey = atoi(argv[2]);
    char* filename = argv[3];

    int shmid = shmget(shmkey, N*sizeof(char), 0);

    printf("%d\n", shmid);

    char* shmbuffer = (char*) shmat(shmid, 0, 0);

    printf("%p\n", shmbuffer);

    if (shmbuffer == 0) exit(EXIT_FAILURE); 
    
    shmbuffer[0] = 'A';
    shmbuffer[1] = '\n';
    kill(pid, SIGUSR1);

    FILE* file = fopen(filename, "r");
    if (file == NULL) exit(EXIT_FAILURE);


    char* linebuffer;
    ssize_t read;
    size_t len = 0;
    while ((read = getline(&linebuffer, &len, file)) != -1) {
        strncpy(shmbuffer, linebuffer, read);
        printf("%s", shmbuffer);
        
        kill(pid, SIGUSR1);

        shmbuffer += read*sizeof(char);

        sleep(1);
    }

    fclose(file);

return 0;
}
