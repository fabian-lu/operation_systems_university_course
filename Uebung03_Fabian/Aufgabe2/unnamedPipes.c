#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

  //create unnamed pipe
  int fd[2];
  pipe(fd);

  if(fork() == 0) {
    //child
    //get the string sent from parent via pipe
    char receivedString[80];

    //close write descriptor
    close(fd[1]);
    //read string from pipe
    read(fd[0], receivedString, 80);

    printf("%s\n", receivedString);
  } else {
    //parent
    //get proper string from command line
    char stringToSend[80];
    if(strlen(argv[1]) > 79) {
      strncpy(stringToSend, argv[1], 79);
      stringToSend[79] = '\0';
    } else if(strlen(argv[1]) < 79) {
      strcpy(stringToSend, argv[1]);
      size_t prevlen = strlen(stringToSend);
      memset(stringToSend + prevlen, '\0', 79 - prevlen);
    } else {
      strcpy(stringToSend, argv[1]);
    }

    //send string to child via pipe
    //close reading descriptor
    close(fd[0]);
    //write string into pipe
    write(fd[1], stringToSend, 80);
    //stop parent process
    exit(0);
  }
  return 0;
}
