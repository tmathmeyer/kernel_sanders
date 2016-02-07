#include "syscall.h"
#include "sfs.h"
#include "alloc.h"
#include "sandersio.h"

dmap *syscall_map;

static unsigned char qwerty_keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static unsigned char dvorak_keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '[', ']', '\b', /* Backspace */
  '\t',     /* Tab */
  '\'', ',', '.', 'p', /* 19 */
  'y', 'f', 'g', 'c', 'r', 'l', '/', '=', '\n', /* Enter key */
    0,      /* 29   - Control */
  'a', 'o', 'e', 'u', 'i', 'd', 'h', 't', 'n', 's', /* 39 */
 '-', '`',   0,    /* Left shift */
 '\\', ';', 'q', 'j', 'k', 'x', 'b',      /* 49 */
  'm', 'w', 'v', 'z',   0,        /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}


void *execute(void *binary_path_or_static) {
    inode *exec = root();
    exec = descend(exec, "bin");
    exec = descend(exec, binary_path_or_static);
    
    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    return NULL;
}

void *__syscall(char *c) {
    return map_get(syscall_map, c);
}

int syscall_init() {
    syscall_map = map_new();
    map_put(syscall_map, "execute", execute);
    return 1;
}
