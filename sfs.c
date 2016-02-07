#include "sfs.h"
#include "alloc.h"
#include "map.h"
#include "goodstring.h"
#include "syscall.h"
#include "sandersio.h"

inode *root_fs;

int fs_init() {
    root_fs = mm_alloc(sizeof(struct _inode));
    if (!root_fs) {
        return 1;
    }
    root_fs->type = DIRECTORY;
    root_fs->_dir_sub = map_new();
    
    if (!root_fs->_dir_sub) {
        return 1;
    }

    mkdir(root_fs, "usr");
    inode *executables = mkdir(root_fs, "bin");
    process(executables, "dvorak", dvorak);
    return 0;
}

inode *mkdir(inode *parent, char *name) {
    if (!parent) {
        return NULL;
    }
    if (parent->type != DIRECTORY) {
        return NULL;
    }

    inode *newdir = mm_alloc(sizeof(struct _inode));
    newdir->name = gs_dup(name);
    newdir->type = DIRECTORY;
    newdir->_dir_sub = map_new();
    map_put(parent->_dir_sub, name, newdir);
    return newdir;
}

inode *process(inode *parent, char *name, FS_PROC fn) {
    if (!parent) {
        return NULL;
    }
    if (parent->type != DIRECTORY) {
        return NULL;
    }

    inode *newdir = mm_alloc(sizeof(struct _inode));
    newdir->name = gs_dup(name);
    newdir->proc = fn;
    newdir->type = FUNCTION;
    map_put(parent->_dir_sub, name, newdir);
    return newdir;
}

inode *descend(inode *parent, char *name) {
    if (!parent) {
        sanders_printf("parent is null\n");
        return NULL;
    }
    if (parent->type != DIRECTORY) {
        return NULL;
    }
    return (inode *)map_get(parent->_dir_sub, name);
}

void ls(inode *dir) {
    if (dir->type != DIRECTORY) {
        sanders_printf("that is not a directory\n");
    } else {
        char *keys;
        void *inodes;
        for(unsigned int i=0;i<(dir->_dir_sub)->size;i++) {
            kvpl *iter = ((dir->_dir_sub)->body)[i];
            if (iter == NULL) {
                sanders_printf("NULL\n");
            } else {
                sanders_printf("NOT NULL\n");
            }
        }
    }
}

inode *root() {
    return root_fs;
}
