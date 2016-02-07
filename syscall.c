#include "keyboard_map.h"
#include "syscall.h"
#include "alloc.h"
#include "sandersio.h"
#include "goodstring.h"
#include "screentext.h"
#include "sandersboard.h"
#include "halt.h"
#include "screentext.h"

int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}

#define ECHO(name) { console_print(#name); console_print("\n"); }
int ls(int argc, char *argv[]) {
    ECHO(dvorak);
    ECHO(qwerty);
    ECHO(videorun);
    ECHO(ls);
    ECHO(si);
    ECHO(sanders_sweeper);
}

void si_keyboard_handler(char keycode) {

}

int si(int argc, char *argv[]) {
    if (argc != 1) {
        console_print("usage: 'si [file]'\n");
    } else {
        //set_keyboard_handler(&si_keyboard_handler);
    }
}

int sanders_exit(int argc, char* argv[]) {
	screentext_clear();
	screentext_writecharplace('G', 16, 4);
	screentext_writecharplace('o', 17, 4);
	screentext_writecharplace('o', 18, 4);
	screentext_writecharplace('d', 19, 4);
	screentext_writecharplace('b', 20, 4);
	screentext_writecharplace('y', 21, 4);
	screentext_writecharplace('e', 22, 4);
	screentext_writecharplace('!', 23, 4);
    halt();
    return 0;
}
