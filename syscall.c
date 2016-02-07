#include "keyboard_map.h"
#include "syscall.h"
#include "sfs.h"
#include "alloc.h"
#include "sandersio.h"
#include "goodstring.h"
#include "screentext.h"
#include "sandersboard.h"

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
        sanders_printf("usage: 'touch [file]+'\n");
    } else {
        inode *res = mm_alloc(sizeof(struct _inode));
        res->type = _FILE;
        map_put(root(), gs_dup(argv[0]), res);
    }
}

int dog(int argc, char *argv[]) {
    if (argc != 1) {
        sanders_printf("usage: 'dog [file]'\n");
    } else {
        inode *in = map_get(root(), argv[0]);
        if (in->type == _FILE) {
            if (in->contents != NULL) {
                sanders_print(in->contents);
            }
        }
    }
}

void si_keyboard_handler(char keycode) {

}

int si(int argc, char *argv[]) {
    if (argc != 1) {
        sanders_printf("usage: 'si [file]'\n");
    } else {
        //set_keyboard_handler(&si_keyboard_handler);
    }
}

void *execute(void *exe) {
    inode *exec = (inode *)map_get(root(), exe);

    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    return NULL;
}


