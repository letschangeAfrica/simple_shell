#include "shell.h"
/**
 * check_builtin_commands - Add more built-in commands here if needed
 *
 * Return: nothing
 */
void  handle_cd(char **args)
{
char *directory = NULL;

if (args[1] == NULL)
{
directory = getenv("HOME");
if (directory ==NULL)
{
fprintf(stderr, "Error getting home directory\n");
return;
}
}
else
{
directory = args[1];
}

if (chdir(directory) == -1) {
perror("Error changing directory");
}
}

