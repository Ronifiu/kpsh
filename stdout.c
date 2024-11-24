#include "shell.h"

/*
int stdout_write(char **args, int index) {
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
    
    // open file in write mode, creates new if not exist, overwrite if exists.
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
*/

/*
int stdout_write(char **args, int index) {
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

    // open file in write mode, creates new if not exist, overwrite if exists.
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // error with opening
    if (fd < 0) {
        perror("open");
        close(original_stdout);
        return 1;
    }

    // redirect output
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        close(original_stdout);
        return 1;
    }
    

    // Close the file descriptor
    close(fd);

    // removing the redirector and file from args
    shift_args_left(args, index);

    // Ensure that anything printed before redirection is flushed
    fflush(stdout);

    // Now handle input after redirection
    int status = handle_input(args);

    // Restore original stdout
    if (dup2(original_stdout, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(original_stdout);
        return 1;
    }

    close(original_stdout);

    return status;
}

int stdout_write(char** args, int index) {
    if (args[index + 1] == NULL) {
        fprintf(stderr, "Syntax error: expected file after '>'\n");
        return 1;
    }

    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);

    shift_args_left(args, index);

    return 0;  // Redirection successful
}


int stdout_write(char **args, int index) {
    // Handle syntax error: if no file is provided after '>'
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

    // Open the file for writing (creates or overwrites the file)
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        close(original_stdout); // Clean up original stdout file descriptor
        return 1;
    }
    fprintf(stderr, "Redirection file descriptor: %d\n", fd);

    close(STDOUT_FILENO);
    // Redirect stdout to the file
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        close(original_stdout);
        return 1;
    }
    close(fd); // Close file descriptor

    // Shift args to remove the redirect operator and the filename
    shift_args_left(args, index);

    // Execute the command after redirection (using the restored stdin)
    fprintf(stderr, "Redirecting output to file\n");
    int status = handle_input(args);
    fprintf(stderr, "Output redirection complete. Now restoring stdout\n");

    fprintf(stderr, "STDOUT_FILENO before: %d\n", STDOUT_FILENO);

    // Restore the original stdout file descriptor
    fprintf(stderr, "Restoring stdout\n");
    if (dup2(original_stdout, STDOUT_FILENO) < 0) {
        perror("dup2 failed to restore stdout");
        close(original_stdout);
        return 1;
    }
    fprintf(stderr, "STDOUT_FILENO before: %d\n", STDOUT_FILENO);

    // Close the original stdout file descriptor
    close(original_stdout);

    return status;
}
*/
int stdout_write(char **args, int index) {
    // Handle syntax error: if no file is provided after '>'
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

    // Open the file for writing (creates or overwrites the file)
    int fd = open(args[index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        close(original_stdout); // Clean up original stdout file descriptor
        return 1;
    }

    // Fork to handle the redirection
    pid_t pid = fork();
    if (pid == 0) { // Child process
        // Close the original stdout in the child process
        close(STDOUT_FILENO);

        // Redirect stdout to the file
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            close(original_stdout);
            exit(1); // Exit child process on error
        }

        // Close the file descriptor in the child process
        close(fd);

        // Shift args to remove the '>' and the filename
        shift_args_left(args, index); // This removes the redirection operator and its filename


        // Execute the command with the remaining arguments
        int status = execvp(args[0], args); // Directly using execvp here
        if (status == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE); // Exit child process on execvp failure
        }

        exit(0); // Exit child process after execution
    } else if (pid < 0) { // Error forking
        perror("fork");
        close(fd);
        close(original_stdout);
        return 1;
    } else { // Parent process
        // Close the file descriptor in the parent process
        close(fd);

        // Wait for the child process to finish
        int status_code;
        waitpid(pid, &status_code, 0);

        // Restore stdout in the parent process
        if (dup2(original_stdout, STDOUT_FILENO) < 0) {
            perror("dup2 failed to restore stdout");
            close(original_stdout);
            return 1;
        }

        // Close the original stdout file descriptor
        close(original_stdout);

        return WEXITSTATUS(status_code); // Return the exit status of the child process
    }
}




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
