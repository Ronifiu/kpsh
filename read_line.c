#include "shell.h"
/*---function returns the string the user typed in---*/
char* read_line() {
	char *buffer;
	size_t bufsize = 32;

	buffer = (char*)malloc(bufsize*sizeof(char));
	// if allocation failed:
	if (buffer == NULL) {
		perror("unable to allocate buffer");
		exit(EXIT_FAILURE);
	}
	// reading line:
	// getline() automatically reallocates memory
	// if line is too long
	if (getline(&buffer, &bufsize, stdin) == -1) {
		free(buffer);    // avoid memory leak
		// error handling from source 1
		// if eof is entered:
		if (feof(stdin)) {
			exit(EXIT_SUCCESS);
		// if something else went wrong:
		} else {
			perror("reading line failed");
			exit(EXIT_FAILURE);
		}
	}
	return buffer;
}