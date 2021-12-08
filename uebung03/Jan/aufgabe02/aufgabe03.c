#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>

#define N 1000


void intHandler(int dummy) {
	printf("Exit\n");
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[]) {
	signal(SIGINT, intHandler);

	if (argc != 2) exit(EXIT_FAILURE);

	char *pipename = argv[1];
	
	int fd = open(pipename, O_RDONLY);

	char buffer[N];

	while(1) {
		if (read(fd, buffer, N) > 0) {
			printf("%s", buffer);
			memset(buffer, '\0', N);
		}
	}

	return 0;
}
