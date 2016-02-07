#include "keyboard_map.h"
#include "syscall.h"
#include "sfs.h"
#include "alloc.h"
#include "sandersio.h"
#include "goodstring.h"
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
