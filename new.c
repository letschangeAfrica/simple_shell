#include "shell.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/**
 * custom_getline - reads a line of input from the user
 *@lineptr: pointer to a pointer
 *@n: pointer
 *@stream: file
 *Return: -1 failed status otherwise
 */
ssize_t custom_getline(char **lineptr, size_t *n, FILE *stream)
{
static char input_buffer[BUFFER_SIZE];
static int buffer_position;
static int buffer_size;

size_t line_length;
char *line = NULL, current_char;
if (buffer_position >= buffer_size)
{
ssize_t bytesRead = read(fileno(stream), input_buffer, BUFFER_SIZE);
if (bytesRead <= 0)
return (-1);
buffer_position = 0;
buffer_size = bytesRead;
}
line_length = 0;
while (buffer_position < buffer_size)
{
if (line_length + 1 >= *n)
{
*n *= 2;
line = realloc(line, *n + 1);
if (!line)
{
fprintf(stderr, "Error: memory allocation failed\n");
return (-1);
}
}
current_char = input_buffer[buffer_position++];
line[line_length++] = current_char;
if (current_char == '\n')
{
break;
}
}
if (line_length == 0)
{
return (-1);
}
line[line_length] = '\0';
*lineptr = line;
return (line_length);
}
/**
 * execute_commands_from_file - execute command from file
 *@filename: pointer
 */
void execute_commands_from_file(const char *filename)
{
char *tokens[BUFFER_SIZE];
size_t line_size = 0;
ssize_t line_length;
char *line = NULL;
FILE *file = fopen(filename, "r");
if (file == NULL)
{
fprintf(stderr, "%s: Can't open %s\n", argv[0] filename);
return (2);
}
while ((line_length = getline(&line, &line_size, file)) != -1)
{
line[line_length - 1] = '\0';

remove_comments(line);

if (strcmp(line, "alias") == 0)
{
print_aliases();
continue;
}
if (strncmp(line, "alias ", 6) == 0)
{
execute_alias(line + 6);
continue;
}
tokenize_command(line, tokens);
if (!is_command_exists(tokens[0]))
{
printf("Command not found: %s\n", tokens[0]);
continue;
}
execute_command(tokens);
}

free(line);
fclose(file);
}
/**
 * replace_variables - replace variables
 *@command: pointer
 */
void replace_variables(char *command)
{
char *var_pos;
char var[3];
char var_value[10];
int exit_status = 0;
while ((var_pos = strstr(command, "$?")) != NULL)
{
sprintf(var, "%d", WEXITSTATUS(exit_status));
strcpy(var_value, var);
memmove(var_pos + strlen(var_value), var_pos + 2, strlen(var_pos + 2) + 1);
memcpy(var_pos, var_value, strlen(var_value));
}
while ((var_pos = strstr(command, "$$")) != NULL)
{
sprintf(var, "%d", getpid());
strcpy(var_value, var);
memmove(var_pos + strlen(var_value), var_pos + 2, strlen(var_pos + 2) + 1);
memcpy(var_pos, var_value, strlen(var_value));
}
}
/**
 * handle_logical_operators - handle logical operators
 *@command: pointer
 *Return: -1 failed status otherwise
 */
int handle_logical_operators(char *command)
{
char *token;
char *next_token;
int result = 0;
token = strtok(command, "&&");
while (token != NULL)
{
replace_variables(token);
result = system(token);
if (result != 0)
break;

next_token = strtok(NULL, "&&");
if (next_token != NULL)
{
replace_variables(next_token);
result = system(next_token);
if (result == 0)
token = strtok(NULL, "&&");
else
break;
}
else
{
break;
}
}

if (result == 0)
return (1);
else
return (0);
}
/**
* main - central tour
*@argc: integer
*@argv: pointer
*Return: nothing
*/
int main(int argc, char *argv[])
{
int exit_status = 0;
char *command = NULL, *tokens[BUFFER_SIZE];
size_t command_size = 0;
ssize_t line_length;
if (argc > 1)
{
execute_commands_from_file(argv[1]);
return (0);
}
while (1)
{
printf("$ ");
fflush(stdout);
line_length = custom_getline(&command, &command_size, stdin);
if (line_length == -1)
break;
command[line_length - 1] = '\0';
remove_comments(command);
if (is_exit_command(command, &exit_status))
break;
if (is_env_command(command))
print_environment();
else if (is_setenv_command(command))
execute_setenv(command + 7);
else if (is_unsetenv_command(command))
execute_unsetenv(command + 9);
else if (is_cd_command(command))
execute_cd(command + 3);
else if (strcmp(command, "alias") == 0)
print_aliases();
else if (strncmp(command, "alias ", 6) == 0)
execute_alias(command + 6);
else
{
replace_variables(command);
if (handle_logical_operators(command))
continue;
handle_command_separator(command);
tokenize_command(command, tokens);
if (!is_command_exists(tokens[0]))
{
printf("Command not found: %s\n", tokens[0]);
continue;
}
execute_command(tokens);
}
}
free(command);
return (WEXITSTATUS(exit_status));
}
