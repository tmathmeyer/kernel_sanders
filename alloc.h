#ifndef ALLOC_H
#define ALLOC_H

#include "types.h"

typedef
struct header {
    unsigned int nxpr : 29;
    unsigned char free : 1;
    unsigned char last : 1;
    unsigned char frst : 1;
} blockhdr;

#define WSIZE       4       /* word size (bytes) */  
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define OVERHEAD    8       /* overhead of header and footer (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))  

#define PACK(size, alloc)  ((size) | (alloc))

#define GET(p)       (*(size_t *)(p))
#define PUT(p, val)  (*(size_t *)(p) = (val))  

#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

#define HDRP(bp)       ((char *)(bp) - WSIZE)  
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


int mm_init(void);
void* mm_alloc(size_t size);  
void* mm_zalloc(size_t size);  
void mm_free(void* ptr);
size_t mm_copy(void *new, void *old, size_t bytes);
void* mm_realloc(void* ptr, size_t size);
void blockcheck();
#endif
