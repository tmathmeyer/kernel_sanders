#include "keyboard_map.h"
#include "syscall.h"
#include "sfs.h"
#include "alloc.h"
#include "sandersio.h"
#include "goodstring.h"

int dvorak(int argc, char* argv[]) {
    keyboard_map = dvorak_keyboard_map;
    return 0;
}

int qwerty(int argc, char* argv[]) {
    keyboard_map = qwerty_keyboard_map;
    return 0;
}

int ls(int argc, char *argv[]) {
    if (argc == 0) {
        sanders_printf("listing for /\n");
    }
    char *name;
    inode *file;
    dmap *r = root();
    map_each(r, name, file) {
        if (file && name && gs_len(name) > 1) {
            sanders_printf("  ");
            sanders_printf(name);
            sanders_printf("  ");
            if (file->type == FUNCTION) {
                sanders_printf("executable");
            }
            if (file->type == _FILE) {
                sanders_printf("file");
            }
            sanders_printf("\n");
        }
    }
}

int touch(int argc, char *argv[]) {
    if (argc == 0) {
        sanders_printf("usage: 'touch [file]*'\n");
    } else {
        inode *res = mm_alloc(sizeof(struct _inode));
        res->type = _FILE;
        map_put(root(), gs_dup(argv[0]), res);
    }
}

void *execute(void *exe) {
    inode *exec = (inode *)map_get(root(), exe);

    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    return NULL;
}


