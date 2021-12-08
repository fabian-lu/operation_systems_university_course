#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
  //Takes the values to calculate from the first string submitted on stdIn, e.g. "1+2"

  char *toCalculate = malloc(strlen(argv[1]) + 2);
  strcpy(toCalculate, argv[1]);
  strcat(toCalculate, "\n");

  //create two pipes, Donwstream -> parent to child, Upstream vice versa
  int pipeDownstream[2];
  int pipeUpstream[2];
  pipe(pipeDownstream);
  pipe(pipeUpstream);

  //processes
  if(fork() == 0) {
    //child
    //does only need to write in upstream pipe, and read from downstream pipe
    //0 = readDescriptor, 1 = writeDescriptor
    close(pipeUpstream[0]);
    close(pipeDownstream[1]);

    //close filedescriptor stdin and stdout
    close(STDOUT_FILENO);
    close(STDIN_FILENO);

    //use pipes for input/output instead
    dup2(pipeUpstream[1], STDOUT_FILENO);
    dup2(pipeDownstream[0], STDIN_FILENO);

    //replace child process with bc
    int err = execl("/usr/bin/bc" , "bc" , NULL);
    // only reached if execl fails
    exit(err);

  } else {
    //parent
    //does only need to write in downstream pipe, and read from upstream pipe
    close(pipeUpstream[1]);
    close(pipeDownstream[0]);

    //write into downstream pipe
    //10 is random, adjust if you need to calculate a longer term, write writes up to 10 bytes
    //like this into the pipe
    write(pipeDownstream[1], toCalculate, strlen(toCalculate));


    //read string from upstream pipe and print it
    char result[256];
    read(pipeUpstream[0], result, 256);

    printf("%s", result);

    return 0;
  }
}
