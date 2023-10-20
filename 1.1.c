#include "shell.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * is_exit_command - exit command
 *@command: a pointer
 *@status: a pointer
 *Return: nothing
 */

bool is_exit_command(char *command, int *status)
{
if (strncmp(command, "exit", 4) == 0)
{
char *argument = command + 4;
*status = atoi(argument);
return (true);
}
return (false);
}
/**
* print_environment - Handle env
 *
 * Return: nothing
 */

void print_environment(void)
{
char **env = environ;

while (*env)
{
printf("%s\n", *env);
env++;
}
}
/**
 * is_env_command - Handle env
 *@command: pointer
 * Return: nothing
 */
int is_env_command(char *command)
{
return (strcmp(command, "env") == 0);
}
/**
 * is_setenv_command - checks if the given command is an environment command
 *@command: pointer
 *Return: -1 failed status otherwise
 */
bool is_setenv_command(char *command)
{
return (strncmp(command, "setenv", 6) == 0);
}
/**
 * is_unsetenv_command - checks if the given command is an unsetenv command
 *@command: pointer
 *Return: -1 failed status otherwise
 */
bool is_unsetenv_command(char *command)
{
return (strncmp(command, "unsetenv", 8) == 0);
}
