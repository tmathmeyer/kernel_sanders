#ifndef __syscall_h__
#define __syscall_h__

#include "keyboard_map.h"

extern unsigned char* keyboard_map;

void dvorak(void) {
    keyboard_map = dvorak_keyboard_map;
    return;
}

void qwerty(void) {
    keyboard_map = qwerty_keyboard_map;
    return;
}

#endif // __syscall_h__
