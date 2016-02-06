#ifndef SCREENTEXTHEADER
#define SCREENTEXTHEADER

int screen_cursorx;
int screen_cursory;

void screentext_clear(void);
void screentext_writecharplace(char c, int x, int y);
void screentext_writechar(char c);
void screentext_newline(void);
void screentext_backspace(void);

void screentext_print(char * c);

int console_init(void);
void console_clear(void);
int console_print(char *s);


#endif
