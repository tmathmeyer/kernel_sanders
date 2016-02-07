#ifndef SFS_H
#define SFS_H

#include "map.h"


#define DIRECTORY 0
#define FILE 1
#define FUNCTION 2

typedef int (*FS_PROC)(int argc, char *argv[]);
typedef struct _inode {
    char *name;
    char type;
    union {
        dmap *_dir_sub;
        FS_PROC proc;
    };
} inode;

int fs_init();
inode *root();
void ls(inode *dir);
inode *mkdir(inode *parent, char *name);
inode *descend(inode *parent, char *name);
inode *process(inode *parent, char *name, FS_PROC fn);

#endif
