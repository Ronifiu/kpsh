#include "shell.h"

int redirect_checker(char** args) {
    if (args == NULL) {
        fprintf(stderr, "Error: no arguments passed to redirect_checker\n");
        return 1;
    }

    char* redirectors[] = {
        "|",
        ">",
        ">>",
        "<",
        "<<",
        "2>",
        "2>>"
    };

    int (*redirector_functions[])(char **, int) = {
        &pipe_function,
        &stdout_write,
        &stdout_append,
        &stdin_read,
        &heredoc,
        &stderr_write,
        &stderr_append
    };

    int i = 0;
    while (args[i] != NULL) {
        for (int j = 0; j < sizeof(redirectors) / sizeof(char*); j++) {
            if (args[i] != NULL && strcmp(args[i], redirectors[j]) == 0) {
                if (redirector_functions[j](args, i) != 0) {
                    fprintf(stderr, "Error handling redirector: %s\n", redirectors[j]);
                    return 1;
                }
                return 0;
            }
        }
        i++;
    }

    return 0;
}

void shift_args_left(char **args, int index) {
    // Shift the elements in args
    int i = index;
    while (args[i] != NULL) {
        args[i] = args[i + 2]; 
        i++;
    }
}

