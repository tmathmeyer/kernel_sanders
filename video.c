#include "alloc.h"
#include "video.h"
#include "sandersboard.h"
#include "sandersio.h"
#include "boopt.h"
#include "sleep.h"
#include <math.h>
#include "matrixlib.h"
// LOTS OF TODO HERE

int vid_x;
int vid_y;



extern char * vidmem;



//no bounds checks
void video_fill_rect(unsigned char color, int mx, int my, int lx, int ly){
	int t;
	if(mx > lx){
		t = mx;
		mx = lx;
		lx = t;
	}
	if(my > ly){
		t = my;
		my = ly;
		ly = t;
	}
	int y, x;
	for(y = my; y < ly; y++){
		unsigned char * line = vidmem + y * vid_x;
		for(x = mx; x < lx; x++){
			line[x] = color;
		}
	}
}
void video_fill_texture(texture_t t, int mx, int my, int lx, int ly){
	int y, x, tx, ty;
	for(ty = 0, y = my; y < ly; y++, ty++){
		unsigned char * line = vidmem + y * vid_x;
		unsigned char * tline = t.data + ty * t.res[0];
		for(tx = 0, x = mx; x < lx; x++, tx++){
			line[x] = tline[tx];
		}
	}
}

void video_draw_line(unsigned char color, int sx, int sy, int ex, int ey){
	if(sy > ey){
		int t = sy;
		sy = ey;
		ey = t;
	}
	float dx = sx - ex;
	if(dx != 0.0){
		float dy = sy - ey;
		float err = 0;
		float derr = (dy / dx);
		if(derr < 0.0) derr = - derr;
		int x, y = sy;
		for(x = sx; x < ex; x++){
			vidmem[y*vid_x + x] = color;
			err += derr;
			while(err >= 0.5f){
				vidmem[y*vid_x + x] = color;
				y++;
				err-=1.0;
			}
		}
	} else { //vertical
		int y;
		for(y = sy; y < ey; y++){
			vidmem[y*vid_x + sx] = color;
		}
	}
}
void line(unsigned char color, int x0, int y0, int x1, int y1) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
  int err = (dx>dy ? dx : -dy)/2, e2;
  for(;;){
    vidmem[y0 * vid_x + x0] = color;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}static inline float orient2d(vec2_t a, vec2_t b, vec2_t c){
    return (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
}
static inline float min3(float a, float b, float c){
	float ret = a;
	if(b < a) ret = b;
	if(c < a) ret = c;
	return ret;
}
static inline float max3(float a, float b, float c){
	float ret = a;
	if(b > a) ret = b;
	if(c > a) ret = c;
	return ret;
}
static inline float max(float a, float b){
	return a > b ? a : b;
}
static inline float min(float a, float b){
	return a < b ? a : b;
}

static inline int iorient2d(ivec2_t a, ivec2_t b, ivec2_t c){
    return (b[0]-a[0])*(c[1]-a[1]) - (b[1]-a[1])*(c[0]-a[0]);
}
static inline int imin3(int a, int b, int c){
	int ret = a;
	if(b < a) ret = b;
	if(c < a) ret = c;
	return ret;
}
static inline int imax3(int a, int b, int c){
	int ret = a;
	if(b > a) ret = b;
	if(c > a) ret = c;
	return ret;
}
static inline int imax(int a, int b){
	return a > b ? a : b;
}
static inline int imin(int a, int b){
	return a < b ? a : b;
}


int screenHeight;
int screenWidth;

int (*renderPixel)(ivec2_t pos, vec3_t weights, void * tridata);

void video_draw_triangle(unsigned int color, ivec_t *verts, void * tridata){
	ivec_t *v0 = verts;
	ivec_t *v1 = verts+2;
	ivec_t *v2 = verts+4;
	// Compute triangle bounding box
	int minX = imin3(v0[0], v1[0], v2[0]);
	int minY = imin3(v0[1], v1[1], v2[1]);
	int maxX = imax3(v0[0], v1[0], v2[0]);
	int maxY = imax3(v0[1], v1[1], v2[1]);
	// Clip against screen bounds
	minX = imax(minX, 0);
	minY = imax(minY, 0);
	maxX = imin(maxX, screenWidth - 1);
	maxY = imin(maxY, screenHeight - 1);
	// Rasterize
	ivec2_t p;
	for(p[1] = minY; p[1] <= maxY; p[1]++){
		for(p[0] = minX; p[0] <= maxX; p[0]++){
			// Determine barycentric coordinates
			int w0 = iorient2d(v1, v2, p);
			int w1 = iorient2d(v2, v0, p);
			int w2 = iorient2d(v0, v1, p);
			// If p is on or inside all edges, render pixel.
			if(w0 >= 0 && w1 >= 0 && w2 >= 0){
				vec3_t weights;
				weights[0] = 0.33;
				weights[1] = 0.33;
				weights[2] = 0.33;
				renderPixel(p, weights, tridata);
			}
		}
	}
}

void video_key_handler(char keycodde){
	return;
}


matrix4x4_t proj;
vec_t  verts[24] = {
-1.0, -1.0, -1.0,
-1.0, -1.0, 1.0,
-1.0, 1.0, -1.0,
-1.0, 1.0, 1.0,
1.0, -1.0, -1.0,
1.0, -1.0, 1.0,
1.0, 1.0, -1.0,
1.0, 1.0, 1.0 };
int indices[24] = { 0, 1, 0, 2, 1, 3, 2, 3, 4, 5, 4, 6, 5, 7, 6, 7,
0, 4, 1, 5, 2, 6, 3, 7};

int oldc1[48] = {0};
int newc1[48] ={0};
int oldc2[48] = {0};
int newc2[48] ={0};
void calc_cube(int * buf, float spinsx, float spinsy, float spinsz, int centx, int centy, float scaler){
	matrix4x4_t meet;
	matrix4x4_t mvp;
	Matrix4x4_CreateFromQuakeEntity(&meet, 0.0, 0.0, 3.0, spinsx, spinsy, spinsy, 1.0);
	Matrix4x4_Concat(&mvp, &proj, &meet);
	int i;

	for( i = 0; i < 12; i++){
		vec_t * pp = &verts[indices[i*2] * 3];
		vec_t * ps = &verts[indices[i*2+1] * 3];
//		vec_t * pp = &verts[0];
//		vec_t * ps = &verts[3];
		vec4_t in1, out1, in2, out2;
		in1[0] = pp[0];
		in1[1] = pp[1];
		in1[2] = pp[2];
		in1[3] = 1.0;
		in2[0] = ps[0];
		in2[1] = ps[1];
		in2[2] = ps[2];
		in2[3] = 1.0;
		Matrix4x4_Transform4(&mvp, in1, out1);
		Matrix4x4_Transform4(&mvp, in2, out2);
		out1[0] /= out1[3];
		out1[1] /= out1[3];
		out2[0] /= out2[3];
		out2[1] /= out2[3];
		//now "clip" to screen coords
		int x1, x2, y1, y2;
		buf[i * 4] = out1[0] * scaler+centx;
		buf[i * 4+1] = out1[1] * scaler+centy;
		buf[i * 4+2] = out2[0] * scaler+centx;
		buf[i * 4+3] = out2[1] * scaler+centy;
	}

}
void draw_cube(char color, int * old, int * new){
	int i = 0;
	for(i = 0; i < 12; i++){
		line(0, old[i*4], old[i*4+1], old[i*4+2], old[i*4+3]);
	}
	for(i = 0; i < 12; i++){
		line(color,new[i*4], new[i*4+1], new[i*4+2], new[i*4+3]);
	}
}

extern int init_vga(int blah);
int videorun(int argc, char * argv[]){
	set_keyboard_handler(&video_key_handler);
	init_vga(0);
	vid_x = 320;
	vid_y = 200;

	float dz = 10.0 - 0.1;
	proj.m[0][0] = 1.0 / ((float)vid_x / (float)vid_y);
	proj.m[1][1] = 1.0;
	proj.m[2][2] = -(10.0 + 0.1) / dz;
	proj.m[2][3] = -1.0;
	proj.m[3][2] = -2.0 * 0.1 * 10.0 / dz;
	int i,z;
	int jj = 0;
	/*
	for(i = 0; i < 320 * 200; i++){
		vidmem[i] = (i*123) % 255;
	}*/
	for(i = 0; 1; i++){
//		video_fill_rect(0, 0, 0, vid_x-1, vid_y-1);
//		for(z = 0; z < 320; z++){
//			video_draw_line(z % 39, z, 100, z, 100 + 25 * sin(z/10.0));
//		}
//		video_fill_rect((i/50)%39, 50, 50, 100, 100);
//		int x = cos(i / 1000.0 + z * M_PI * 0.5) * 50;
//		int y = sin(i / 1000.0 + z * M_PI * 0.5) * 50;
	int x = i % 160;
	int y = 30;
		float xoff = sin(i * 0.001 + M_PI * 0.5);
		float yoff = sin(i * 0.001);
		float cent = sin(i * 0.00131 + 0.00313) * 20.0;
		line(i/ 1000 % 39, 160 + xoff * cent, 100 + yoff * cent, 160 + xoff * (40.0 + cent * 0.1), 100 + yoff * (40 + cent * 0.1));
		if(!(i % 10)){
			jj = !jj;
			calc_cube( jj ? newc1 : oldc1, i * 0.01, sin(i * 0.00011) * 500.0,0.0, 70, 100, 100.0);
			calc_cube( jj ? newc2 : oldc2, i * 0.01 + 30.0, sin(i * 0.000051) * 1000.0,0.0, 250, 100, 100.0);
			if(jj) draw_cube(6, oldc1, newc1);
			else draw_cube(6, newc1, oldc1);
			if(jj) draw_cube(10, oldc2, newc2);
			else draw_cube(10, newc2, oldc2);
		}
		sleepy_sanders(10000);
	}
//	sanders_printf("video mode is %i\n", get_mode());
//	video_mode();
//	text_mode();
	return 1;
}
