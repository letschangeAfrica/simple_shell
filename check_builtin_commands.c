#include "shell.h"
/**
 * handle_cd - handle command
 *@args: pointer to a pointer to a string
 * Return: nothing
 */
void  handle_cd(char **args)
{
char *directory = NULL;

if (args[1] == NULL)
{
directory = getenv("HOME");
if (directory == NULL)
{
fprintf(stderr, "Error getting home directory\n");
return;
}
}
else
{
directory = args[1];
}

if (chdir(directory) == -1)
perror("Error changing directory");
}

