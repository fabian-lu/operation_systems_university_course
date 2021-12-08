#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>

void sigint_handler() {
    printf("Tschuessi!\n");
    exit(EXIT_SUCCESS);
}

int main() {
    pid_t fork_pid = fork();
    signal(SIGINT, sigint_handler);
    if (fork_pid == 0) { // child
        while (true) {
            pid_t pid = getpid();
            pid_t ppid = getppid();
            printf("Child: PID=%d, PPID=%d\n", pid, ppid);
            sleep(1);
        }
    } else { // parent
        while (true) {
            pid_t pid = getpid();
            printf("Parent: PID=%d\n", pid);
            sleep(1);
        }
    }
}

/*
Parent: PID=4540
Child: PID=4541

matthias@DESKTOP-HGCQHRC:/mnt/c/Users/Matthias$ pstree 4540
uebung1_a───uebung1_a
matthias@DESKTOP-HGCQHRC:/mnt/c/Users/Matthias$ pstree 4541
uebung1_a
matthias@DESKTOP-HGCQHRC:/mnt/c/Users/Matthias$ pstree 9
bash───make───uebung1_a───uebung1_a

Nach 'kill 4540':
Child prozess laeuft weiter, PID=4541 unveraendert, PPID=1 (init)
*/