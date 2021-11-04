
# Aufgabe 1
### Teil 1

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/wait.h>

    int main () {
    if (fork () == 0) {
    //child process
    while(1) {
        printf("Child: pid->%ld ppid->%ld\n", (long)getpid(), (long)getppid());
    }
    } else {
    //parent
    while(1) {
        printf("Parent: pid->%ld\n", (long)getpid());
    }
    }

    return 0;
    }

### Teil 2
pstree:
![image](./images/pstree.png)

ps -af:
![image](./images/ps_prior_kill.png)

### Teil 3
Der Child Prozess läuft auch nach Beendigung des Parent Prozesses weiter. Anstatt das abwechselnd vom Child und Parent der print Befehl ausgeführt wird, kommt nur noch der Child Prozess zum Zug. 

Vom Child verändert sich die PID nicht, aber natürlich ist die PPID nicht mehr die vorherige (einleuchtend, da der Parent Prozess gekillt wurde).

![image](./images/ps_after_kill.png)


