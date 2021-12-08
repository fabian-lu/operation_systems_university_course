#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define N 80


int main(int argc, char *argv[]) {
	if (argc != 2) exit(EXIT_FAILURE);

	
	int fd = open(argv[1], O_WRONLY | O_CREAT, S_IRWXU);
	
	close(STDOUT_FILENO);
	
	dup2(fd, STDOUT_FILENO);

	printf("Hello World!\n");
	
	close(fd);
	
	return 0;
}
