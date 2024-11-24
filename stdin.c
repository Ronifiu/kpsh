#include "shell.h"

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