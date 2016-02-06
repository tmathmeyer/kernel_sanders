#include "alloc.h"
#include "screentext.h"
#include "stringlib.h"


int screen_cursorx = 0;
int screen_cursory = 0;


int vid_lines = 25;
int vid_col = 80;
int vid_bpc = 2;

char * vidptr = (char*) 0xb8000;

char * screen_outbuffer[25];
int screen_outbufferx = 0;
int screen_outbuffery = 0;

static inline void outb(unsigned short port, unsigned char val){
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

void update_cursor(){
	unsigned short position=(screen_cursory * vid_col + screen_cursorx);
	// cursor LOW port to vga INDEX register
	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char)(position&0xFF));
	// cursor HIGH port to vga INDEX register
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char )((position>>8)&0xFF));
}


void screentext_clear(void){
	unsigned int ss = vid_lines * vid_col;
	unsigned int i;
	for(i = 0; i < ss; i++){
		((unsigned short *) vidptr)[i] = 0x0720;
	}
	screen_cursorx = 0;
	screen_cursory = 0;
	update_cursor();
}

void screentext_newline(void){
	screen_cursorx = 0;
	screen_cursory = (screen_cursory+1) % vid_lines;
	update_cursor();
}

void screentext_backspace(void){
	screen_cursorx--;
	if(screen_cursorx < 0){
		screen_cursorx = vid_col-1;
		screen_cursory = screen_cursory ? screen_cursory-1 : 0;
	}
	screentext_writecharplace(' ', screen_cursorx, screen_cursory);
	update_cursor();
}

void screentext_writecharplace(char c, int x, int y){
	vidptr[(y * vid_col + x)*vid_bpc] = c;
	vidptr[(y * vid_col + x)*vid_bpc+1] = 0x07;
	update_cursor();
}
void screentext_writecharplacenc(char c, int x, int y){
	vidptr[(y * vid_col + x)*vid_bpc] = c;
	vidptr[(y * vid_col + x)*vid_bpc+1] = 0x07;
}
void screentext_writechar(char c){
	screentext_writecharplacenc(c, screen_cursorx, screen_cursory);
	screen_cursorx ++;
	screen_cursory += screen_cursorx/vid_col;
	screen_cursorx %= vid_col;
	screen_cursory %= vid_lines;
	update_cursor();
}
void screentext_writecharnc(char c){
	screentext_writecharplacenc(c, screen_cursorx, screen_cursory);
	screen_cursorx ++;
	screen_cursory += screen_cursorx/vid_col;
	screen_cursorx %= vid_col;
	screen_cursory %= vid_lines;
}

void screentext_print(char * c){
	for(;*c;c++){
		if(*c == '\n') screentext_newline();
		else screentext_writecharnc(*c);
	}
	update_cursor();
}




int console_init(void){
	int i;
	for(i = 0; i < vid_lines; i++){
		screen_outbuffer[i] = mm_alloc(vid_col + 1);
		mmemset(screen_outbuffer[i], 0, vid_col+1);
	}
	return i;
}
int console_screendraw(void){
	//slide through the text
	int y;
	for(y = 0; y < vid_lines; y++){
		int cplace = (y + screen_outbuffery + 1) % vid_lines;
		int x;
		char * boof = screen_outbuffer[cplace];
		for( x = 0; boof[x]; x++){
			screentext_writecharplacenc(boof[x], x, y);
		}
	}
	return y;
}
int console_screendrawrange(int mx, int x){
	return 0;
}
int console_print(char *s){
	int needsupdate = 0;
	for(; *s; s++){
		screen_outbuffer[screen_outbuffery][screen_outbufferx] = *s;
		if(*s != '\n')screen_outbufferx++;
		if(screen_outbufferx >= vid_col || *s == '\n'){
			screen_outbuffer[screen_outbuffery][screen_outbufferx] = 0;
			screen_outbufferx = 0;
			screen_outbuffery++;
			screen_outbuffery = (screen_outbuffery %vid_lines);
			needsupdate = 1;
		}

	}
	screen_outbuffer[screen_outbuffery][screen_outbufferx] = 0;
	if(needsupdate) console_screendraw();
	else console_screendraw(); // todo
	return 1;
}
