/*
 ============================================================================
 Name        : pipe.c
 Author      : stephen
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define	MAX_LINE	20

int main(void) {

	int n;
	int fd[2];
	pid_t pid;
	char line[MAX_LINE];
	char send_data[] = "hello world\n";


	if (pipe(fd) < 0)
		printf("pipe error\n");

	if ((pid = fork()) < 0)
		printf("fork error\n");
	else if (pid > 0)
	{
		close(fd[0]);
		write(fd[1], send_data, strlen(send_data));
	}
	else
	{
		close(fd[1]);
		n = read(fd[0], line, MAX_LINE);
		write(STDOUT_FILENO, line, n);
	}


	return EXIT_SUCCESS;
}
