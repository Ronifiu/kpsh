#ifndef SHELL_H
#define SHELL_H

/*---LIBRARIES---*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

/*---MACROS---*/
#define DELIM " \t\r\n\a\""

/*---STRUCTURES---*/
struct Node {
	char *dir;
	struct Node *next;
};


/*---PROTOTYPES---*/
void shell_interactive(void);
void shell_non_interactive(void);
void get_prompt();
char* read_line();
char* read_stream();
char** parse_line(char* line);
int handle_input(char** args);
int execute_commands(char** args);
int redirect_checker(char** args);
void shift_args_left(char** args, int index);

// redirect functions:
int pipe_function(char** args, int index);
int stdout_write(char** args, int index);
int stdout_append(char** args, int index);
int stdin_read(char** args, int index);
int heredoc(char** args, int index);
int stderr_write(char** args, int index);
int stderr_append(char** args, int index);

#endif