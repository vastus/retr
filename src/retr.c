#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void usage()
{
	printf("usage: retr COMMAND\n");
}

static int run(char *argv[])
{
	int status;
	pid_t pid = fork();

	switch (pid) {
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0:
			execvp(argv[0], argv);
			perror("exec");
			exit(EXIT_FAILURE);
		default:
			waitpid(pid, &status, 0);
	}

	if (WIFEXITED (status)) {
		return WEXITSTATUS (status);
	}

	return -1;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage();
		exit(EXIT_FAILURE);
	}

	int rc = -1;
	int retry_count = 2;
	int rest_in_secs = 1;
	struct timespec rest = { .tv_sec = rest_in_secs, .tv_nsec = 0 };

	for (int i = 0; i < retry_count; ++i) {
		if ((rc = run(&argv[1])) == 0) {
			return 0;
		}
		nanosleep(&rest, NULL);
	}

	return rc;
}
