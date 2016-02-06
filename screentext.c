#include "screentext.h"



int screen_cursorx = 0;
int screen_cursory = 0;


int vid_lines = 25;
int vid_col = 80;
int vid_bpc = 2;

char * vidptr = (char*) 0xb8000;


void screentext_clear(void){
	unsigned int ss = vid_lines * vid_col;
	unsigned int i;
	for(i = 0; i < ss; i++){
		((unsigned short *) vidptr)[i] = 0x0720;
	}
}

void screentext_newline(void){
	screen_cursorx = 0;
	screen_cursory = (screen_cursory+1) % vid_lines;
}

void screentext_backspace(void){
	screen_cursorx--;
	if(screen_cursorx < 0){
		screen_cursorx = vid_col-1;
		screen_cursory = screen_cursory ? screen_cursory-1 : 0;
	}
	screentext_writecharplace(' ', screen_cursorx, screen_cursory);
}

void screentext_writecharplace(char c, int x, int y){
	vidptr[(y * vid_col + x)*vid_bpc] = c;
	vidptr[(y * vid_col + x)*vid_bpc+1] = 0x07;
}
void screentext_writechar(char c){
	screentext_writecharplace(c, screen_cursorx, screen_cursory);
	screen_cursorx ++;
	screen_cursory += screen_cursorx/vid_col;
	screen_cursorx %= vid_col;
	screen_cursory %= vid_lines;
}

void screentext_print(char * c){
	for(;*c;c++){
		if(*c == '\n') screentext_newline();
		else screentext_writechar(c);
	}
}
