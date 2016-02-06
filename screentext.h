#ifndef SCREENTEXTHEADER
#define SCREENTEXTHEADER

int screen_cursorx;
int screen_cursory;

void screentext_clear(void);
void screentext_writecharplace(char c, int x, int y);
void screentext_writechar(char c);
void screentext_newline(void);
void screentext_backspace(void);

// Accepts a null terminated character array and returns the number of characters processed
void screentext_print(char * c);


#endif
