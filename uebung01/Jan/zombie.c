#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void intHandler(int dummy) {
	exit(EXIT_SUCCESS);
}

int main() {

	signal(SIGINT, intHandler);

	pid_t child_pid = fork();

	pid_t second_child_pid = fork();

	if (child_pid != 0 && second_child_pid != 0) {
		// Parent
		sleep(100);

		int wstatus;

		wait(&wstatus);
		//waitpid(child_pid, &wstatus, 0);
	} else {
		// Child
		exit(EXIT_SUCCESS);
	}

	return EXIT_SUCCESS;
}
