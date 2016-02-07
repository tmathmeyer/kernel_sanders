
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
}

extern int init_vga(int blah);

int the_iowa_caucus(int argc, char *argv[]) {
    init_vga(0);

    while(1) {
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
            for(int i = 0; i<10000; i++);
        }
        while(*pb(x,y) != 5);
        
        for (i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT; i++) {
            *(pixels_back + i) = 0;
            color(p(i,0), 0);
        }
    }
    the_iowa_caucus_exit();
    return 0;
}

int the_iowa_caucus_exit() {
    mm_free(pixels_back);
    halt();
    return 0;
}
