#ifndef PARSE_H
#define PARSE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void trim(char *str);
int parse_input(char *input, char **args);

#endif /* PARSE_H */  