#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

void print(char check, int *ptrInt);

int main () {
  //memory sharing
  int shmid, *ptrInt;
  shmid = shmget(IPC_PRIVATE, sizeof(int) * 2, IPC_CREAT | 0777);
  ptrInt = (int *) shmat(shmid, 0, 0);
  print('0',ptrInt);
  ////////////////////////////////////////
  if (fork () == 0) {
    //child process
    print('2',ptrInt);
    if(*ptrInt == 1) {
      *ptrInt = 0;
    }
    print('3',ptrInt);
  } else {
    //parent
    if(*ptrInt == 0) {
      *(ptrInt + 1) = 10;
      *ptrInt = 1;
      print('1',ptrInt);
    }
  }

  return 0;
}

void print(char check, int *ptrInt) {
  printf("---------- %c ----------\n", check);
  printf("Erste Stelle: %d\n",*ptrInt);
  printf("Zweite Stelle: %d\n",*(ptrInt + 1));
}
