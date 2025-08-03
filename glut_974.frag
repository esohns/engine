#version 130

// glut_974_common.glsl
#define PI 3.141592653589
#define E  2.718281828459
#define EPSILON 0.0000001

#define KEY_LEFT  37
#define KEY_UP    38
#define KEY_RIGHT 39
#define KEY_DOWN  40
#define KEY_SPACE 32
#define KEY_1     49
#define KEY_9     57

const vec3 v0 = vec3(0,0,0);
const vec3 vx = vec3(1,0,0);
const vec3 vy = vec3(0,1,0);
const vec3 vz = vec3(0,0,1);

const vec3 red   = vec3(0.8,0.0,0.0);
const vec3 green = vec3(0.0,0.5,0.0);
const vec3 blue  = vec3(0.2,0.2,1.0);
const vec3 white = vec3(1.0,1.0,1.0);

struct _text
{
  ivec2 size;
  ivec2 adv;
} text;

struct _gl
{
  vec2  uv;
  vec2  frag;
  vec2  mouse;
  vec2  mp;
  ivec2 ifrag;
  float aspect;
  vec4  color;
  int   option;
} gl;

void
initGlobal (vec2 fragCoord, vec2 resolution, vec4 mouse)
{
  text.size = ivec2(16,32);
  text.adv  = ivec2(text.size.x,0);

  mouse.xy = min(mouse.xy,resolution);
  if (mouse.z == 0.0) gl.mouse = resolution * 0.5;
  else gl.mouse = mouse.xy;

	gl.mp = (2.0*abs(gl.mouse)-resolution)/resolution.y;

  gl.aspect = resolution.x / resolution.y;
  gl.frag   = fragCoord;
  gl.ifrag  = ivec2(fragCoord);
  gl.uv     = (fragCoord+fragCoord-resolution)/resolution.y;
}

float
hash11 (float p)
{
  p = fract(p * 0.1031);
  p *= p + 33.33;
  p *= p + p;

  return fract(p);
}

vec3
hash33 (vec3 p3)
{
  p3 = fract(p3 * vec3(0.1031, 0.1030, 0.0973));
  p3 += dot(p3, p3.yxz+33.33);

  return fract((p3.xxy + p3.yxx)*p3.zyx);
}

vec3
hash31 (float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);

  return fract((p3.xxy+p3.yzz)*p3.zyx); 
}

float
hash12 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * .1031);
  p3 += dot(p3, p3.yzx + 33.33);

  return fract((p3.x + p3.y) * p3.z);
}

float
gradientNoise (vec2 v)
{
  return fract(52.9829189 * fract(dot(v, vec2(0.06711056, 0.00583715))));
}

vec3
hsl2rgb (vec3 c)
{
  vec3 rgb = clamp( abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );

  return c.z + c.y * (rgb-0.5)*(1.0-abs(2.0*c.z-1.0));
}

vec3 hsl (float h, float s, float l) { return hsl2rgb(vec3(h,s,l)); }

vec3
rgb2hsl (vec3 col)
{
  float minc = min( col.r, min(col.g, col.b) );
  float maxc = max( col.r, max(col.g, col.b) );
  vec3  mask = step(col.grr,col.rgb) * step(col.bbg,col.rgb);
  vec3 h = mask * (vec3(0.0,2.0,4.0) + (col.gbr-col.brg)/(maxc-minc + EPSILON)) / 6.0;
  return vec3( fract( 1.0 + h.x + h.y + h.z ),              
               (maxc-minc)/(1.0-abs(minc+maxc-1.0) + EPSILON),  
               (minc+maxc)*0.5 );
}

mat3
alignMatrix (vec3 dir) 
{
  vec3 f = normalize(dir);
  vec3 s = normalize(cross(f, vec3(0.48, 0.6, 0.64)));
  vec3 u = cross(s, f);

  return mat3(u, s, f);
}

float rad2deg (float r) { return 180.0 * r / PI; }
float deg2rad (float d) { return PI * d / 180.0; }

mat3
rotMat (vec3 u, float angle)
{
  float s = sin(deg2rad(angle));
  float c = cos(deg2rad(angle));
  float i = 1.0-c;

  return mat3(
    c+u.x*u.x*i, u.x*u.y*i-u.z*s, u.x*u.z*i+u.y*s,
    u.y*u.x*i+u.z*s, c+u.y*u.y*i, u.y*u.z*i-u.x*s,
    u.z*u.x*i-u.y*s, u.z*u.y*i+u.x*s, c+u.z*u.z*i
  );
}

vec3
rotAxisAngle (vec3 position, vec3 axis, float angle)
{
  mat3 m = rotMat(axis, angle);

  return m * position;
}

vec4
quatAxisAngle (vec3 axis, float angle)
{ 
  float half_angle = deg2rad(angle*0.5);

  return vec4(axis*sin(half_angle), cos(half_angle));
}

vec4
quatConj (vec4 q)
{ 
  return vec4(-q.x, -q.y, -q.z, q.w); 
}

vec4
quatMul (vec4 q1, vec4 q2)
{ 
  vec4 qr;
  qr.x = (q1.w * q2.x) + (q1.x * q2.w) + (q1.y * q2.z) - (q1.z * q2.y);
  qr.y = (q1.w * q2.y) - (q1.x * q2.z) + (q1.y * q2.w) + (q1.z * q2.x);
  qr.z = (q1.w * q2.z) + (q1.x * q2.y) - (q1.y * q2.x) + (q1.z * q2.w);
  qr.w = (q1.w * q2.w) - (q1.x * q2.x) - (q1.y * q2.y) - (q1.z * q2.z);

  return qr;
}

vec3
rotate (vec4 q, vec3 p)
{
  vec4 conj = quatConj(q);
  vec4 q_tmp = quatMul(q, vec4(p, 0));

  return quatMul(q_tmp, conj).xyz;
}

vec3
rotate (vec4 q, vec3 o, vec3 p)
{
  vec4 conj = quatConj(q);
  vec4 q_tmp = quatMul(q, vec4(p-o, 0));

  return o + quatMul(q_tmp, conj).xyz;
}

vec3
rotAxisAngleQuat (vec3 p, vec3 axis, float angle)
{ 
  vec4 qr = quatAxisAngle(axis, angle);

  return quatMul(quatMul(qr, vec4(p, 0)), quatConj(qr)).xyz;
}

vec3
rotRayAngle (vec3 p, vec3 ro, vec3 rd, float angle)
{ 
  return rotAxisAngle(p-ro, rd-ro, angle)+ro;
}

vec3
rotY (vec3 v, float d)
{
  float r = deg2rad(d);
  float c = cos(r);
  float s = sin(r);

  return vec3(v.x*c+v.z*s, v.y, v.z*c+v.x*s);
}

vec3
rotX (vec3 v, float d)
{
  float r = deg2rad(d);
  float c = cos(r);
  float s = sin(r);

  return vec3(v.x, v.y*c+v.z*s, v.z*c+v.y*s);
}

vec3
rotZ (vec3 v, float d)
{
  float r = deg2rad(d);
  float c = cos(r);
  float s = sin(r);

  return vec3(v.x*c+v.y*s, v.y*c+v.x*s, v.z);
}

vec3
posOnPlane (vec3 p, vec3 a, vec3 n)
{
  return p-dot(p-a,n)*n;
}

vec3
posOnRay (vec3 ro, vec3 rd, vec3 p)
{
  return ro + max(0.0, dot(p - ro, rd) / dot(rd, rd)) * rd;
}

bool
rayIntersectsSphere (vec3 ro, vec3 rd, vec3 ctr, float r)
{
  return length(posOnRay(ro, rd, ctr) - ctr) < r;
}

float
opDiff (float d1, float d2) 
{
  float k = 0.05;
  float h = clamp(0.5 - 0.5*(d2+d1)/k, 0.0, 1.0);

  return mix(d1, -d2, h) + k*h*(1.0-h); 
}

float
opUnion (float d1, float d2) 
{
  float k = 0.15;
  float h = clamp(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);

  return mix(d2, d1, h) - k*h*(1.0-h);
}

float
sdCone (vec3 p, vec3 a, vec3 b, float r1, float r2)
{
  vec3 ab = b-a;
  vec3 ap = p-a;
  float t = dot(ab,ap) / dot(ab,ab);
  t = clamp(t, 0.0, 1.0);
  vec3 c = a + t*ab;

  return length(p-c)-(t*r2+(1.0-t)*r1);      
}

float
sdCylinder (vec3 p, vec3 a, vec3 b, float r)
{
  vec3  ba = b - a;
  vec3  pa = p - a;
  float baba = dot(ba,ba);
  float paba = dot(pa,ba);
  float x = length(pa*baba-ba*paba) - r*baba;
  float y = abs(paba-baba*0.5)-baba*0.5;
  float x2 = x*x;
  float y2 = y*y*baba;
  float d = (max(x,y)<0.0)?-min(x2,y2):(((x>0.0)?x2:0.0)+((y>0.0)?y2:0.0));

  return sign(d)*sqrt(abs(d))/baba;
}
// glut_974_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel1;

#define save(a,c) if(gl.ifrag.x==(a)&&gl.ifrag.y==0){gl.color=(c);}
#define load(x) texelFetch(iChannel1, ivec2(x,0), 0)

int  id = -1;
int  num = 0;
vec4 val;

#define SPEED    0.07
#define DAMP     0.6
#define MAX_FISH 511.

vec2 hash (int n) { return fract(sin(vec2(float(n),float(n)*7.))*43758.5); }

vec2
repulse (vec4 fish, vec2 target, float dist, float a)
{
  vec2 w = target-fish.xy;
  float x = length(w);
  if (x < EPSILON)
    return a*0.5*(hash31(float(id))-vec3(0.5)).xy;

  return w*a*(smoothstep(0.0, dist, x)-1.0)/x;
}

vec2
attract (vec4 fish, vec2 target, float dist, float a, float s2, float s3)
{
  vec2  w = target-fish.xy;
  float x = length(w);
  float k = dist+s2;
  float d = (2.0*dist+s2)*0.5;
  float xkd = (x-d)/(k-d);
  float l = min(max(-1.0, -abs(x-d)/(k-d)), xkd*exp(1.0+xkd));
  float r = max(-1.0, xkd*exp(1.0-xkd)-max(0.0,(1.0-s3)*(1.0-exp(1.0-x/k))));
  float f = a*max(r,l)*0.5+0.5;

  return w*f;
}

vec2
swirl (vec4 fish, vec2 center, float a, float k)
{
  vec2  w = center-fish.xy;
  w = vec2(-w.y,w.x);
  float x = length(w);

  return w*a/pow(x,k);
}

void
boid ()
{
  vec2 vel, acc, w, u = vec2(0.0);
  vec4 fish = load(id);
  float d, v, a;

  for (int i = 1; i <= num; i++) 
  {
    if (i == id)
      continue;
    acc += repulse(fish, load(i).xy, 0.07, 4.0);
  }

  acc += attract(fish, gl.mp, 0.25, 0.01, 0.2, 0.1);
  acc += repulse(fish, gl.mp, 0.15, 10.0);
  acc += swirl  (fish, gl.mp, 0.008, 2.0);

  acc = normalize(acc)*min(0.5,length(acc));
  vel = fish.zw + acc*SPEED;

  vel *= pow(DAMP, SPEED);

  w = fish.xy+vel*SPEED;

  float b = 0.05;
  val.x  = clamp(w.x,-1.0*gl.aspect+b,1.0*gl.aspect-b);
  val.y  = clamp(w.y,-1.0+b,1.0-b);
  val.zw = vel;

  save(id, val);
}

void
initFish (int id)
{
  float r = 2.0*hash11(float(id))-1.0;
  save(id, 0.1*vec4(r, 2.0*fract(r*123.0)-1.0, 0.0, 0.0)); 
}

void
main ()
{
  initGlobal(gl_FragCoord.xy, iResolution, iMouse);

  if (iFrame == 0) 
  {
  	save(0,vec4(1,0,0,0));
    initFish(1);
    gl_FragColor = gl.color;
    return;
  }

  ivec2 mem = ivec2(gl_FragCoord.xy);
  if (mem.y > 0)
    return;
  id = mem.x;

  val = load(id);
  num = int(load(0).x);

  if (id == 0)
  {
    save(id, vec4(clamp(floor(iTime*100.0)+1.0, 1.0, MAX_FISH),0,0,0));
  }
  else
  {
    if (id > num)
      initFish (id);
    else
      boid ();
  }

  gl_FragColor = gl.color;
}
