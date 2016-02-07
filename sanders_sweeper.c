#include "sanders_sweeper.h"
#include "sandersboard.h"
#include "syscall.h"
#include "screentext.h"


int sanderssweeper(int argc, char *argv[]) {
	set_keyboard_handler(&sanders_sweeper_keyboard_handler);
	screentext_clear();

	return 0;
}

void sanders_sweeper_keyboard_handler(char keycode) {
	if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
  			sanders_sweeper_exit();
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
  			sanders_sweeper_exit();
    	}
    	return;
    }
}

int sanders_sweeper_exit() {
	set_default_keyboard_handler();
	return 0;
}

