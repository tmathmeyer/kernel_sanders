#include "goodstring.h"
#include "alloc.h"

int gs_comp(char *X, char *Y) {
    return gs_ncomp(X, Y, 0);
}

int gs_ncomp(char *X, char *Y, size_t n) {
    int ncomp = n;
    while(*X && *Y && (ncomp?n-->0:1)) {
        if (*X - *Y) {
            return *X - *Y;
        }
        ++X;
        ++Y;
    }
    if (*X || *Y) {
        return *X - *Y;
    }
    return 0;
}

int gs_len(char *X) {
    int i = 0;
    while(i++,*X++);
    return i;
}

char *gs_dup(char *X) {
    int size = gs_len(X);
    char *res = mm_alloc(size);
    char *tmp = res;
    while(*tmp++ = *X++);
    return res;
}
