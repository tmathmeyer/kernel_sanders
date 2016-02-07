#ifndef SHELL_HEADER
#define SHELL_HEADER

#include "sfs.h"

FS_PROC shell_command_lookup(char *cmd);

void shell_run(char *line);

int invalid_command(int argc, char *argv[]);


#endif // SHELL_HEADER
