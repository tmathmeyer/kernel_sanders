#ifndef __syscall_h__
#define __syscall_h__

#include "keyboard_map.h"

extern unsigned char* keyboard_map;


int dvorak(int argc, char* argv[]);
int qwerty(int argc, char* argv[]);

void *__syscall(char *);
#define SYSCALL(x) (__syscall((x)))

#endif // __syscall_h__
