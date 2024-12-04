#include "shell.h"

/*---prints the prompt for the user---*/
void get_prompt() {
	if (isatty(STDOUT_FILENO)) {
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		char *username = getlogin();
		char hostname[1024];
		gethostname(hostname, sizeof(hostname));
		fprintf(stdout, "%s@%s:%s$ ", username, hostname, cwd);
	}
}