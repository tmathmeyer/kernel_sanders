#include "sfs.h"
#include "alloc.h"
#include "map.h"
#include "goodstring.h"
#include "syscall.h"
#include "sandersio.h"
#include "map.h"

void process(dmap *fs, char *name, FS_PROC fn) {
    inode *res = mm_alloc(sizeof(struct _inode));
    res->type = FUNCTION;
    res->proc = fn;
    map_put(fs, gs_dup(name), res);
}


