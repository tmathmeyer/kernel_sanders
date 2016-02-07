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


void *execute(void *binary_path_or_static) {
    inode *exec = root();
    
    ls(exec);
    
    /*
    exec = descend(exec, "bin");
    exec = descend(exec, binary_path_or_static);
    
    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    */
    return NULL;
}


