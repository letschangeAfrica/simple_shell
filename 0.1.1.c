#include "shell.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * tokenize_command - tokenizes a command string into individual tokens
 *@command: a pointer to a string
 *@tokens: a pointer to a pointer to a string
 */

void tokenize_command(char *command, char **tokens)
{
int token_count = 0;
char *token = strtok(command, " ");

while (token != NULL && token_count < BUFFER_SIZE - 1)
{
tokens[token_count] = token;
token_count++;
token = strtok(NULL, " ");
}

tokens[token_count] = NULL;
}
/**
 * execute_command - executes a command b forking a new process
 *@args: pointer to a pointer
 *Return: -1 failed status otherwise
 */

int execute_command(char **args)
{
pid_t pid = fork();

if (pid == 0)
{
execvp(args[0], args);
fprintf(stderr, "Error: failed to execute command\n");
exit(1);
}
else if (pid > 0)
{
int status;
waitpid(pid, &status, 0);
return (status);
wait(NULL);
}
else
{
fprintf(stderr, "Error: failed to fork\n");
return (-1);
}
}
/**
 * is_command_exists - handle commands
 *@command:  a pointer
 *Return: 0 if success
 */


int is_command_exists(char *command)
{
char *path = getenv("PATH");
char *path_copy = strdup(path);
char *path_token = strtok(path_copy, ":");
char command_path[BUFFER_SIZE];
while (path_token != NULL)
{
snprintf(command_path, BUFFER_SIZE, "%s/%s", path_token, command);

if (access(command_path, F_OK) == 0)
{
free(path_copy);
return (1);
}
path_token = strtok(NULL, ":");
}

free(path_copy);
return (0);
}
/**
 * is_cd_command - is this cd command
 *@command: pointer
 *Return: -1 failed status otherwise
 */
bool is_cd_command(char *command)
{
return (strncmp(command, "cd", 2) == 0);
}
/**
 * execute_cd - executes the cd command by extracting the dirctory path
 *@command: pointer
 *Return: -1 failed status otherwise
 */
void execute_cd(char *command)
{
char *directory = strtok(command, " ");
char current_directory[MAX_PATH_LENGTH];
if (directory == NULL)
{
directory = getenv("HOME");
if (directory == NULL)
{
fprintf(stderr, "cd: Failed to retrieve home directory\n");
return;
}
}
else if (strcmp(directory, "-") == 0)
{
directory = getenv("OLDPWD");
if (directory == NULL)
{
fprintf(stderr, "cd: Failed to retrieve previous directory\n");
return;
}
}
if (getcwd(current_directory, MAX_PATH_LENGTH) == NULL)
{
fprintf(stderr, "cd: Failed to retrieve current directory\n");
return;
}
if (chdir(directory) != 0)
{
fprintf(stderr, "cd: Failed to change directory to %s\n", directory);
}
if (setenv("OLDPWD", current_directory, 1) != 0)
{
fprintf(stderr, "cd: Failed to set OLDPWD environment variable\n");
return;
}
if (setenv("PWD", directory, 1) != 0)
{
fprintf(stderr, "cd: Failed to set PWD environment variable\n");
return;
}
}
