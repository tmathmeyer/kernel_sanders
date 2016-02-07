/*
place to put random string based functions
*/
#ifndef STRINGLIBHEADER
#define STRINGLIBHEADER

#include "alloc.h"

#define ISWHITESPACE(char) (!(char)||(char)==' '||(char)=='\r'||(char)=='\n'||(char) =='\t')
#define ISNUM(char) ((char) >= '0' && (char) <= '9')
#define ISALPHA(char) ( ((char) >= 'a' && (char) <= 'z') || ((char) >= 'A' && (char) <= 'Z') )
#define ISALPHANUM(char) ( ISALPHA((char)) || ISNUM((char)) )
#define ISLOWER(char) ( ((char) >= 'a' && (char) <= 'z') )
#define ISUPPER(char) ( ((char) >= 'A' && (char) <= 'Z') )
#define TOLOWER(char) ( ISUPPER((char)) ? (char) + 32 : (char) )
#define TOUPPER(char) ( ISLOWER((char)) ? (char) - 32 : (char) )



int string_testEqual(const char *one, const char * two);
int string_testEqualCI(const char *one, const char * two);
int string_testEqualN(const char *one, const char * two, const unsigned int n);
int string_testEqualNCI(const char *one, const char * two, const unsigned int n);
//unsigned int string_toVec(const char *s, vec_t *v, const unsigned int maxdem);
unsigned int string_countWords(const char *s);

unsigned int string_len(const char *a);
char * string_copy(char * dest, const char * a);
char * string_cat(char * dest, const char * a);

void * mmemset(void *s, int c, size_t n);

int atoi(const char *a);
int itoa(char *a, const int i);

#endif
