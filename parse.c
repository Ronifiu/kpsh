#include "shell.h"

/*---function parses the input into tokens based on the delimeter---*/
char** parse_line(char* line) {
	int counter = 0;
	size_t bufsize = 32;
	char *token;
	char **tokens = (char **)malloc(bufsize*sizeof(char*));
	if (tokens == NULL) {
		perror("cannot allocate memory for tokens");
		exit(EXIT_FAILURE);
	}
	/*---tokenizing---*/
	token = strtok(line, DELIM);

	// error handling from source 1
	while (token != NULL) {
		// handle comments:
		if (token[0] == '#')
			break;
		// iterate tokens:
		tokens[counter] = token;
		counter++;
		// if too many token:
		if (counter >= bufsize) {
			bufsize += bufsize;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			// handle error
			if (tokens == NULL) {
				perror("reallocation failed");
				exit(EXIT_FAILURE);
			}
		}
		// next token
		token = strtok(NULL, DELIM);
		
	}
	// last token
	tokens[counter] = token;

	return tokens;
}