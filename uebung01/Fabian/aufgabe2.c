/*
Please make sure to add -D_BSD_SOURCE while compiling, since I am using strdup()
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define LINE_LENGTH 80
#define PARAMETER_LENGTH 10

void readLineInput(char *params[]);
void executeCommand(char *params[]);
void printChildrenProcesses();
int isLastElementControlSign(char *params[]);
int startCommandAsynchronous(char *params[]);
void startCommandSynchronous(char *params[]);

//max 10 children pids
int children[10];
int childrenCount = 0;


int main(int argc, char *argv[]) {

  if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    // associates standard input with submitted file
    freopen(argv[2], "r", stdin);
  }

  while(true) {
    //save divided line input in variable params
    char* params[PARAMETER_LENGTH];
    readLineInput(params);

    //run the Command with the entered params
    executeCommand(params);
  }

  return 0;
}

void readLineInput(char *params[]) {
  char line[LINE_LENGTH];
  char delim[] = " ";

  int i = 0;
  while(i < LINE_LENGTH && (line[i] = getchar()) != '\n')
    i++;

  if (line[i] != '\n')
    while(getchar() != '\n');
  line[i] = '\0';

  char *param = strtok(line, delim);
  for(i = 0; i < PARAMETER_LENGTH && param; i++) {
    params[i] = strdup(param);
    param = strtok(NULL, delim);
  }

  params[i] = NULL;
}

void executeCommand(char *params[]) {
  if (params[0] == NULL) {
    printf("Es wurde keine Anweisung übergeben.\n");
  } else if(strcmp(params[0],"childs") == 0) {
    printChildrenProcesses();
  } else if(strcmp(params[0],"quit") == 0) {
    printf("Bis zum nächsten mal!\n");
    exit(EXIT_SUCCESS);
  } else if(isLastElementControlSign(params)) {
    pid_t child_pid = startCommandAsynchronous(params);
    children[childrenCount] = child_pid;
    childrenCount++;
  } else {
    startCommandSynchronous(params);
  }
}

void printChildrenProcesses() {
  for (int i = 0; i < childrenCount; i++) {
    //kill 0 sends signal to process and checks activeness
    kill(children[i], 0);
    bool isActive = errno != ESRCH;
    printf("Child[%d] -> pid=%d : %s\n", i, children[i], (isActive ? "Running" : "Done"));
  }

}

int isLastElementControlSign(char *params[]) {
  //find index of last elemtent
  int i = 0;
  while(params[i] != NULL) {
    i++;
  }
  int indexLastElement = i - 1;
  int isControlSign = strcmp(params[indexLastElement], "&") == 0;

  if(isControlSign)
    params[indexLastElement] = NULL;

  return isControlSign;
}

int startCommandAsynchronous(char *params[]) {
  pid_t child_pid;
  child_pid = fork();

  if(child_pid == 0) {
    //child process
    int error = execv(params[0], params);
    printf("ERROR\n");
    exit(error);
  }

  return child_pid;
}

void startCommandSynchronous(char *params[]) {
  pid_t child_pid;
  child_pid = fork();

  if(child_pid == 0) {
    //child process
    int error = execv(params[0], params);
    printf("ERROR\n");
    exit(error);
  } else {
    //parent process
    wait(NULL);
  }
}
