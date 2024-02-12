precision highp float;

#define PI 3.14159265358979323846
#define MAX_DEPTH 150.
#define MAX_STEPS 450
#define EPSILON 0.0005

varying vec2 uv;
uniform vec2 pd;
uniform float t,rat;

mat2 rot(float a) {
  return mat2(cos(a), -sin(a), sin(a), cos(a));
}

float saturate (float x)
{
    return min(1.0, max(0.0,x));
}
vec3 saturate (vec3 x)
{
    return min(vec3(1.,1.,1.), max(vec3(0.,0.,0.),x));
}

//specral code from https://www.shadertoy.com/view/ls2Bz1
// --- Spectral Zucconi --------------------------------------------
// By Alan Zucconi
// Based on GPU Gems: https://developer.nvidia.com/sites/all/modules/custom/gpugems/books/GPUGems/gpugems_ch08.html
// But with values optimised to match as close as possible the visible spectrum
// Fits this: https://commons.wikimedia.org/wiki/File:Linear_visible_spectrum.svg
// With weighter MSE (RGB weights: 0.3, 0.59, 0.11)
vec3 bump3y (vec3 x, vec3 yoffset)
{
	vec3 y = vec3(1.,1.,1.) - x * x;
	y = saturate(y-yoffset);
	return y;
}
vec3 spectral_zucconi (float w)
{
    // w: [400, 700]
	// x: [0,   1]
	float x = saturate((w - 400.0)/ 300.0);

	const vec3 cs = vec3(3.54541723, 2.86670055, 2.29421995);
	const vec3 xs = vec3(0.69548916, 0.49416934, 0.28269708);
	const vec3 ys = vec3(0.02320775, 0.15936245, 0.53520021);

	return bump3y (	cs * (x - xs), ys);
}

//noise function from https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

//iq's sphere intersect function
vec2 sphIntersect( in vec3 ro, in vec3 rd, in vec3 ce, float ra )
{
    vec3 oc = ro - ce;
    float b = dot( oc, rd );
    float c = dot( oc, oc ) - ra*ra;
    float h = b*b - c;
    if( h<0.0 ) return vec2(-1.0); // no intersection
    h = sqrt( h );
    return vec2( -b-h, -b+h );
}

//basic raymarching stuff
float map(vec3 p) {
    float d = MAX_DEPTH;

    p.xz *= rot(abs(p.y));
    p.xy *= rot(t);
    p.xz *= rot(t*.5);
    p.y += fract(p.x*2.)/8.;
    d = length(p)-3.5;
    return d;
}

vec3 norm(vec3 p) {
    float h = EPSILON;
    vec2 k = vec2(1,-1);
    return normalize(k.xyy*map(p+k.xyy*h) +
                     k.yyx*map(p+k.yyx*h) +
                     k.yxy*map(p+k.yxy*h) +
                     k.xxx*map(p+k.xxx*h));
}

float march(vec3 ro, vec3 rd) {
    float a = 0.;
    for(int i = 0; i < MAX_STEPS; i++) {
        float d = map(ro+rd*a);
        a += d;
        if(a>MAX_DEPTH || d < EPSILON) break;
    }
    return a;
}

vec3 ortho_dir(vec3 t, vec3 e) { return normalize(t-e); }
vec3 ortho_origin(vec2 p, vec3 e, vec3 t, vec3 rd, vec3 u) {
    vec3 c_r = cross(rd, u);
    vec3 c_u = cross(c_r, rd);
    return e + c_r*p.x + c_u*p.y;
}

vec3 space(vec3 ro, vec3 rd) {
    vec3 c = vec3(0);

    c += vec3(smoothstep(0., .1, noise((ro+rd*0.)*5.)-.9));
    c += vec3(smoothstep(0., .1, noise((ro+rd*1.)*5.)-.9));

    for(float i = 5.; i < 10.; i+=.25) {
        float sd = noise((ro+rd*i)*.4);
        c += spectral_zucconi(sd*1400.)*.05;
    }

    return c;
}

void main()
{
  vec2 p = uv * 2.0 - 1.0;
  p.y /= rat;
	p.y += .5;
  vec3 col = vec3 (0);

  vec3 c_e = vec3(5, 5, 5);
  vec3 c_t = vec3(0);

  vec3 rd = ortho_dir(c_t, c_e);
  vec3 ro = ortho_origin(p, c_e, c_t, rd, vec3(0,1,0))*5.;

  float d = march(ro, rd);
  vec3 hit = ro+rd*d;
  vec3 n = norm(hit);

  if(d < MAX_DEPTH)
  {
    rd = reflect(rd,n);
    vec2 si =sphIntersect(hit, rd, vec3(0), 10.);
    col = space(hit+rd*si.x, rd);
  } else
  {
    col += vec3(.95);
  }

  gl_FragColor = vec4(col,1);
}
