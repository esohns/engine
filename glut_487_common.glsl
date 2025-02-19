#define texel(a, p) texelFetch(a, ivec2(p), 0)
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3
#define R iResolution.xy
#define PI 3.14159265

#define dt 3.
#define loop(i,x) for(int i = min(0, iFrame); i < x; i++)

//rendering scale
#define SC 1.

#define smoothR 2.5
#define density 0.036

//sim stuff
struct obj
{
    int id; //ID
    vec2 X; //position
    vec2 V; //velocity
    float Pressure; //pressure
    float Rho; //neighbor density
    float SScale; //smooth scale
    float Div; //average distance to neighbors
    vec4 Y; //additional data
};
    
float Force(float d)
{
    return 0.2*exp(-0.05*d)-2.*exp(-0.5*d);
}

//40% of the buffer used for particles
#define P 0.5
#define SN ivec2(4, 2)

ivec2 N; //buffer size
ivec2 sN; //buffer single element size
int TN; //buffer length

ivec2 i2xy(ivec3 sid)
{
    return sN*ivec2(sid.x%N.x, sid.x/N.x) + sid.yz;
}

ivec3 xy2i(ivec2 p)
{
    ivec2 pi = p/sN;
    return ivec3(pi.x + pi.y*N.x, p.x%sN.x, p.y%sN.y);
}

ivec2 cross_distribution(int i)
{
    return (1<<(i/4)) * ivec2( ((i&2)/2)^1, (i&2)/2 ) * ( 2*(i%2) - 1 );
}

float sqr(float x)
{
return x*x + 1e-2;
}

//hash funcs
vec2 hash22(vec2 p)
{
	vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

float hash13(vec3 p3)
{
	p3  = fract(p3 * .1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yxz+33.33);
    return fract((p3.xxy + p3.yxx)*p3.zyx);
}

vec3 hash31(float p)
{
   vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
   p3 += dot(p3, p3.yzx+33.33);
   return fract((p3.xxy+p3.yzz)*p3.zyx); 
}
