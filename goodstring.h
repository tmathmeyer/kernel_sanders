#ifndef GOODSTRING_H
#define GOODSTRING_H

#include "alloc.h"

int gs_comp(char *X, char *Y);
int gs_ncomp(char *X, char *Y, size_t n);
int gs_len(char *X);
char *gs_dup(char *X);

#endif
