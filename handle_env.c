# include "shell.h"

/**
 * handle_env - Handle env
 *
 * Return: nothing
 */

void handle_env(void)
{
char **env_var = environ;

while (*env_var != NULL)
{
printf("%s\n", *env_var);
env_var++;
}
}
