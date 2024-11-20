#include "shell.h"

int main() {
	if (isatty(STDIN_FILENO)) {
		// shell is running in interactive mode
		shell_interactive();
		
	} else {
		// shell is running in non-interactive mode
		// execute commands from script or batch file
	}

	return 0;
}