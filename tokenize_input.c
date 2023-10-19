#include "shell.h"

/**
 * tokenize_input - description
 *@buffer: a pointer to a string
 *@args: a pointer to a pointer to a string
 * Return: nothing
 */

void tokenize_input(char *buffer, char **args)
{
char *token = strtok(buffer, " ");
int arg_count = 0;
while (token != NULL && arg_count < MAX_ARGS - 1)
{
args[arg_count] = token;
arg_count++;
token = strtok(NULL, " ");
}
args[arg_count] = NULL;
}

