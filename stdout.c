#include "shell.h"

/*---handles stdout write redirector---*/
int stdout_write(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '>'\n");
        return 1;
    }

    // Save the original stdout file descriptor
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdout < 0) {
        perror("dup");
        return 1;
    }
    
    // open file in write mode, creates new if not exist, overwrites if exists.
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // error with opening
    if (fd < 0) {
        perror("open");
        close(original_stdout);
        return 1;
    }
    // redirecting and error with writing
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        close(original_stdout);
        return 1;
    }
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    int status = handle_input(args);

    if (dup2(original_stdout, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(original_stdout);
        return 1;
    }

    close(original_stdout);

    return status;    
}



/*---handles stdout append redirector---*/
int stdout_append(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '>>'\n");
        return 1;
    }

    // Save the original stdout file descriptor
    int original_stdout = dup(STDOUT_FILENO);
    if (original_stdout < 0) {
        perror("dup");
        return 1;
    }
    
    // open file in append mode, creates new if not exist, appends if exists.
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
    // error with opening
    if (fd < 0) {
        perror("open");
        close(original_stdout);
        return 1;
    }
    // redirecting and error with writing
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        close(original_stdout);
        return 1;
    }
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    int status = handle_input(args);

    if (dup2(original_stdout, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(original_stdout);
        return 1;
    }

    close(original_stdout);

    return status;    
}
