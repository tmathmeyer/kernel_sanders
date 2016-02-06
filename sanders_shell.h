// A function pointer
#ifndef SHELL_HEADER
#define SHELL_HEADER

typedef int (*shell_func)(int argc, unsigned char *argv[]);

shell_func shell_command_lookup(unsigned char *cmd);

void shell_run(unsigned char *line);

int invalid_command(int argc, unsigned char *argv[]);


#endif // SHELL_HEADER
