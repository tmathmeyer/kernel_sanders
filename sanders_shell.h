#ifndef SHELL_HEADER
#define SHELL_HEADER

// A function pointer
typedef int (*shell_func)(int argc, char *argv[]);

shell_func shell_command_lookup(char *cmd);

void shell_run(char *line);

int invalid_command(int argc, char *argv[]);


#endif // SHELL_HEADER
