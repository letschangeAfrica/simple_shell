#include "shell.h"
/**
 * read_input - Store the standard input in the buffer
 *
 * Return: 0 if there is more to read, -1 if there was an error in the input, 1 * otherwise.
 */
int read_input(void)
{
if (input_buffer_index == 0)
{
int bytes_read = read(STDIN_FILENO, input_buffer, BUFFER_SIZE);
if (bytes_read == 0)
{
return (0);
}

if (bytes_read < 0)
{
perror("Error reading command");
return (-1);
}
}

return (1);
}

/**
 * *find_newline - searches for the first occurnce of a newline in the input bu * ffer starting from the current index
 * Return: a pointer to the newline character if found or NULL if a newline cha * racter is not found
 */
char *find_newline(void)
{
char *newline = memchr(input_buffer + input_buffer_index, '\n', BUFFER_SIZE - input_buffer_index);

return (newline);
}

/**
 * *extract_line - extracts a line from the input buffer up to the newline char * acter, allocates memory for the line, copies the characters from the input b * uff 
 * er to the line, adds a null terminator at the end.
 * Return: a pointer to the extracted line.
 */

char *extract_line(char *newline)
{
int line_end_index = newline - input_buffer + input_buffer_index;
int line_size = line_end_index - input_buffer_index;

char *line = malloc(line_size + 1);
if (line == NULL)
{
perror("Error allocating memory");
return (NULL);
}

memcpy(line, input_buffer + input_buffer_index, line_size);
line[line_size] = '\0';

input_buffer_index = line_end_index + 1;

return (line);
}

/**
 * *custom_getline - read input from the standard input 
 * Return: a line of text as a dynamically allocated string
 * 
 */

char *custom_getline(void)
{
char *line = NULL;
int line_length = 0;
char *newline, *line_realloc;
int result, remaining_bytes;
while (1)
{
result = read_input();

if (result == 0)
{
return (NULL);
}
if (result < 0)
{
return (NULL);
}

newline = find_newline();
{
if (newline != NULL)
{
line = extract_line(newline);
return (line);
}

remaining_bytes = BUFFER_SIZE - input_buffer_index;
line_realloc = realloc(line, line_length + remaining_bytes + 1);
if (line_realloc == NULL)
{
perror("Error allocating memory");
return (NULL);
}
line = line_realloc;

memcpy(line + line_length, input_buffer + input_buffer_index, remaining_bytes);
line_length += remaining_bytes;

input_buffer_index = 0;
}
}
}

