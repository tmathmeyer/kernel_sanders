#ifndef __si_h__
#define __si_h__

#include "screentext.h"
#include "alloc.h"
#include "screentext.h"
#include "stringlib.h"

typedef enum si_mode {
    INSERT,
    EXECUTE
} si_mode;

typedef struct {
    si_mode mode;
    int cursorloc;
    char* filename;
    void* fileaddr;
    int filelen;
    void* buffer;
} si_status;

void si_openfile(char* filename);
void si_readfile(void);
void si_displayfile(void);
void si_writefile(void);
void si_closefile(void);

#endif // __si_h__
