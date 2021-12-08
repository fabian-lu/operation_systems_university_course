#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {

    int shmid;
    int* values;
    
    shmid = shmget(IPC_PRIVATE, sizeof(int)*2, IPC_CREAT | 0777);

    if (fork() == 0) {
        // Child

        values = (int*) shmat(shmid, 0, 0);

        // printf("[D] Child: %d, %d\n", values[0], values[1]);

        if (values[0] == 1) {
            printf("Child: %d\n", values[1]);

            values[0] == 0;
        }

        printf("Child exited\n");
        return 0;
    }


    values = (int*) shmat(shmid, 0, 0);

    // printf("[D] Parent: %d, %d\n", values[0], values[1]);

    if (values[0] == 0) {
        values[1] = 42;
        values[0] = 1;
        printf("Parent wrote values\n");
    }

    printf("Parent exited\n");
    return 0;
}
