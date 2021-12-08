#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void handler();

int shmid;
char* fileTextPtr;

int main () {
  signal(SIGUSR1, handler);

  int faulty = 1;
  int key = 100;
  while(faulty) {
    shmid = shmget(key, sizeof(char) * 1024, IPC_CREAT | 0777);
    key++;
    faulty = shmid == -1 ? 1 : 0;
  }


  fileTextPtr = (char*) shmat(shmid, 0, 0);
  printf("%p\n", fileTextPtr);
  printf("Working key: %d\n", key - 1);
  printf("Process ID: %d\n", getpid());

  while(1) {
    pause();
  }
  return 0;
}

void handler()
{
  printf("%s", fileTextPtr);
}
