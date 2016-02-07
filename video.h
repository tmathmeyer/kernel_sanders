#ifndef VIDEOHEADER
#define VIDEOHEADER

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 200

#define GRADIENT_START 64

typedef int ivec_t;
typedef float vec_t;
typedef vec_t vec2_t[2];
typedef vec_t vec3_t[3];
typedef vec_t vec4_t[4];

typedef ivec_t ivec2_t[2];
typedef ivec_t ivec3_t[3];
typedef ivec_t ivec4_t[4];

typedef struct texture_s {
	ivec2_t res;
	unsigned char * data;
} texture_t;


int videorun(int argc, char * argv[]);
float F_sin (float angle);

#endif


