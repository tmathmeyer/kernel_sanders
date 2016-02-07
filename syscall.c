#include "syscall.h"
#include "sfs.h"
#include "alloc.h"
#include "sandersio.h"

int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}


void *execute(void *exe) {
    inode *exec = (inode *)map_get(root(), exe);

    if (exec != NULL) {
        sanders_printf("type = %i\n", exec->type);
    }

    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    return NULL;
}


