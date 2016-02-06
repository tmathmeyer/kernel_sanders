#ifndef FONTHEADER
#define FONTHEADER
typedef struct glyph_s {
	char name;
	unsigned int width;
	unsigned int height;
	unsigned char *data;
} glyph_t;

typedef struct font_s {
	char * name;
	glyph_t glyphs[256];
} font_t;


#endif
