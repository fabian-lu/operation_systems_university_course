#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

void *print_hello(void *threadid);


int main () {
  int i = 0;
  pthread_t thread_id[10];
  while (i < 10) {
    pthread_create(&thread_id[i], NULL, print_hello, NULL);
    i++;
  }

  i = 0;
  while (i < 10) {
    pthread_join(thread_id[i], NULL);
    i++;
  }
  return 0;
}


void *print_hello(void *threadid) {
  printf("Hallo von Thread %ld \n", pthread_self());
  pthread_exit(NULL);

  return 0;
}
