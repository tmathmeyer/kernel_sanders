#include "sanders_shell.h"

// p_func shell_command_lookup(char *cmd);

void shell_run(char *line) {
	char *p_cmd;
	char *p_args;

	shell_func func;

	p_cmd = line;
	p_args = line;
	
	while (*p_args != ' ' && p_args) {
		p_args++;
	}

	*p_args = '\0';


	func = shell_command_lookup(p_cmd);

	func(p_args);
}


shell_func shell_command_lookup(char *cmd) {
	return '\0';
}


