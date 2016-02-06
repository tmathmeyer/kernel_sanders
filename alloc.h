#ifndef ALLOC_H
#define ALLOC_H

#define NULL 0
#define size_t unsigned int

#define HEADER_SIZE   4       // the size of the header in bytes
#define DOUBLE_HEADER 8       // get it? like baseball :D
#define MIN_ALLOC_SIZE 32     // dont ever allocate less than this... too much hassle

#define MAX_NUM_LOOPS 250     // only check through this many free spaces...
                              // may need to be changed depending on tests?
                              // we will see what is optimal
                              // results:
                              //   100 - 84/100
                              //   150 - 85/100
                              //   200 - 85/100
                              //   250 - 85/100
                              //   300 - 85/100




//Macros:

// pointer things
#define DEREFERENCE(a)          (*(unsigned int *)(a))
#define REREFERENCE(a,b)        (*(unsigned int *)(a) = (b)) 

// extract information from headers/footers
#define EXTRACT_SIZE(a)         (DEREFERENCE(a) & -8) // get all but first three bits
#define EXTRACT_FIELDS(a)       (DEREFERENCE(a) & 1) // get the first bit

// get the headers and footers of memory
#define GET_HEADER(a)           (a - HEADER_SIZE) // get the header of the memory
#define GET_FOOTER(a)           (a + EXTRACT_SIZE(GET_HEADER(a))) // get the footer of the memory

// set the headers and footers to:
#define SET_HEADER(a,b)         (REREFERENCE(GET_HEADER(a), b))
#define SET_FOOTER(a,b)         (REREFERENCE(GET_FOOTER(a), b))

// get the previous and next blocks of memory (their location)
#define GET_NEXT_BLOCK(a)       ((a) + EXTRACT_SIZE(GET_HEADER(a)) + 2*HEADER_SIZE)
#define GET_PREVIOUS_BLOCK(a)   (a - EXTRACT_SIZE(a-DOUBLE_HEADER) - 2*HEADER_SIZE)  

// dereference those locations
#define GET_NEXT(a)             (*(void **)(a + DOUBLE_HEADER))
#define GET_PREV(a)             (*(void **)a)

// change the header/footer pointer locations
#define SET_NEXT(a, b)          (GET_NEXT(a) = b)
#define SET_PREV(a, b)          (GET_PREV(a) = b)

// included macro (but I modified it a bit)
#define ALIGN(p)                (((size_t)(p) + 7) & -8)


int mm_init(void);
void* alloc(size_t size);  
void mm_free(void* ptr);
size_t memcopy(void *new, void *old, size_t bytes);
void* mm_realloc(void* ptr, size_t size);

#endif
