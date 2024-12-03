#include "shell.h"

/*---handling shell running in script mode---*/
void shell_non_interactive(void) {
    // mostly same as shell_interactive
    int status = 0; // for the loop

    do {
        char *line = read_stream(); // bit different from read_line, difference noticed from source 1
        char ** args = parse_line(line);
        status = handle_input(args);

        if (status < 0) {
			fprintf(stderr, "Command '%s' not found\n", args[0]);
		}

		// freeing allocated memory in the heap
		// to avoid memory leaks
		free(line);
		free(args);
		
        // if failed
		if (status > 0) {
			fprintf(stderr, "Command failed with status: %d\n", status);
			exit(status);
		}

    }
    while (status <= 0);
}