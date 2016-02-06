#include "sanders_shell.h"

void shell_run(char *line) {
	char *cmd;
	char *args;

	int argc = 0;
	shell_func func;

	cmd = line;

	char *head = line;
	while (*head != ' ' && *head) {
		head++;
	}
	
	// Separate arguments and function name with null
	*head = 0;
	head++;
	args = head;

	// Get argc
	while (*head) {
		// Skip past spaces
		while (*head == ' ') {
			head++;
		}
		// Beginning of argument
		if (*head) {
			argc++;
			// Go to end of argument
			while (*head != ' ' && *head) {
				head++;
			}
		}		
		head++;
	}

	// Set argv
	char *argv[argc];
	argc = 0;
	// Set head back to start
	head = args;
	while (*head) {
		// Skip past spaces and set to null
		while (*head == ' ') {
				*head = 0;
				head++;
		}
		// Beginning of argument
		if (*head) {
			argv[argc] = head;
			argc++;
			// Go to end of argument
			while (*head != ' ' && *head) {
				head++;
			}
		}
	}

	func = shell_command_lookup(cmd);



	func(argc, argv);
}


shell_func shell_command_lookup(char *cmd) {
	return invalid_command;
}

int invalid_command(int argc, char *argv[]) {
	return 0;
}
