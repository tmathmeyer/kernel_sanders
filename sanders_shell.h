// A function pointer
#ifndef SHELL_HEADER
#define SHELL_HEADER

typedef int (*shell_func)(char *args);

shell_func shell_command_lookup(char *cmd);

void shell_run(char *line);



#endif // SHELL_HEADER
