#include "alloc.h"
#include "video.h"
// LOTS OF TODO HERE

unsigned int * vid_buffer;
int vid_x;
int vid_y;


static inline float float_abs(float in){
	return in < 0.0 ? -in : in;
}





//no bounds checks
void video_fill_rect(unsigned int color, int mx, int my, int lx, int ly){
	int y, x;
	for(y = my; y < ly; y++){
		unsigned int * line = vid_buffer + y * vid_x;
		for(x = mx; x < lx; x++){
			line[x] = color;
		}
	}
}

void video_draw_line(unsigned int color, int sx, int sy, int ex, int ey){
	if(sy > ey){
		int t = sy;
		sy = ey;
		ey = t;
	}
	//todo walk the tree
	float dx = sx - ex;
	if(dx != 0.0){
		float dy = sy - ey;
		float err = 0;
		float derr = float_abs(dy / dx);
		int x, y = sy;
		for(x = sx; x < ex; x++){
			vid_buffer[y*vid_x + x] = color;
			err += derr;
			while(err >= 0.5f){
				vid_buffer[y*vid_x + x] = color;
				y++;
				err-=1.0;
			}
		}
	} else { //vertical
		int y;
		for(y = sy; y < ey; y++){
			vid_buffer[y*vid_x + sx] = color;
		}
	}
}
