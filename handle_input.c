#include "shell.h"

/*---my implementation of exit---*/
// exits the shell
int my_exit(char **args);
/*---my implementation of echo---*/
// prints the input after echo
int my_echo(char **args);
/*---my implementation of cd---*/
// chages directory
int my_cd(char **args);
/*---my implementation of pwd---*/
// prints current working directory
int my_pwd(char **args);

/*---executes the given commands---*/
int handle_input(char** args) {
	// the implemented commands:
	char *built_in_commands[] = {
		"exit",
		"echo",
		"cd",
		"pwd"
	};
	// their functions:
	int (*built_in__functions[])(char **) = {
		&my_exit,
		&my_echo,
		&my_cd,
		&my_pwd
	};
	// this built in function handling from source 1
	// if nothing was entered
	if (args[0] == NULL) 
		return 1;
	// if the command is built in:
	for (int i=0; i < sizeof(built_in_commands) / sizeof(char *); i++) {
		if (strcmp(args[0], built_in_commands[i]) == 0)
			return ((*built_in__functions[i])(args));
	}
	
	// if the command is not built in:
	return (execute_commands(args));
}

int my_exit(char **args) {
	// exists the program
	return 0;
}

int my_echo(char **args) {
	int i = 1;
	// loop through args and prints
	while (args[i] != NULL) {
		printf("%s ", args[i]);
		i++;
	}
	printf("\n");
	return 1;
}

int my_cd(char **args) {
	// change to home:
	if (args[1] == NULL) {
		char *home = getenv("HOME");
		// home variable does not exist
		if (home == NULL) {
			perror("HOME not set");
			exit(EXIT_SUCCESS);
		}
		// cd failed
		if (chdir(home) != 0) {
			perror("cd");
		}
	// change elsewhere:
	} else {
		if (chdir(args[1]) != 0) {
			perror("cd");
		}
	}
	return 1;
}

int my_pwd(char** args) {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s\n", cwd);
	return 1;
}