#ifndef SHELL_H
#define SHELL_H
extern char **environ;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define MAX_ARGS 10
/**
 * Simple Shell - Create a simple shell
 *
 * Return:0 if SUCCESS
 *
 */

int main_2(void);
int main(int argc, char *argv[]);
void handle_exit(void);
void handle_env(void);
void execute_external_command(char **args);
void tokenize_input(char *buffer, char **args);
char *custom_getline(void);
int read_input(void);
char *find_newline(void);
char *extract_line(char *newline);
void handle_unsetenv(char **args);
void handle_setenv(char **args);
void handle_cd(char **args);

#endif

