#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

void handle_comments(char *command);
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
 */

void execute_command(char **args)
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
wait(NULL);
}
else
{
fprintf(stderr, "Error: failed to fork\n");
}
}
/**
 * handle_unsetenv - unsets an environment variable
 *@args: pointer to pointer to a string
 */

void handle_unsetenv(char **args)
{
if (args[1] == NULL)
{
fprintf(stderr, "Usage: unsetenv VARIABLE\n");
return;
}

if (unsetenv(args[1]) == -1)
{
perror("Error unsetting environment variable");
}
}
/**
 * handle_setenv - sets an environment variable
 *@args: pointer to pointer to a string
 */

void handle_setenv(char **args)
{
if (args[1] == NULL || args[2] == NULL)
{
fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
return;
}

if (setenv(args[1], args[2], 1) == -1)
{
perror("Error setting environment variable");
}
}
/**
 * handle_exit - exit
 */

void handle_exit(void)
{
exit(0);
}
/**
 * handle_cd - handle command
 *@args: pointer to a pointer to a string
 * Return: nothing
 */

void handle_cd(char **args)
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
{
perror("Error changing directory");
}
}
/**
 * handle_alias - handle aliases in a command
 *@args: a pointer to pointer to a string
 */

void handle_alias(char **args)
{
char *alias, *token;

if (args[1] == NULL)
{
alias = getenv("ALIAS");

if (alias != NULL)
{
token = strtok(alias, ",");

while (token != NULL)
{
printf("%s\n", token);
token = strtok(NULL, ",");
}
}
}
else
{
while (token != NULL)
{
printf("%s\n", token);
token = strtok(NULL, "=");
}
}
}
/**
 * handle_variables - replaces variables within acommandwithcorrespondingvalues
 *@args: a pointer to a string
 */

void handle_variables(char *args)
{
char *variable = strchr(args, '$');

if (variable != NULL)
{
char *value = getenv(variable + 1);

if (value != NULL)
{
size_t variable_length = strlen(variable);
size_t value_length = strlen(value);
size_t args_length = strlen(args);
char *new_args = malloc(args_length + value_length - variable_length + 1);

if (new_args == NULL)
{
fprintf(stderr, "Memory allocation failed\n");
return;
}

strncpy(new_args, args, variable - args);
strncpy(new_args + (variable - args), value, value_length);
strcpy(new_args + (variable - args) + value_length,
variable + variable_length);

handle_variables(new_args);
free(new_args);
}
}
}
/**
 * handle_comments - rmoves comments from a command
 * @command: a pointer to a string
 */

void handle_comments(char *command)
{
char *comment = strchr(command, '#');

if (comment != NULL)
{
*comment = '\0';
}
}
/**
 * main - central tour
 *
 *Return: nothing
 */
int main(void)
{
char command[BUFFER_SIZE];
char *tokens[BUFFER_SIZE];

while (1)
{
printf("$ ");
fflush(stdout);

if (fgets(command, sizeof(command), stdin) == NULL)
{
break;
}
command[strcspn(command, "\n")] = '\0';
tokenize_command(command, tokens);
execute_command(tokens);
handle_comments(command);
handle_variables(command);

if (tokens[0] == NULL)
{
continue;
}

if (strcmp(tokens[0], "env") == 0)
{
handle_env();
}
else if (strcmp(tokens[0], "unsetenv") == 0)
{
handle_unsetenv(tokens);
}
else if (strcmp(tokens[0], "setenv") == 0)
{
handle_setenv(tokens);
}
else if (strcmp(tokens[0], "exit") == 0)
{
handle_exit();
}
else if (strcmp(tokens[0], "cd") == 0)
{
handle_cd(tokens);
}
else if (strcmp(tokens[0], "alias") == 0)
{
handle_alias(tokens);
}
}

return (0);
}
