/* necessary modules imported */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "logic.h"

#define ALLOC_ERROR "Shell: allocation error\n"
#define TOKEN_DELIM " \t\r\n\a"

#define BUFFSIZE 1024
#define TOKEN_BUFFSIZE 64

#define NUM_BUILTINS(x) (sizeof(x) / sizeof(char *))

/* function prototypes */
int cd_command(char **args);
int help_command(char **args);
int exit_command(char **args);
int cat_command(char **args);

/* commands created */
const char *builtin_str[] = {
	"cd",
	"help",
	"exit",
	"cat"
};

/* function arguments to make function calls simpler */
int (*builtin_func[]) (char **) = {
	&cd_command,
	&help_command,
	&exit_command,
	&cat_command
};

/* function which reads a line */
char *read_line(void) {
	int buffsize = BUFFSIZE, position = 0, ch;
	char *buffer = NULL;
	void *temp = NULL;

    /* dynamically allocated space to store line */
	buffer = malloc(buffsize+1);
	if (!buffer) {
		fprintf(stderr, ALLOC_ERROR);
		exit(EXIT_FAILURE);
	}
    
    /* main loop for reading a line */
    /* could also use getline here */
	while (true) {
		ch = getchar();

		if (ch == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else if (ch == EOF) {
			exit(EXIT_SUCCESS);
		}

		buffer[position++] = ch;
        
        /* reallocating more space for buffer */
		if (position >= buffsize) {
			buffsize *= 2;
			temp = realloc(buffer, buffsize);
			if (!temp) {
				fprintf(stderr, ALLOC_ERROR);
				exit(EXIT_FAILURE);
			}
			buffer = temp;
		}
	}
}

/* function for parsing a line */
char **split_line(char *line) {
	int buffsize = TOKEN_BUFFSIZE, position = 0;
	char **tokens = NULL;
	char *token = NULL, *copy = NULL;
	void *temp = NULL;
	const char *delim = TOKEN_DELIM;
    
    /* tokens storage */
	tokens = malloc(buffsize * sizeof(*tokens));
	if (!tokens) {
		fprintf(stderr, ALLOC_ERROR);
		exit(EXIT_FAILURE);
	}

	copy = line;
	token = strtok(copy, delim);

    /* safely parses the line */
	while (token != NULL) {
		tokens[position++] = token;

		if (position >= buffsize) {
			buffsize *= 2;
			temp = realloc(tokens, buffsize);
			if (!temp) {
				fprintf(stderr, ALLOC_ERROR);
				exit(EXIT_FAILURE);
			}
			tokens = temp;
		}

		token = strtok(NULL, delim);
	}

	tokens[position] = NULL;

	return tokens;
}

/* function which launches commands */
int launch(char **args) {
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("Shell");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("Shell");
	} else {
		do {
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

/* function which excecutes commands */
int execute(char **args) {
	int i;

	if (args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < NUM_BUILTINS(builtin_str); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	return launch(args);
}

/* cd command */
int cd_command(char **args) {
	if (args[1] == NULL) {
		fprintf(stderr, "Shell: expected arguement to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("Shell");
		}
	}

	return 1;
}

/* help command */
int help_command(char **args) {
	int i;

	printf("Basic Shell\n");
	printf("Type program names and arguements, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < NUM_BUILTINS(builtin_str); i++) {
		printf("  %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs.\n");
  	return 1;
}

/* Nice and short exit command */
int exit_command(char **args) {
	return 0;
}

/* cat command */
int cat_command(char **args) {
    int i, ch;
	FILE *file;

	if (args[1] == NULL) {
		fprintf(stderr, "Shell: expected arguement to \"cat\"\n");
	}

	for (i = 1; args[i] != NULL; i++) {
		file = fopen(args[i], "r");
		if (!file) {
			fprintf(stderr, "cat: %s: No such file or directory\n", args[i]);
			break;
		}

		while ((ch = fgetc(file)) != EOF) {
			putchar(ch);
		}

		fclose(file);
	}

	return 1;
}






