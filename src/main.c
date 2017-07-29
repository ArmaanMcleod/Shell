/* necessary modules included */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logic.h"

#define MAX_PATH 1024

/* main loop for running shell commands */
void loop(void) {
	char *line = NULL;
	char **args = NULL;
	int status;

	printf("MyShell\n");
	printf("Copyright (C) 2017 Segmentation Fault Corporation. All rights reserved.\n\n");

	do {
		char cwd[MAX_PATH+1];
   		if (getcwd(cwd, sizeof(cwd)) != NULL) {	
            fprintf(stdout, "root@localhost:%s$ ", cwd);
   		}

		line = read_line();
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);
	} while (status);
}

/* function all gets run here */
int main(int argc, char *argv[]) {
    
    /* main loop */
	loop();
    
    exit(EXIT_SUCCESS);
}
