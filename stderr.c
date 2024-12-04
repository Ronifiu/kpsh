#include "shell.h"

/*---handles stderr write redirector---*/
int stderr_write(char **args, int index) {
    // Handle syntax error:
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '2>'\n");
        return 1;
    }

    // open file in write mode, overwrite if it exists, creates if not
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // error with opening
    if (fd < 0) {
        perror("open");
        return 1;
    }
    // redirecting and error with writing
    if (dup2(fd, STDERR_FILENO) < 0) {
        close(fd);
        perror("dup2");
        return 1;
    }
    close(fd);

    // Removing the redirector and file from args
    shift_args_left(args, index);

    return 0;
}

/*---handles stderr append redirector---*/
int stderr_append(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '2>>'\n");
        return 1;
    }
    
    // open file in append mode, creates new if not exist, appends if exists.
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    // error with opening
    if (fd < 0) {
        perror("open");
        return 1;
    }
    // redirecting and error with writing
    if (dup2(fd, STDERR_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    return 0;    
}
