#ifndef _SHELL_H
#define _SHELL_H
extern char **environ;
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_SIZE 1024
#define MAX_ARGS 10
#define MAX_PATH_LENGTH 4096
#define MAX_ALIASES 100
/**
 * struct  - handle alias
 *@name: pointer
 *@value: pointer
 */
typedef struct
{
char *name;
char *value;
} Alias;

/**
 * main - Create a simple shell
 *@argc: integer
 *@argv: pointer
 * Return:0 if SUCCESS
 *
 */
int main(int argc, char *argv[]);
void tokenize_command(char *command, char **tokens);
int execute_command(char **args);
void print_environment(void);
int is_command_exists(char *command);
bool is_exit_command(char *command, int *status);
void execute_cd(char *command);
bool is_setenv_command(char *command);
int is_env_command(char *command);
bool is_cd_command(char *command);
bool is_unsetenv_command(char *command);
void add_alias(char *name, char *value);
void print_aliases(void);
void remove_comments(char *command);
void handle_command_separator(char *command);
void execute_alias(char *command);
void execute_setenv(char *command);
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream);
void execute_unsetenv(char *command);
void execute_commands_from_file(const char *filename);
void replace_variables(char *command);
int handle_logical_operators(char *command);


#endif

