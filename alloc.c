#include "alloc.h"


//static vars:
static void* free_list = NULL;

void *mem_heap_lo() {
    return (void *)0x8000000;
}

void *mem_heap_hi() {
    return (void *)0x28000000;
}



// split free memory so that it can be allocated
void* break_memblock(void *ptr, size_t newsize) {
    int old_free_size = EXTRACT_SIZE(GET_HEADER(ptr)); // get the old free size
    int new_free_size = old_free_size - newsize - DOUBLE_HEADER;

    SET_HEADER(ptr, new_free_size);
    SET_FOOTER(ptr, new_free_size);

    void* p = GET_NEXT_BLOCK(ptr);

    // mark them as free (first bit)
    SET_HEADER(p, (newsize|1));
    SET_FOOTER(p, (newsize|1));

    return p;
}


// move the start of the free lists to ptr
void update_freelist_head(void* ptr) {
    GET_NEXT(ptr) = free_list;
    GET_PREV(ptr) = NULL;

    if (free_list != NULL) {
        GET_PREV(free_list) = ptr;
    }

    free_list = ptr;
}


// delete a node (`rm -rf / --no-preserve-root` it I mean)
void delete(void* ptr) {
    void *next = GET_NEXT(ptr);
    void *prev = GET_PREV(ptr);

    if (prev == NULL) {
        free_list = next;
        if (next != NULL) {
            SET_PREV(next, NULL);
        }
    } else {
        SET_NEXT(prev, next);
        if (next != NULL) {
            SET_PREV(next, prev);
        }
    }
}




// combine adjacent nodes if they are free
// free software is the best
void spread_glorious_GNU_freedom(void* ptr) {
    size_t next_alloc = EXTRACT_FIELDS((char*)(GET_FOOTER(ptr)) + HEADER_SIZE); // the next allocated block's freedom level
    size_t prev_alloc = EXTRACT_FIELDS((char*)(ptr) - DOUBLE_HEADER);           // the previous allocated block's freedom level
    size_t size       = EXTRACT_SIZE(GET_HEADER(ptr));                          // the size of this block

    // both sides are allocated *sigh
    if (prev_alloc && next_alloc)
    {
        update_freelist_head(ptr);
        return;
    } 


    // the next block is free! lets take it!
    if (prev_alloc && !next_alloc)
    {  
        size += EXTRACT_SIZE(GET_HEADER(GET_NEXT_BLOCK(ptr))) + DOUBLE_HEADER;
        delete(GET_NEXT_BLOCK(ptr));
        SET_HEADER(ptr, size);
        SET_FOOTER(ptr, size);

        update_freelist_head(ptr);
        return;
    } 


    // the previous block is free! lets take it!
    if (!prev_alloc && next_alloc)
    {
        ptr = GET_PREVIOUS_BLOCK(ptr);
        size += EXTRACT_SIZE(GET_HEADER(ptr)) + DOUBLE_HEADER;
        SET_HEADER(ptr, size);
        SET_FOOTER(ptr, size);
        return;
    } 



    // all the blocks are free!
    // this is tricky... take the previous block and expand it over this block
    // and the next one.
    void* prev = GET_PREVIOUS_BLOCK(ptr);
    void* next = GET_NEXT_BLOCK(ptr);       
    size += EXTRACT_SIZE(GET_HEADER(prev)) + DOUBLE_HEADER; // add size of previous block
    size += EXTRACT_SIZE(GET_HEADER(next)) + DOUBLE_HEADER; // add size of next block
    SET_HEADER(prev, size);
    SET_FOOTER(prev, size); 
    delete(next);
}






// init the memory manager:
//   0 - there were no errors
//  -1 - your *fake* kernel hates you
int mm_init(void) {
    void* heap_bottom = mem_heap_lo();
    free_list = NULL;

    REREFERENCE(heap_bottom, 1);
    REREFERENCE(heap_bottom + HEADER_SIZE, 1);

    return 0;
}






void* alloc(size_t size) {  
    if (size <= 0) {
        return NULL;
    }

    void *temp_list            = free_list;
    unsigned int limit_loops   = 0;
    unsigned int alloc_size    = (size>MIN_ALLOC_SIZE)?
        ALIGN(size):
        MIN_ALLOC_SIZE;


    // search free list for first free block for size newsize
    while (temp_list != NULL &&   (limit_loops++ < MAX_NUM_LOOPS))
    {
        // the size of the first free chunk of memory
        unsigned int free_size = EXTRACT_SIZE(GET_HEADER(temp_list));

        // this node has enough space
        if (free_size >= alloc_size)
        {
            // there's so much room, we can add a new tail/header
            // and it will still fit within the free space
            if (free_size >= alloc_size + 32)
            {
                return break_memblock(temp_list, alloc_size); 
            }

            // uugh, guess we have to convert our whole space to an occupied node
            // unlink it from the list, and then return the new spot
            delete(temp_list);
            SET_HEADER(temp_list, (free_size|1));
            SET_FOOTER(temp_list, (free_size|1));

            return temp_list;
        }
        else
        {
            temp_list = GET_NEXT(temp_list);
        }
    }
    return NULL;
}




// free the memory
void mm_free(void* ptr) {
    // you cant free that!
    if(ptr == 0) {
        return;
    }


    size_t size = EXTRACT_SIZE(GET_HEADER(ptr));

    SET_HEADER(ptr, size);
    SET_FOOTER(ptr, size);

    if(free_list != NULL)
    {
        // add our new freedom to all of our old freedoms
        // then worship stallman
        spread_glorious_GNU_freedom(ptr);
    }
    else
    {
        // make this our first freedom
        // then worship stallman
        update_freelist_head(ptr);
    }
}

size_t memcopy(void *new, void *old, size_t bytes) {
    char *n = (char *)new;
    char *o = (char *)old;
    while(bytes --> 0) {
        n[bytes] = o[bytes];
    }
    return bytes;
}

// expand memory
void* mm_realloc(void* ptr, size_t size)
{
    // they want to delete their memory??
    // they should have used free, so we'll do it
    // for them... FOR NARNIA
    if (!size)
    {
        mm_free(ptr);
        return 0;
    }

    // they never had anything there before... 
    // just call malloc (which they should have done anyways)
    if (ptr == NULL)
    {
        return alloc(size);
    }

    // attempt to get new memory
    // obviously there is a better way to do this
    // but I'm REALLY tired ( and I discussed that way in my readme )
    void* newptr = alloc(size);

    // the new malloc fails! oh well, we should probably tell they used they failed...
    // their old pointer will work, but the new one will be borked
    if (!newptr)
    {
        return 0;
    }

    // Copy the old data
    size_t oldsize = EXTRACT_SIZE(GET_HEADER(ptr));

    // they want less memory than they do now.....
    // weirdos
    if (size < oldsize)
    {
        oldsize = size;
    }

    // copy their data (yeah this is the slow part)
    memcopy(newptr, ptr, oldsize);

    // free the old memory
    mm_free(ptr);

    return newptr;
}
