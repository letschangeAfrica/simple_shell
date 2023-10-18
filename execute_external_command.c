#include "shell.h"
#include <sys/wait.h>

/**
 * execute_external_command - check if the command exists in the path
 *
 * Return: nothing
 */
void execute_command(const char *command_path, char **args);

void execute_external_command(char **args)
{
char *path = getenv("PATH");
char *path_token = strtok(path, ":");
while (path_token != NULL)
{
char command_path[BUFFER_SIZE];
snprintf(command_path, BUFFER_SIZE, "%s/%s", path_token, args[0]);
if (access(command_path, X_OK) == 0)
{
execute_command(command_path, args);
return;
}
path_token = strtok(NULL, ":");
}
printf("Command nor found: %s\n", args[0]);
}

void execute_command(const char *command_path, char **args)
{
pid_t pid = fork();
if (pid == 0)
{
execv(command_path, args);
fprintf(stderr, "Error: failed to execute command\n");
exit(1);
}
else if (pid > 0)
{
wait(NULL);
}
else
{
fprintf(stderr, "Error: failed to fork\n");
}
}

