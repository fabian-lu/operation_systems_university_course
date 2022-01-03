#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

volatile long int in;

//Global Variables for Sync
//0 = false, 1 = true
int flag[2] = {0, 0};

int turn;



void *threadFunc(int *args)
{
  unsigned long int count_max = args[1];
  int threadId = args[0];
  const int constTurn = threadId == 1 ? 0 : 1;

  while (1) {
    flag[threadId] = 1;
    asm volatile("mfence" ::: "memory");
    turn = threadId == 1 ? 0 : 1;
    asm volatile("mfence" ::: "memory");
    while (flag[constTurn] == 1 && turn == constTurn) {
      //do nothing - waiting for turn
    }
    asm volatile("mfence" ::: "memory");
    // -------------- criticical section -------------------------------
    int i = 0;
    long int next_free_slot = in;
    while (i < count_max) {
      next_free_slot++;
      i++;
    }
    in = next_free_slot;
    flag[threadId] = 0;
    asm volatile("mfence" ::: "memory");
    pthread_exit((void*) 0);
    // -------------- end criticical -----------------------------------
  }
}


int main(int argc, char *argv[])
{
  //main thread
  in = 0;
  unsigned long int count_max = atoi(argv[1]);

  //assume thread 1 has id 0 und thread 2 has id 1
  pthread_t th1, th2;
  int argsP1[2] = {0, count_max};
  int argsP2[2] = {1, count_max};
  pthread_create(&th1, NULL, (void *) threadFunc, argsP1);
  pthread_create(&th2, NULL, (void *) threadFunc, argsP2);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  printf("%ld\n", in);
  return 0;
}
