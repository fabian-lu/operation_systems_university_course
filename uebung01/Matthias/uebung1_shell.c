#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define L 80 // Maximale Eingabezeilenlaenge
#define P 10 // Maximale Parameterzahl
#define MAX_CHILDS 10 // Maximale Anzahl an Kindprozessen

// Liste der Hintergrundprozesse.
// Die PIDs an einem Index haben nur bedeutung,
// wenn die dazugehoerige Stelle in CHILD_RUNNING true ist.
pid_t CHILDS[MAX_CHILDS];
bool CHILD_RUNNING[MAX_CHILDS];

// Markiere beendete Kindprozesse in der CHILD_RUNNING liste.
void checkOnChildren();
// Liste alle Hintergrundprozesse.
void listChildren();
// Starte einen Vordergrundprozess.
void execute(char* program, char *params[]);
// Starte einen Hintergrundprozess.
void executeWait(char* program, char *params[]);

// Markiere beendete Kindprozesse in der CHILD_RUNNING liste.
void checkOnChildren() {
    int status;
    for (int i = 0; i < MAX_CHILDS; i++) {
        if (CHILD_RUNNING[i]) {
            int waitRet = waitpid(CHILDS[i], &status, WNOHANG);
            if (waitRet >= 1) { // waitRet ist PID, falls Prozess beendet.
                CHILD_RUNNING[i] = false;
            }
        }
    }
}

// Liste alle Hintergrundprozesse.
void listChildren() {
    checkOnChildren();
    printf("# Running children:\n");
    int counter = 1;
    for (int i = 0; i < MAX_CHILDS; i++) {
        if (CHILD_RUNNING[i]) {
            printf("# Child %d: PID %d\n", counter, CHILDS[i]);
            counter ++;
        }
    }
}

// Starte einen Hintergrundprozess.
void executeWait(char* program, char *params[]) {
    pid_t child_pid = fork();
    if (child_pid == 0) { // Kindprozess
        execv(program, params);
        perror("Error at execv");
        exit(-1);
    }
    else { // Elternprozess
        int status;
        waitpid(child_pid, &status, 0);
    }
}

// Starte einen Vordergrundprozess.
void execute(char* program, char *params[]) {
    checkOnChildren();
    int child_index = -1;
    for (int i = 0; i < MAX_CHILDS; i++) {
        if (!CHILD_RUNNING[i]) {
            child_index = i;
            break;
        }
    }
    if (child_index < 0) {
        printf("Can only host %d child processes.\n", MAX_CHILDS);
    }

    pid_t child_pid = fork();
    if (child_pid == 0) { // Kindprozess
        execv(program, params);
        perror("Error at execv");
        exit(-1);
    }
    else { // Elternprozess
        CHILDS[child_index] = child_pid;
        CHILD_RUNNING[child_index] = true;
    }
}

int main(int argc, char *argv[]) {
    // Entscheide zwischen stdin und einer Datei.
    // Setze from_file entsprechend.
    FILE* input = stdin;
    bool from_file = false;
    if (argc == 3 && strcmp(argv[1], "-f") == 0) {
        input = fopen(argv[2], "r");
        if (input == NULL) {
            perror("Failed to open script");
            input = stdin;
        }
        else {
            from_file = true;
        }
    }

    while (true) {
        // Aus dem Template...

        char* params[P];
        int paramcount;
        char line[L];
        char delim[] = " "; 
        
        int i = 0;
        while(i < L && ((line[i] = fgetc(input)) != '\n' && line[i] != EOF))
            i++;
        
        char buf;
        if (line[i] != '\n')
            while((buf = fgetc(input)) != '\n' && buf != EOF); // EOF hinzugefuegt
        line[i] = '\0';
        if (from_file) {
            printf("> %s\n", line);
        }
        
        i = 0;
        char *p = strtok(line, delim);
        while(i < P && (params[i] = p) != NULL) {
            p = strtok(NULL, delim);
            i++;
            paramcount = i; // Paramcount hinzugefuegt
        }
        params[i] = NULL;

        // Eigener Code

        if (paramcount == 0) { /* NICHTS */ }
        else if (paramcount == 1 && strcmp(params[0], "childs") == 0) {
            listChildren();
        }
        else if (paramcount == 1 && strcmp(params[0], "quit") == 0) {
            if (from_file) fclose(input);
            return 0;
        }
        else if (paramcount >= 1 && access(params[0], X_OK) == 0) {
            // ^^^ gueltige Datei als Eingabe
            char* program = params[0];
            bool fork = (paramcount >= 2 && strcmp(params[paramcount - 1], "&") == 0);
            if (fork) {
                params[paramcount - 1] = NULL; // '#' Loeschen
                execute(program, params);
            }
            else {
                executeWait(program, params);
            }
        }
        else {
            if (from_file) fclose(input);
            printf("Ungueltige Eingabe: /%s/\n", params[0]);
            return -1;
        }
    }
}