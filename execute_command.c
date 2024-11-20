#include "shell.h"

/*---executes the command from the PATH---*/
int execute_commands(char** args) {
    char* command = args[0];
    char** argument_list = args;
    pid_t pid;
    int status_code;

    // creating a child process:
    pid = fork();
    
    if (pid == 0) {
        // child process doesn't exists or works
        if (execvp(command, argument_list) == -1) {
            perror("error with child process");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // error with forking:
        perror("error forking");
    } else {
        // parent process:
        do {
            waitpid(pid, &status_code, WUNTRACED);
        } while (WIFEXITED(status_code) == 0 && WIFSIGNALED(status_code) == 0);
    }

    return 1;
}