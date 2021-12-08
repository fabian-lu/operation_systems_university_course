#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Benoetigt ein Argument, welches die Wartezeit in Sekunden ist.
int main(int c, char** v) {
    if (c == 2) {
        int wait_time = atoi(v[1]);
        printf("Warte %d Sekunden...\n", wait_time);
        sleep(wait_time);
    }
}