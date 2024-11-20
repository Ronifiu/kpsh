#include "shell.h"

void get_prompt() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	char *username = getlogin();
	char hostname[1024];
	gethostname(hostname, sizeof(hostname));
	printf("%s@%s:%s$ ", username, hostname, cwd);
}