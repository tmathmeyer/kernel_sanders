
#include "the_iowa_caucus.h"

#include "alloc.h"
#include "sand_rand.h"
#include "halt.h"
#include "syscall.h"
#include "video.h"

extern unsigned char *vidmem;
char *pixels_back;

int randdir(int x, int y) {
    unsigned int i = sand_rand();
    int k[5] = {0};
    int ct = 0;

    if (x==0 && y==0) { // top left
        k[2] = 2;
        k[3] = 3;
        ct = 2;
    } else if (x==VIDEO_WIDTH-1 && y==VIDEO_HEIGHT-1) { // bottom right
        k[1] = 1;
        k[4] = 4;
        ct = 2;
    } else if (x==0 && y==VIDEO_HEIGHT-1) { // bottom left
        k[1] = 1;
        k[2] = 2;
        ct = 2;
    } else if (x==VIDEO_WIDTH-1 && y==0) { // top right
        k[3] = 3;
        k[4] = 4;
        ct = 2;
    } else if (x==0) { // left wall
        k[1] = 1;
        k[2] = 2;
        k[3] = 3;
        ct = 3;
    } else if (x==VIDEO_WIDTH-1) { // right wall
        k[1] = 1;
        k[3] = 3;
        k[4] = 4;
        ct = 3;
    } else if (y==0) { // top
        k[2] = 2;
        k[3] = 3;
        k[4] = 4;
        ct = 3;
    } else if (y==VIDEO_HEIGHT-1) { // bottom
        k[1] = 1;
        k[2] = 2;
        k[4] = 4;
        ct = 3;
    } else { // any
        k[1] = 1;
        k[2] = 2;
        k[3] = 3;
        k[4] = 4;
        ct = 4;
    }

    if (k[1] == 1) {
        k[1] = !*pb(x, y-1);
        ct-=(k[1]?0:1);
    }

    if (k[2] == 2) {
        k[2] = !*pb(x+1, y);
        ct-=(k[2]?0:1);
    }

    if (k[3] == 3) {
        k[3] = !*pb(x, y+1);
        ct-=(k[3]?0:1);
    }

    if (k[4] == 4) {
        k[4] = !*pb(x-1, y);
        ct-=(k[4]?0:1);
    }

    if (ct == 0) {
        return 0;
    }

    i %= ct;
    for(int q=1;q<5;q++) {
        if (k[q]) {
            if (i==0) {
                return q;
            }
            i--;
        }
    }
    return 0;
}

void color(unsigned char* dest, unsigned char* src) {
    *dest = ((*src - GRADIENT_START + 1) % (256-GRADIENT_START)) + GRADIENT_START;
    // *dest = (*src + 1) % 256;
    // *dest = 130;
}

extern int init_vga(int blah);

int the_iowa_caucus(int argc, char *argv[]) {
    init_vga(0);

    pixels_back = mm_alloc(VIDEO_WIDTH * VIDEO_HEIGHT);

    int i; 
    for (i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
        *(pixels_back + i) = 0;
    }

    int x = sand_rand() % VIDEO_WIDTH;
    int y = sand_rand() % VIDEO_HEIGHT;

    *p(x, y) = sand_rand() % (256 - GRADIENT_START) + GRADIENT_START;
    
    *pb(x, y) = 5;

    int write = 0;
    do {
        int z = randdir(x,y);
        if (z) {write++;}
        switch(z) {
            case 0:
                switch(*pb(x,y)) {
                    case 5:
                        the_iowa_caucus_exit();
                        return 0;
                    case 1:
                        y--; break;
                    case 2:
                        x++; break;
                    case 3:
                        y++; break;
                    case 4:
                        x--; break;
                }
                break;
            case 1: // go up
                color(p(x,y-1), p(x,y));
                y--;
                *pb(x,y) = 3;
                break;
            case 3: // go down
                color(p(x,y+1), p(x,y));
                y++;
                *pb(x,y) = 1;
                break;
            case 2: // go right
                color(p(x+1,y), p(x,y));
                x++;
                *pb(x,y) = 4;
                break;
            case 4: // go left
                color(p(x-1,y), p(x,y));
                x--;
                *pb(x,y) = 2;
                break;
        }
    }
    while(*pb(x,y) != 5);
    the_iowa_caucus_exit();
    return 0;
}

int the_iowa_caucus_exit() {
    mm_free(pixels_back);
    halt();
    return 0;
}

 /*

struct pixl *pixels = 0;

#ifdef FRAMEBUFFER
int fb_fd = 0;
int sleep_t = 0;
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
uint8_t *fbp = 0;

inline uint32_t pixel_color(uint8_t r, uint8_t g, uint8_t b, struct fb_var_screeninfo *vinfo) {
    return (r<<vinfo->red.offset) | (g<<vinfo->green.offset) | (b<<vinfo->blue.offset);
}
#endif

void mapcolor(struct pixl *from, struct rgb *to) {
    switch(colormode) {
        case 1: //rgb
            rgb_create(from->r
                      ,from->g
                      ,from->b
                      ,to);
            break;
        case 2: //hsl
        rgb_from_hsl2(from->r
                    ,from->g
                    ,from->b
                    ,to);
        break;
    }
}

void color(struct pixl *dest, struct pixl *src) {
    switch(colormode) {
        case 1: //rgb
            rgb_color(dest, src);
            break;
        case 2:
            hsl_color(dest, src);
            break;
    }
}


#ifdef FRAMEBUFFER
void framebuffer(int x, int y) {
    struct rgb draw;
    mapcolor(p(x,y), &draw);
    if (sleep_t) {
        usleep(sleep_t);
    }
    long location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8)
                  + (y+vinfo.yoffset) * finfo.line_length;
    
    *((uint32_t*)(fbp + location))=pixel_color(draw.r, draw.g, draw.b, &vinfo);
}
#endif


#ifndef FRAMEBUFFER
struct BMPHeader {
    char bfType[2];       /* "BM" */
    //int bfSize;           /* Size of file in bytes */
    //int bfReserved;       /* set to 0 */
    //int bfOffBits;        /* Byte offset to actual bitmap data (= 54) */
    //int biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */
    //int biWidth;          /* Width of image, in pixels */
    //int biHeight;         /* Height of images, in pixels */
    //short biPlanes;       /* Number of planes in target device (set to 1) */
    //short biBitCount;     /* Bits per pixel (24 in this case) */
    //int biCompression;    /* Type of compression (0 if no compression) */
    //int biSizeImage;      /* Image size, in bytes (0 if no compression) */
    //int biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
    //int biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
    //int biClrUsed;        /* Number of colors in the color table (if 0, use 
    //                         maximum allowed by biBitCount) */
    //int biClrImportant;   /* Number of important colors.  If 0, all colors 
    //                         are important */
/*};

int write_bmp(const char *filename, int width, int height, struct pixl *rgb) {
    int i, j, ipos;
    int bytesPerLine;
    unsigned char *line;

    FILE *file;
    struct BMPHeader bmph;

    /* The length of each line must be a multiple of 4 bytes */
/*
    bytesPerLine = (3 * (width + 1) / 4) * 4;

    strcpy(bmph.bfType, "BM");
    bmph.bfOffBits = 54;
    bmph.bfSize = bmph.bfOffBits + bytesPerLine * height;
    bmph.bfReserved = 0;
    bmph.biSize = 40;
    bmph.biWidth = width;
    bmph.biHeight = height;
    bmph.biPlanes = 1;
    bmph.biBitCount = 24;
    bmph.biCompression = 0;
    bmph.biSizeImage = bytesPerLine * height;
    bmph.biXPelsPerMeter = 0;
    bmph.biYPelsPerMeter = 0;
    bmph.biClrUsed = 0;
    bmph.biClrImportant = 0;

    file = fopen (filename, "wb");
    if (file == NULL) return(0);

    fwrite(&bmph.bfType, 2, 1, file);
    fwrite(&bmph.bfSize, 4, 1, file);
    fwrite(&bmph.bfReserved, 4, 1, file);
    fwrite(&bmph.bfOffBits, 4, 1, file);
    fwrite(&bmph.biSize, 4, 1, file);
    fwrite(&bmph.biWidth, 4, 1, file);
    fwrite(&bmph.biHeight, 4, 1, file);
    fwrite(&bmph.biPlanes, 2, 1, file);
    fwrite(&bmph.biBitCount, 2, 1, file);
    fwrite(&bmph.biCompression, 4, 1, file);
    fwrite(&bmph.biSizeImage, 4, 1, file);
    fwrite(&bmph.biXPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biYPelsPerMeter, 4, 1, file);
    fwrite(&bmph.biClrUsed, 4, 1, file);
    fwrite(&bmph.biClrImportant, 4, 1, file);

    line = (unsigned char*)malloc(bytesPerLine);
    if (line == NULL) {
        fprintf(stderr, "Can't allocate memory for BMP file.\n");
        return(0);
    }

    struct rgb cur;

    for (i = height - 1; i >= 0; i--) {
        for (j = 0; j < width; j++) {
            ipos = (width * i + j);
            mapcolor(&(rgb[ipos]), &cur);
            line[3*j+0] = cur.b;
            line[3*j+1] = cur.g;
            line[3*j+2] = cur.r;
        }
        fwrite(line, bytesPerLine, 1, file);
    }

    fclose(file);

    return(1);
}
#endif
*/