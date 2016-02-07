#include "screentext.h"
#include "sandersboard.h"
#include "syscall.h"

void draw_board(int position) {
    screentext_clear();
    int i;
    for(i=0;i<20;i++) {
        screentext_writecharplace('#', 10, i);
        screentext_writecharplace('#', 21, i);
    }

    for(i=0;i<32;i++) {
        screentext_writecharplace('#', i, 6);
        screentext_writecharplace('#', i, 13);
    }

    switch(position) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            for(i=14;i<21;i++) {
                screentext_writecharplace('$', i, 13);
            }
            for(i=13;i<20;i++) {
                screentext_writecharplace('$', 21, i);
                screentext_writecharplace('$', 10, i);
            }
            break;
        case 9:
            for(i=21;i<32;i++) {
                screentext_writecharplace('$', i, 13);
            }
            for(i=13;i<20;i++) {
                screentext_writecharplace('$', 21, i);
            }
            break;
    }
}

int position = 9;
void ttt_keyboard_handler(char keycode) {
	if (keycode <= 0) {
		return;
	}

	if (key_status[CTRL_KEY_CODE] == 1) {
  		if (keyboard_map[(unsigned char) keycode] == 'c') {
            screentext_clear();
            init_keyboard();
    	}
    	if (keyboard_map[(unsigned char) keycode] == 'z') {
            screentext_clear();
            init_keyboard();
    	}
    	return;
    }

    switch(keycode) {
    	case UP_KEY_CODE:
            switch(position) {
                case 1: case 2: case 3: break;
                default:
                                        position = position - 3;
                                        break;
            }
    		return;
    	case DOWN_KEY_CODE:
            switch(position) {
                case 7: case 8: case 9: break;
                default:
                                        position = position + 3;
                                        break;
            }
    		return;
    	case LEFT_KEY_CODE:
            switch(position) {
                case 1: case 4: case 7: break;
                default:
                                        position = position - 1;
                                        break;
            }
    		return;
    	case RIGHT_KEY_CODE:
            switch(position) {
                case 3: case 6: case 9: break;
                default:
                                        position = position + 1;
                                        break;
            }
    		return;
        default:
            (void) "fuck";
            int ascii_key = keyboard_map[(unsigned char) keycode];
            screentext_writecharplace(ascii_key, 1, 1);
            

    }
    draw_board(8);
}




int bs(int argc, char *argv[]) {
	set_keyboard_handler(&ttt_keyboard_handler);
    while(1) {
        draw_board(position);
        for(int i=0;i<1000000;i++);
    }
}



