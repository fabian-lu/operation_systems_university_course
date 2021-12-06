#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  int fd = open("text.txt", O_WRONLY | O_CREAT , S_IRWXU);
  close(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);

  printf("Hallo von dup.c 1!\n");
  return 0;
}
