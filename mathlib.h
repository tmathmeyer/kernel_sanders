#ifndef MATHLIBHEADER
#define MATHLIBHEADER
#ifndef M_PI
#define M_PI  3.14159265358979323846
#endif

#define vec2dot(a,b) ((a)[0] * (b)[0] + (a)[1] * (b)[1])
#define vec2length(a) sqrt(vec2dot((a), (a)))


#define vec3dot(a,b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define vec3cross(a,b,c) (a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; (a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; (a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];
#define vec3length(a) sqrt(vec3dot((a), (a)))
//#define vec3norm(a,b) {float dontusel = vec3length((b));(a)[0] = (b)[0]/dontusel;(a)[1] = (b)[1]/dontusel;(a)[2] = (b)[2]/dontusel;}

#define vec4dot(a,b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2] + (a)[3] * (b)[3])
//#define vec4cross(a,b,c) (a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; (a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; (a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];
#define vec4length(a) sqrt(vec4dot((a),(a)))
//#define vec4norm(a,b) {float dontusel = vec4length((b));(a)[0] = (b)[0]/dontusel;(a)[1] = (b)[1]/dontusel;(a)[2] = (b)[2]/dontusel;}

//add a vector to a vector
#define vec1addvec(a,b,c) (a)[0] = (b)[0] + (c)[0];
#define vec2addvec(a,b,c) vec1addvec((a),(b),(c)) (a)[1] = (b)[1] + (c)[1];
#define vec3addvec(a,b,c) vec2addvec((a),(b),(c)) (a)[2] = (b)[2] + (c)[2];
#define vec4addvec(a,b,c) vec3addvec((a),(b),(c)) (a)[3] = (b)[3] + (c)[3];

//subtract a vector from a vector
#define vec1subvec(a,b,c) (a)[0] = (b)[0] - (c)[0];
#define vec2subvec(a,b,c) vec1addvec((a),(b),(c)) (a)[1] = (b)[1] - (c)[1];
#define vec3subvec(a,b,c) vec2addvec((a),(b),(c)) (a)[2] = (b)[2] - (c)[2];
#define vec4subvec(a,b,c) vec3addvec((a),(b),(c)) (a)[3] = (b)[3] - (c)[3];


//add a scaler to a vector
#define vec1add(a,b,c) (a)[0] = (b)[0] + (c);
#define vec2add(a,b,c) vec1add((a),(b),(c)) (a)[1] = (b)[1] + (c);
#define vec3add(a,b,c) vec2add((a),(b),(c)) (a)[2] = (b)[2] + (c);
#define vec4add(a,b,c) vec3add((a),(b),(c)) (a)[3] = (b)[3] + (c);

//subtracts a scaler from a vector
#define vec1sub(a,b,c) (a)[0] = (b)[0] - (c);
#define vec2sub(a,b,c) vec1sub((a),(b),(c)) (a)[1] = (b)[1] - (c);
#define vec3sub(a,b,c) vec2sub((a),(b),(c)) (a)[2] = (b)[2] - (c);
#define vec4sub(a,b,c) vec3sub((a),(b),(c)) (a)[3] = (b)[3] - (c);


//multiply a vector by a vector
#define vec1multvec(a,b,c) (a)[0] = (b)[0] * (c)[0];
#define vec2multvec(a,b,c) vec1multvec((a),(b),(c)) (a)[1] = (b)[1] * (c)[1];
#define vec3multvec(a,b,c) vec2multvec((a),(b),(c)) (a)[2] = (b)[2] * (c)[2];
#define vec4multvec(a,b,c) vec3multvec((a),(b),(c)) (a)[3] = (b)[3] * (c)[3];

//multiply a vector by a scaler
#define vec1mult(a,b,c) (a)[0] = (b)[0] * (c);
#define vec2mult(a,b,c) vec1mult((a),(b),(c)) (a)[1] = (b)[1] * (c);
#define vec3mult(a,b,c) vec2mult((a),(b),(c)) (a)[2] = (b)[2] * (c);
#define vec4mult(a,b,c) vec3mult((a),(b),(c)) (a)[3] = (b)[3] * (c);


//divide a vector by a vector
#define vec1divvec(a,b,c) (a)[0] = (b)[0] / (c)[0];
#define vec2divvec(a,b,c) vec1divvec((a),(b),(c)) (a)[1] = (b)[1] / (c)[1];
#define vec3divvec(a,b,c) vec2divvec((a),(b),(c)) (a)[2] = (b)[2] / (c)[2];
#define vec4divvec(a,b,c) vec3divvec((a),(b),(c)) (a)[3] = (b)[3] / (c)[3];


//divide a vector by a scaler
#define vec1div(a,b,c) (a)[0] = (b)[0] / (c);
#define vec2div(a,b,c) vec1div((a),(b),(c)) (a)[1] = (b)[1] / (c);
#define vec3div(a,b,c) vec2div((a),(b),(c)) (a)[2] = (b)[2] / (c);
#define vec4div(a,b,c) vec3div((a),(b),(c)) (a)[3] = (b)[3] / (c);


#define vec1comp(a,b) ((a)[0] == (b)[0])
#define vec2comp(a,b) ((a)[0] == (b)[0] && (a)[1] == (b)[1])
#define vec3comp(a,b) ((a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2])
#define vec4comp(a,b) ((a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2] && (a)[3] == (b)[3])


//check if bboxes collide
#define testBBoxInBBox(a,b) ( (a)[1] < (b)[0] && (b)[1] < (a)[0] && (a)[3] < (b)[2] && (b)[3] < (a)[2] && (a)[5] < (b)[4] && (b)[5] < (a)[4] )


//todo should i inline these?
void getBBoxPFromBBox(const vec_t * bbox, vec_t *bboxp);
// distance from one vector to another (dist between points)
vec_t vec3distvec(const vec3_t b, const vec3_t c);
vec_t vec4distvec(const vec4_t b, const vec4_t c);
// non sqrt-d distance from one vector to another (good for comparisons)
vec_t vec3distfastvec(const vec3_t b, const vec3_t c);
vec_t vec4distfastvec(const vec4_t b, const vec4_t c);

//todo optimize?
//vec_t * vec3norm(vec3_t b);
//vec_t * vec4norm(vec4_t b);

void vec2norm2(vec2_t a, const vec2_t b);
void vec3norm2(vec3_t a, const vec3_t b);
void vec4norm2(vec4_t a, const vec4_t b);

unsigned char checkBBoxPInBBox(const vec_t *bbox, const vec_t *bboxp);
unsigned char checkSphereInSphere(const vec3_t sphere1, const vec_t size1, const vec3_t sphere2, const vec_t size2);
unsigned char checkVertsInBBox(const vec_t *bbox, const vec_t *verts, const unsigned int count);

#endif
