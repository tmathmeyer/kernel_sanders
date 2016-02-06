#include "alloc.h"
#ifdef TESTING
  #include <stdlib.h>
  #include <stdio.h>
#endif

#define MEM_LO 0x8000000
#define MEM_SIZE 0x1FF0

void *mmebrk() {
#ifdef TESTING
    return malloc(MEM_SIZE);
#else
    return (void *)MEM_LO;
#endif
}

blockhdr *link = NULL;

int mm_init(void) {
    void *mem_low = mmebrk();
    blockhdr *lowloc = (blockhdr *)mem_low;
    lowloc->nxpr = MEM_SIZE - sizeof(struct header);
    lowloc->free = 1;
    lowloc->frst = 1;
    lowloc->last = 1;
    link = lowloc;
    
    blockhdr *highloc = (blockhdr *)(mem_low + MEM_SIZE - sizeof(struct header));
    highloc->nxpr = MEM_SIZE - sizeof(struct header);
    highloc->free = 1;
    highloc->frst = 1;
    highloc->last = 1;
}

void split_block(blockhdr *block, size_t size) {
    blockhdr *newtail = ((void *)block) + (size + sizeof(struct header));
    blockhdr *nexthead = ((void *)block) + (size + 2*sizeof(struct header));
    blockhdr *last = ((void *)block) + (block->nxpr);

    size_t sblock_size = block->nxpr - size - 2*sizeof(struct header);
    newtail->free = 1;
    nexthead->free = 1;
    newtail->nxpr = block->nxpr = size + sizeof(struct header);
    last->nxpr = nexthead->nxpr = sblock_size;

    nexthead->last = block->last;
    block->last = 0;
    nexthead->frst = 0;
}

void* mm_alloc(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    
    if (size%8) {
        size = ((size>>3)+1)<<3;
    }
    
    blockhdr *tmp = link;
    while(!tmp->free && tmp->nxpr >= size + sizeof(struct header)) {
        tmp = (blockhdr *)(((void *)tmp) + tmp->nxpr + sizeof(struct header));
    }
    
    split_block(tmp, size);
    tmp->free = 0;
    return ((void *)tmp) + sizeof(struct header);
}

void mergeblocks(blockhdr *data, blockhdr *next) {
    data->last = next->last;
    data->nxpr = (data->nxpr + next->nxpr + sizeof(struct header));
    blockhdr *last = ((void *)next) + next->nxpr;
    last->nxpr = data->nxpr;
    last->frst = data->frst;
}

void mm_free(void* ptr) {
    blockhdr *data = ptr-sizeof(struct header);
    data->free = 1;
    if (!data->last) {
        blockhdr *next = ((void *)data) + data->nxpr + sizeof(struct header);
        if (next->free) {
            mergeblocks(data, next);
        }
    }
    if (!data->frst) {
        blockhdr *prev = ((void *)data) - sizeof(struct header);
        if (prev->free) {
            prev = ((void *)prev) - prev->nxpr;
            mergeblocks(prev, data);
        }
    }
}

void blockcheck() {
#ifdef TESTING
    blockhdr *tmp = link;
    while(1) {
        printf("block @ %i\n", tmp);
        printf("     size: %i\n", tmp->nxpr - sizeof(struct header));
        printf("     free: %s\n", tmp->free?"free":"allocated");
        printf("     last: %s\n", tmp->last?"last":"no");
        printf("     frst: %s\n", tmp->frst?"frst":"no");
        if (!tmp->last) {
            tmp = ((void *)tmp) + tmp->nxpr + sizeof(struct header);
        } else {
            goto done;
        }
    }
done:
    puts("");
#endif
}

void* mm_zalloc(size_t size) {
    void *mem = mm_alloc(size);
    if (!mem) {
        return mem;
    }
    while(size --> 0) {
        ((char *)mem)[size] = 0;
    }
    return mem;
}

size_t mm_copy(void *new, void *old, size_t bytes);

void* mm_realloc(void* ptr, size_t size);
