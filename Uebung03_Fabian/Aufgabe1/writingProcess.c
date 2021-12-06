#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

int main (int argc, char* argv[]) {

  if(argc != 4) {
    printf("Error! Submit a <PID> <KEY> <FILENAME>.\n");
    return -1;
  }

  int pid = atoi(argv[1]);
  int key = atoi(argv[2]);
  //size = 0 because it is an already existing shared memory
  int shmid = shmget(key, 0, 0);
  char* fileName = argv[3];

  char* fileTextPtr = (char*) shmat(shmid, 0, 0);

  //write into shared memory
  FILE* filePointer = fopen(fileName, "r");


  while(fgets(fileTextPtr, sizeof(fileTextPtr), filePointer)) {
    //after a line is read and stored in shared memory, send signals
    kill(pid, SIGUSR1);
    sleep(1);
  }
  fclose(filePointer);


  return 0;
}
