#include "shell.h"
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
