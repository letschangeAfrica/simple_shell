#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define MAX_ARGS 10

void execute_command_2(char **args);
void handle_exit_2(char **args);

/**
 * main_2 - central tour
 * Return: nothing
 */
int main_2(void)
{
char buffer[BUFFER_SIZE];
char prompt[] = "$ ";
int bytes_read;
char *args[MAX_ARGS];
int arg_count = 0;
int i = 0;
int in_arg = 0;
while (1)
{
printf("%s", prompt);

bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

if (bytes_read == 0)
{
printf("\n");
break;
}

if (bytes_read < 0)
{
perror("Error reading command");
continue;
}

buffer[bytes_read - 1] = '\0';

while (buffer[i] != '\0' && arg_count < MAX_ARGS - 1)
{
if (buffer[i] == ' ')
{
buffer[i] = '\0';
in_arg = 0;
}
else
{
if (!in_arg)
{
args[arg_count] = &buffer[i];
arg_count++;
in_arg = 1;
}
}
i++;
}
args[arg_count] = NULL;

execute_command_2(args);
}

return (0);
}

/**
 * handle_exit_2 - handles the exit command
 *@args: pointer to a pointer to a string
 */

void handle_exit_2(char **args)
{
if (args[1] != NULL)
{
int status = atoi(args[1]);
exit(status);
}
else
{
exit(0);
}
}

