#include "dog.h"

void* fopen(char* fpath);
int filelen(char* fpath);
void fclose(char* fpath);

void dog_openfile(char* file) {
    int i = 0;
    void* memaddr = fopen(file);
    int flen = filelen(file);
    char* buffer = mm_alloc(flen);
    mm_copy(buffer, memaddr, flen);
    fclose(file);

    console_writechar('\n');
    while(1) {
        if (buffer[i] == 0)  break;
        console_writechar(buffer[i]);
        i++;
    }
}

