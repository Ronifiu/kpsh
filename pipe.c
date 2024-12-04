#include "shell.h"

/*---handles piping redirector---*/
int pipe_function(char** args, int index) {
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '|'\n");
    }

    // dividing args into two subarrays:
    int left_size = index;
    int right_size = 0;
    for (int i = index + 1; args[i] != NULL; i++) {
        right_size++;
    }

    // allocating memory for new arrays
    char **left_args = malloc((left_size + 1) *sizeof(char*));
    char **right_args = malloc((right_size + 1) * sizeof(char*));
    // handle error
    if (left_args == NULL || right_args == NULL) {
        free(left_args);
        free(right_args);
        perror("Cannot allocate memory for args");
        return 1;
    }
    // copying left elements
    for (int i = 0; i < left_size; i++) {
        left_args[i] = args[i];
    }
    left_args[left_size] = NULL;

    // copying right elements
    for (int i = 0; args[index + 1 + i] != NULL; i++) {
        right_args[i] = args[index + i + 1];
    }
    right_args[right_size] = NULL;

    // piping:
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        free(left_args);
        free(right_args);
        perror("pipe");
        return 1;
    }

    // fork the first process
    if ((pid1 = fork()) == -1) {
        free(left_args);
        free(right_args);
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        // executes the first process
        if (dup2(pipefd[1], STDOUT_FILENO) < 0) {
            free(left_args);
            free(right_args);
            perror("dup2");
            close(pipefd[1]);
            return 1;
        }
        close(pipefd[0]);
        close(pipefd[1]);

        handle_input(left_args);
        perror("handle input");
        exit(EXIT_FAILURE);
    }

    // forks the second process
    if ((pid2 = fork()) == -1) {
        free(left_args);
        free(right_args);
        perror("fork");
        return 1;
    } 

    if (pid2 == 0) {
        // executes the second process
        if (dup2(pipefd[0], STDIN_FILENO) < 0) {
            free(left_args);
            free(right_args);
            perror("dup2");
            close(pipefd[0]);
            return 1;
        }
        close(pipefd[0]);
        close(pipefd[1]);

        handle_input(right_args);
        perror("handle input");
        exit(EXIT_FAILURE);   
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    free(left_args);
    free(right_args);

    shift_args_left(args, index);

    return 0;
}