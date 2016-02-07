#include "si.h"

void* fopen(char* filename);
int getflen(char* filename);
int fclose(char* filename);

si_status status;

void si_openfile(char* filename) {
    string_copy(status.filename, filename);
    status.fileaddr = fopen(filename);
    status.filelen = getflen(filename);
    screentext_clear();
    update_cursorxy(0,0);
}

void si_readfile(void) {
    status.buffer = mm_alloc(status.filelen);
    mm_copy(status.buffer, status.fileaddr, status.filelen);
}

void si_displayfile(void) {
    char* nextchar = status.buffer;
    while(1) {
        if (*nextchar == 0) break;
        screentext_writecharnc(*nextchar);
        nextchar++;
    }
    update_cursorxy(0,0);
}

void si_writefile(void) {
    ////////
}

void si_closefile(void) {
    fclose(status.filename);
    mm_free(status.buffer);
}

void si_run(void) {
    while(1) {
        switch(status.mode) {
            case INSERT:
            
            break;
            case EXECUTE:
            
            break;
        }
    }
}
