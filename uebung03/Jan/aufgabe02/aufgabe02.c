#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 80


int main(int argc, char *argv[]) {
	if (argc != 2) exit(EXIT_FAILURE);

	char buffer[N];

	char *str = argv[1];
	
	int i;
	for(i = 0; *str != '\0' && i < N; i++) {
		buffer[i] = *str;
		str++;
	}

	while(i < N) {
		buffer[i] = '\0';
		i++;
	}


	int fd[2];

	pipe(fd);

	if (fork() == 0) {
		// Child

		close(fd[1]);

		char child_buffer[N];

		printf("Child: Reading...\n");
		read(fd[0], child_buffer, N);
		printf("Child: Reading finished.\n");

		printf("%s\n", child_buffer);

		printf("Child: Exited\n");
		return 0;
	}

	close(fd[0]);

	printf("Parent: First write\n");
	write(fd[1], buffer, N/2);

	printf("Parent: Waiting 5 seconds...\n");
	sleep(5);

	printf("Parent: Second write\n");
	write(fd[1], (buffer+sizeof(char)*N/2), N/2);	

	printf("Parent: Exited\n");
	return 0;
}
