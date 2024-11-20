#include "shell.h"

void shell_interactive() {
	int status = 1; // for the loop
	do {
		get_prompt();	// printing the prompt
		char *line = read_line(); // getting the input as string
		char **args = parse_line(line); // parsing the input into an array of strings
		status = handle_input(args); // executing the commands

		// freeing allocated memory in the heap
		// to avoid memory leaks
		free(line);
		free(args);
	}
	while(status);
}