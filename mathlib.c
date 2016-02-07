#include "video.h"
#include "mathlib.h"
#define TRUE 1
#define FALSE 0

float F_sqrt(float n)
{
    /*We are using n itself as initial approximation
     *    This can definitely be improved */
    float x = n+0.001;
    float y = 1;
    float e = 0.000001; /* e decides the accuracy level*/
    while(x - y > e)
    {
        x = (x + y)/2;
        y = n/x;
    }
    return x;
}

//todo should i inline these?
void getBBoxPFromBBox(const vec_t * bbox, vec_t *bboxp){
	int i;
	for(i = 0; i < 8; i++){
		bboxp[(i*3)+0] = bbox[(i&1)+0];
		bboxp[(i*3)+1] = bbox[((i&2)>>1)+2];
		bboxp[(i*3)+2] = bbox[((i&4)>>2)+4];
	}

}
unsigned char checkBBoxPInBBox(const vec_t *bbox, const vec_t *bboxp){
	unsigned int j;
	for(j = 0; j < 24; j+=3){ if(bboxp[j] < bbox[0]) break;} if(j==24)return FALSE;
	for(j = 0; j < 24; j+=3){ if(bboxp[j] > bbox[1]) break;} if(j==24)return FALSE;
	for(j = 1; j < 24; j+=3){ if(bboxp[j] < bbox[2]) break;} if(j> 24)return FALSE;
	for(j = 1; j < 24; j+=3){ if(bboxp[j] > bbox[3]) break;} if(j> 24)return FALSE;
	for(j = 2; j < 24; j+=3){ if(bboxp[j] < bbox[4]) break;} if(j> 24)return FALSE;
	for(j = 2; j < 24; j+=3){ if(bboxp[j] > bbox[5]) break;} if(j> 24)return FALSE;
	return TRUE;
}

unsigned char checkVertsInBBox(const vec_t *bbox, const vec_t *verts, const unsigned int count){
	unsigned int j;
	unsigned int max = count*3;
	for(j = 0; j < max; j+=3){ if(verts[j] < bbox[0]) break;} if(j==max)return FALSE;
	for(j = 0; j < max; j+=3){ if(verts[j] > bbox[1]) break;} if(j==max)return FALSE;
	for(j = 1; j < max; j+=3){ if(verts[j] < bbox[2]) break;} if(j> max)return FALSE;
	for(j = 1; j < max; j+=3){ if(verts[j] > bbox[3]) break;} if(j> max)return FALSE;
	for(j = 2; j < max; j+=3){ if(verts[j] < bbox[4]) break;} if(j> max)return FALSE;
	for(j = 2; j < max; j+=3){ if(verts[j] > bbox[5]) break;} if(j> max)return FALSE;
	return TRUE;
}
/* changed to a #define
unsigned char checkBBoxInBBox(const vec_t * bboxa, const vec_t * bboxb){
	if(bboxa[1] < bboxb[0] && bboxb[1] < bboxa[0] && bboxa[3] < bboxb[2] && bboxb[3] < bboxa[2] && bboxa[5] < bboxb[4] && bboxb[5] < bboxa[4]) return TRUE;
	return FALSE;
}
*/
vec_t vec3distvec(const vec3_t b, const vec3_t c){
	vec3_t math;
	vec3subvec(math, b, c);
	return vec3length(math);
}
vec_t vec3distfastvec(const vec3_t b, const vec3_t c){
	vec3_t math;
	vec3subvec(math, b, c);
	return vec3dot(math, math);
}
vec_t vec4distvec(const vec4_t b, const vec4_t c){
	vec4_t math;
	vec4subvec(math, b, c);
	return vec4length(math);
}
vec_t vec4distfastvec(const vec4_t b, const vec4_t c){
	vec4_t math;
	vec4subvec(math, b, c);
	return vec4dot(math, math);
}

/*
vec_t * vec3norm(vec3_t b){
	vec_t length = vec3length(b);
	vec3_t out;
	vec3div(out, b, length);
	return out;
}
vec_t * vec4norm(vec4_t b){
	vec_t length = vec4length(b);
	vec4_t out;
	vec4div(out, b, length);
	return out;
}
*/

void vec2norm2(vec2_t a, const vec2_t b){
	vec_t length = vec2length(b);
	vec2div(a, b, length);
}
void vec3norm2(vec3_t a, const vec3_t b){
	vec_t length = vec3length(b);
	vec3div(a, b, length);
}
void vec4norm2(vec4_t a, const vec4_t b){
	vec_t length = vec4length(b);
	vec4div(a, b, length);
}


unsigned char checkSphereInSphere(const vec3_t sphere1, const vec_t size1, const vec3_t sphere2, const vec_t size2){
	vec_t tots = size1+size2;
	return(vec3distvec(sphere1, sphere2) < tots);
}
