#include "shell.h"
void handle_setenv(char **args)
{
if (args[1] == NULL || args[2] == NULL)
{
fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
return;
}
if (setenv(args[1], args[2], 1) == -1)
{
perror("Error settine environment variable");
}
}