#ifndef SHELL_H
#define SHELL_H

/*---LIBRARIES---*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/*---MACROS---*/
#define DELIM " \t\r\n\a\""

/*---STRUCTURES---*/
struct Node {
	char *dir;
	struct Node *next;
};


/*---PROTOTYPES---*/
void shell_interactive(void);
void get_prompt();
char* read_line();
char** parse_line(char* line);
int handle_input(char** args);
int execute_commands(char** args);

#endif