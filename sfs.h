#ifndef SFS_H
#define SFS_H

#include "map.h"


#define _FILE 1
#define FUNCTION 2

typedef int (*FS_PROC)(int argc, char *argv[]);
typedef struct _inode {
    char type;
    union {
        char *contents;
        FS_PROC proc;
    };
} inode;

int fs_init();
dmap *root();
inode *descend(inode *parent, char *name);
void process(dmap *fs, char *name, FS_PROC fn);

#endif
