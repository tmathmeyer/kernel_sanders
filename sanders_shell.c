#include "sanders_shell.h"
#include "sandersio.h"
#include "syscall.h"
#include "goodstring.h"
#include "halt.h"
#include "video.h"
#include "screentext.h"
#include "sandersboard.h"

void shell_keyboard_handler(char keycode) {
	if(keycode < 0)
        return;

    if(keycode == ENTER_KEY_CODE) {
        console_print("\n");
        sandersin[sandersindex] = 0;
        sandersindex = 0;
        shell_run((char*)sandersin);
        return;
    }
    if(keycode == BACKSPACE_KEY_CODE) {
        if (sandersindex > 0) {
            sandersin[sandersindex--] = 0;
            console_print("\b");
        }
        return;
    }
    
    if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
    		// Caught ctrl+c\n
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
    		// Caught ctrl+z\n
    	}
    	return;
    }
    
    char ascii_key = keyboard_map[(unsigned char) keycode];
    if (ascii_key) {
	    if (key_status[SHIFT_KEY_CODE] == 1) {
	    	if (ascii_key >= 'a' && ascii_key <= 'z') {
	    		ascii_key -= 32;
	    	} else if (ascii_key == '-') {
	    		ascii_key = '_';
	    	}
    	}
	    console_writechar(ascii_key);
        sandersin[sandersindex++] = ascii_key;
    }
}

void shell_run(char *line) {
	char *cmd;
	char *args;

	int argc = 0;
	FS_PROC func;

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

	func = execute(cmd);
	if(!func){
        console_print(cmd);
		console_print(" is not a valid command\n");
	} else {
		func(argc, argv);
	}
    console_print("> ");
}

//NOOOO
int invalid_command(int argc, char *argv[]) {
	return 0;
}
