#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define L 80
#define P 10
#define DELIM ' '
#define N 200

int readLine(char* params[P]) {
    printf("$ ");

    char line[L];

    int i = 0;
    while(i < L && (line[i] = getchar()) != '\n')
        i++;
    
    if (line[i] != '\n')
        while(getchar() != '\n');
    line[i] = '\0';

    if (i == 0) {
        return 0;
    }
    
    int j = 0;
    int last_j = -1;
    int k = 0;
    while(j <= i) {
        if (j == i || line[j] == DELIM) {
            params[k] = strndup(&line[last_j+1], (j-last_j-1));
            last_j = j;
            k++;
        }
        j++;
    }
    params[k] = NULL;

    if (params[0] == NULL) {
        return 0;
    }

    return k;
}

int runCommand(char* cmd, char *arguments[], bool shouldWait) {
    pid_t child_pid;
    int child_status;

    child_pid = fork();
    if(child_pid == 0) {
        // Child
        int result = execv(cmd, &arguments[0]);
        perror("execve");
        printf("ERROR: Invalid command: %d | %d\n", result, errno);
        exit(0);
    } else {
        // Parent
        if (shouldWait) {
            pid_t tpid = waitpid(child_pid, &child_status, 0);
            /*do {
                
                //if(tpid != child_pid) process_terminated(tpid);
            } while(tpid != child_pid);*/

            printf("DEBUG: Child exited: %d | %d\n", tpid, child_status);
        }

        return child_pid;
    }
}

int main(int argc, char *argv[]) {
    
    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        freopen(argv[2], "r", stdin);
    }

    char* params[P];
    int children[N];
    int children_size = 0;

    while(1) {
        int param_length = 0;
        while ((param_length = readLine(params)) == 0) {}

        // Copy arguments
        for (int i = 0; i < param_length; i++) {
            printf("DEBUG: params[%d] = %s\n", i, params[i]);
        }

        if (strcmp(params[0],"quit") == 0 && param_length == 1) {
            printf("Goodbye!\n");
            exit(EXIT_SUCCESS);
        } else if (strcmp(params[0],"childs") == 0 && param_length == 1) {
            for (int j = 0; j < children_size; j++) {
                int child_status;
                waitpid(children[j], &child_status, WNOHANG);
                bool exited = WIFEXITED(child_status);
                printf("pid=%d: %s(%d)\n", children[j], (exited ? "EXITED" : "RUNNING"), child_status);
            }
        } else {
            // Execute program
            bool ctrl = (strcmp(params[param_length-1], "&") == 0);

            if (ctrl) {
                params[param_length-1] = NULL;
            }

            int child_pid = runCommand(params[0], params, !ctrl);

            if (ctrl) {
                children[children_size] = child_pid;
                children_size++;            
            }
        }

        for (int i = 0; i < P; i++) {
            params[i] = NULL;
        }
    }

    return 0;
}
