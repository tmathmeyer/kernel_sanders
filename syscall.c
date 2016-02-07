#include "keyboard_map.h"
#include "syscall.h"
#include "sfs.h"
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

int ls(int argc, char *argv[]) {
    if (argc == 0) {
        console_print("listing for /\n");
    }
    char *name;
    inode *file;
    dmap *r = root();
    
    map_each(r, name, file) {
        if (file && name && gs_len(name) > 1) {
            console_print("  ");
            console_print(name);
            console_print("  ");
            if (file->type == FUNCTION) {
                console_print("executable");
            }
            else if (file->type == _FILE) {
                console_print("file");
            }
            console_print("\n");
        }
    }
}

int touch(int argc, char *argv[]) {
    if (argc == 0) {
        console_print("usage: 'touch [file]+'\n");
    } else {
        inode *res = mm_alloc(sizeof(struct _inode));
        res->type = _FILE;
        map_put(root(), argv[0], res);
    }
}

int dog(int argc, char *argv[]) {
    if (argc != 1) {
        console_print("usage: 'dog [file]'\n");
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
        console_print("usage: 'si [file]'\n");
    } else {
        //set_keyboard_handler(&si_keyboard_handler);
    }
}

void *execute(char *exe) {
    inode *exec = (inode *)map_get(root(), exe);

    if (exec->type == FUNCTION) {
        return exec->proc;
    }
    return NULL;
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
