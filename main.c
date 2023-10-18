#include "shell.h"
#include <sys/types.h>
#include <sys/wait.h>

/**
 * main - central tour
 * no parameters
 * Return: 0 if SUCCESS
 */
void tokenize_command(char *command, char **tokens);
void handle_comments(char *command);
void handle_cd(char **args);
void handle_alias(char **args);
void handle_variables(char *args);
void execute_file(char *filename);
void execute_command(char** tokens);

int main(int argc, char *argv[])
{
char buffer[BUFFER_SIZE];
char prompt[] = "$ ";
int bytes_read, j;
char *args[BUFFER_SIZE];
int command_count = 0;
char *commands[BUFFER_SIZE];
if (argc > 1)
{
execute_file(argv[1]);
return (0);
}
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
tokenize_command(buffer, commands);
for ( j = 0; j < command_count; j++)
{
handle_comments(commands[j]);
handle_variables(commands[j]);
tokenize_input(buffer, args);
if (strcmp(args[0], "exit") == 0)
{
handle_exit();
break;
}
if (strcmp(args[0], "setenv") == 0)
{
handle_setenv(args);
continue;
}
if (strcmp(args[0], "unsetenv") == 0)
{
handle_unsetenv(args);
continue;
}
if (strcmp(args[0], "cd") == 0)
{
handle_cd(args);
continue;
}
if (strcmp(args[0], "alias") == 0)
{
handle_alias(args);
continue;
}
if (strcmp(args[0], "env") == 0)
{
handle_env();
continue;
}
execute_external_command(args);
}
}
return (0);
}

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
void execute_command(char** tokens) {
pid_t pid = fork();
if (pid < 0) {
perror("Error forking process");
return;
}

if (pid == 0) {
execvp(tokens[0], tokens);
perror("Error executing command");
exit(EXIT_FAILURE);
}
else
{

wait(NULL);
}
}
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
void handle_variables(char *args) {
char *variable = strchr(args, '$');
if (variable != NULL) {
char *value = getenv(variable + 1);
if (value != NULL) {
size_t variable_length = strlen(variable);
size_t value_length = strlen(value);
size_t args_length = strlen(args);
char *new_args = malloc(args_length + value_length - variable_length + 1);
if (new_args == NULL) {
fprintf(stderr, "Memory allocation failed\n");
return;
}
strncpy(new_args, args, variable - args);
strncpy(new_args + (variable - args), value, value_length);
strcpy(new_args + (variable - args) + value_length, variable + variable_length);


handle_variables(new_args);
free(new_args);
}
}

}

void handle_comments(char *command)
{
char *comment = strchr(command, '#');

if (comment != NULL)
{
*comment = '\0';
}
}

void execute_file(char *filename)
{
FILE *file = fopen(filename, "r");
char *commands[BUFFER_SIZE];
int command_count = 0, j;
char line[BUFFER_SIZE];
char *args[BUFFER_SIZE];
if (file == NULL)
{
perror("Error opening file");
return;
}

while (fgets(line, BUFFER_SIZE, file) != NULL)
{
line[strcspn(line, "\n")] = '\0';
tokenize_command(line, commands);

for ( j = 0; j < command_count; j++)
{
handle_comments(commands[j]);
handle_variables(commands[j]);

tokenize_command(commands[j], args);

if (strcmp(args[0], "setenv") == 0)
{
handle_setenv(args);
continue;
}

if (strcmp(args[0], "cd") == 0)
{
handle_cd(args);
continue;
}


if (strcmp(args[0], "alias") == 0)
{
handle_alias(args);
continue;
}

execute_command(args);
}
}

fclose(file);
}
