#ifndef SHELL_HEADER
#define SHELL_HEADER

#include "sfs.h"

FS_PROC shell_command_lookup(char *cmd);
extern unsigned char sandersin[256];
extern unsigned char sandersindex;

// A function pointer
typedef int (*shell_func)(int argc, char *argv[]);

void shell_keyboard_handler(char keycode);

shell_func shell_command_lookup(char *cmd);

void shell_run(char *line);

int invalid_command(int argc, char *argv[]);


#endif // SHELL_HEADER
