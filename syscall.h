#ifndef __syscall_h__
#define __syscall_h__

#include "keyboard_map.h"

extern unsigned char* keyboard_map;

int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}


#endif // __syscall_h__
