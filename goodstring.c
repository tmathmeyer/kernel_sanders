#include "goodstring.h"
#include "alloc.h"

int gs_comp(char *X, char *Y) {
    while(1) {
        if (*X == *Y) {
            if (*X) {
                X++;
                Y++;
            } else {
                return 0;
            }
        } else {
            return *X - *Y;
        }
    }
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
