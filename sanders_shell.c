#include "sanders_shell.h"
#include "sandersio.h"
#include "syscall.h"
#include "goodstring.h"
#include "halt.h"
#include "sandersboard.h"

void shell_keyboard_handler(char keycode) {
	if(keycode < 0)
        return;

    if(keycode == ENTER_KEY_CODE) {
        // console_print("\n");
        sanders_printf("%c", '\n');
        sandersin[sandersindex] = 0;
        sandersindex = 0;
        shell_run((char*)sandersin);
        return;
    }
    if(keycode == BACKSPACE_KEY_CODE) {
        if (sandersindex > 0) {
            sandersin[sandersindex--] = 0;
            // console_writechar('\b');
            sanders_printf("%c", '\b');
        }
        return;
    }
    
    if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
    		// sanders_printf("Caught ctrl+c\n");
    		// Do something
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
    		// sanders_printf("Caught ctrl+z\n");
    		// Do something else
    	}
    	return;
    }
    // console_writechar(keyboard_map[(unsigned char) keycode]);
    // sanders_printf("WHAT?");
    sanders_printf("%c", keyboard_map[(unsigned char) keycode]);
    sandersin[sandersindex++] = keyboard_map[(unsigned char) keycode];
}

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
	if(!func){
		sanders_printf("BAD: %s\n", cmd);
	} else {
		sanders_printf("shelly: %s\n", cmd);
		func(argc, argv);
	}


}


shell_func shell_command_lookup(char *cmd) {
    shell_func call = (shell_func)SYSCALL(cmd);
    return call;
}
//NOOOO
int invalid_command(int argc, char *argv[]) {
	return 0;
}
