#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define N 100


int main(int argc, char *argv[]) {
	if (argc != 2) exit(EXIT_FAILURE);

	char *input = argv[1];

	int fd_parent_child[2];
	int fd_child_parent[2];

	pipe(fd_parent_child);
	pipe(fd_child_parent);

	if (fork() == 0) {
		// Child

		close(fd_parent_child[1]);
		close(fd_child_parent[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);

		dup2(fd_parent_child[0], STDIN_FILENO);
		dup2(fd_child_parent[1], STDOUT_FILENO);

		execl("/usr/bin/bc", "bc", (char*)NULL);

		return 0;
	}

	close(fd_parent_child[0]);
	close(fd_child_parent[1]);

	write(fd_parent_child[1], input, strlen(input));
	write(fd_parent_child[1], "\n", 1);

	char buffer[N];

	read(fd_child_parent[0], buffer, N);

	printf("%s", buffer);

	return 0;
}