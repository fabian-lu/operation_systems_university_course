#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(int argc, char *argv[]) {
  //argv[1] = pipeName, open pipe for reading only
  int fd = open(argv[1], O_RDONLY);
  char buf[MAX_BUF];
  int bytesread;
  while(1) {
    if((bytesread = read( fd, buf, MAX_BUF - 1)) > 0)
    {
        buf[bytesread] = '\0';
        printf("Received: %s\n", buf);
    }
  }

  return 0;
}
