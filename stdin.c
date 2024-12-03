#include "shell.h"

/*
int stdin_read(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '<'\n");
        return 1;
    }
    
    // open file in read only mode
    int fd = open(args[index + 1], O_RDONLY);
    // error with opening
    if (fd < 0) {
        perror("open");
        return 1;
    }
    // redirecting and error with reading
    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    return 0;    
}
*/

int stdin_read(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '<'\n");
        return 1;
    }

    // Save the original stdout file descriptor
    int original_stdin = dup(STDIN_FILENO);
    if (original_stdin < 0) {
        perror("dup");
        return 1;
    }
    
    // open file in write mode, creates new if not exist, overwrites if exists.
    int fd = open(args[index + 1], O_RDONLY);
    // error with opening
    if (fd < 0) {
        perror("open");
        close(original_stdin);
        return 1;
    }
    // redirecting and error with writing
    if (dup2(fd, STDIN_FILENO) < 0) {
        perror("dup2");
        close(fd);
        close(original_stdin);
        return 1;
    }
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    int status = handle_input(args);

    if (dup2(original_stdin, STDIN_FILENO) < 0) {
        perror("dup2");
        close(original_stdin);
        return 1;
    }

    close(original_stdin);

    return status;    
}

int heredoc(char** args, int index) {
    // handle syntax error
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected delimeter after '<<'\n");
        return 1;
    }

    char *delimeter = args[index + 1];

    // create pipe and handle error
    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return 1;
    }
    
    char buffer[1024];
    printf(">");
    while (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, delimeter) == 0) {
            break;
        }

        write(pipefd[1], buffer, strlen(buffer));
        write(pipefd[1], "\n", 1);
        printf(">");
    }

    close(pipefd[1]);

    if (dup2(pipefd[0], STDIN_FILENO) < 0) {
        perror("dup2");
        close(pipefd[0]);
        return 1;
    }

    close(pipefd[0]);

    // removing the redirector and file from args
    shift_args_left(args, index);

    return 0;    
}