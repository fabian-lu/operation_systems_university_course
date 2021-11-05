#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void intHandler(int dummy) {
	printf("Machts gut und danke für den Fisch!\n");
	exit(EXIT_SUCCESS);
}

int main() {

	signal(SIGINT, intHandler);

	if (fork() != 0) {
		// Parent
		while (1) {
			printf("Parent: pid->%d\n", getpid());
			sleep(1);
		}
	} else {
		// Child
		while (1) {
			printf("Child: pid->%d ppid->%d\n", getpid(), getppid());
			sleep(1);
		}
	}

	return EXIT_SUCCESS;
}
