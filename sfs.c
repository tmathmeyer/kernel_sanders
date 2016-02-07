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
    root_fs->node = NULL;
    
    mkdir(root_fs, "usr");
    inode *executables = mkdir(root_fs, "bin");
    //process(executables, "dvorak", dvorak);
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
    newdir->node = NULL;

    ilist *node = mm_alloc(sizeof(struct _ilist));
    node->name = gs_dup(name);
    node->next = parent->node;
    node->node = newdir;
    parent->node = node;
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
    
    ilist *node = mm_alloc(sizeof(struct _ilist));
    node->name = gs_dup(name);
    node->next = parent->node;
    node->node = newdir;
    parent->node = node;
    
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
    ilist *l = parent->node;
    while(l) {
        if (!gs_comp(l->name, name)) {
            return l->node;
        }
        l = l->next;
    }
    return NULL;
}

void ls(inode *dir) {
    if (dir->type != DIRECTORY) {
        sanders_printf("that is not a directory\n");
    } else {
        ilist *l = dir->node;
        while(l) {
            sanders_printf(l->name);
            sanders_printf("\n");
            l = l->next;
        }
    }
}

inode *root() {
    return root_fs;
}
