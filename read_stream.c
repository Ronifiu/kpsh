#include "shell.h"

/*---reads a line from a stream---*/
char *read_stream() {
    int bufsize = 1024;
    int i = 0;
    char *line = malloc(sizeof(char) * bufsize);
    int c;

    // if allocation failed
    if (line == NULL) {
        fprintf(stderr, "cannot allocate memory for line");
        exit(EXIT_FAILURE);
    }

    // reading the stream
    while (1) {
        c = getchar();
        if (c == EOF) {
            free(line);
            exit(EXIT_SUCCESS);
        } else if (c == '\n') {
            line[i] = '\0';
            return (line);
        } else {
            line[i] = c;
        }
        i++;

        // if stream is longer than allocated array
        // double the size of array
        if (i >= bufsize) {
            bufsize += bufsize;
            line = realloc(line, bufsize);
            // if allocation failed:
            if (line == NULL) {
                fprintf(stderr, "reallocation failed for line");
                exit(EXIT_FAILURE);
            }
        }
    }
}