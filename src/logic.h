#ifndef LOGIC_H
#define LOGIC_H

char *read_line(void);

char **split_line(char *line);

int execute(char **args);

#endif
