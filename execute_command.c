#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>

/**
 * execute_command_2 - execute command
 *
 * Return: nothing
 */
void execute_command_2(char **args)
{
pid_t pid = fork();

if (pid < 0)
{
perror("Error forking process");
return;
}

if (pid == 0)
{
execvp(args[0], args);
perror("Error executing command");
exit(EXIT_FAILURE);
}
else
{
wait(NULL);
}
}

