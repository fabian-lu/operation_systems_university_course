#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
    int zombie_count = 4;
    for (int i = 0; i < zombie_count; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Als child direkt wieder beenden
            exit(EXIT_SUCCESS);
        }
        else {
            printf("Created zombie PID=%d\n", pid);
        }
    }

    pid_t wait_proc = fork();
    if (wait_proc == 0) { // Idle-Prozess starten.
        printf("Wait process PID=%d\n", getpid());
        while (true) {}
    }
    else {
        printf("Parent PID=%d\n", getpid());
        int status;
        waitpid(wait_proc, &status, 0);
    }
}

/*
AUSGABE PROGRAMM:

Created zombie PID=4907
Created zombie PID=4908
Created zombie PID=4909
Created zombie PID=4910
Parent PID=4906
Wait process PID=4911

AUSGABE WATCH:

Every 2.0s: ps -A | grep zombie          DESKTOP-HGCQHRC: Fri Nov 12 15:59:49 2021
 4906 tty1     00:00:00 uebung1_zombie
 4907 tty1     00:00:00 uebung1_zombie <defunct>
 4908 tty1     00:00:00 uebung1_zombie <defunct>
 4909 tty1     00:00:00 uebung1_zombie <defunct>
 4910 tty1     00:00:00 uebung1_zombie <defunct>
 4911 tty1     00:00:27 uebung1_zombie
*/