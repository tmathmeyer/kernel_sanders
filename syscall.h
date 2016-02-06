#ifndef __syscall_h__
#define __syscall_h__

#include "keyboard_map.h"

extern unsigned char* keyboard_map;

static inline int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

static inline int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}

void *__syscall(char *);
#define SYSCALL(x) (__syscall((x)))

#endif // __syscall_h__
