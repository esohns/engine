#version 300 es
precision mediump float;

bool alert = false;
int
assert (bool b)
{
  if (!b) alert = true;
  return 0;
}

const float PI = 3.141592654;
const float TWOPI = 2.0 * 3.141592654;
#define sin(x) sin(mod((x),TWOPI))
#define cos(x) cos(mod((x),TWOPI))

vec2
rotate (vec2 p, float t)
{
  return cos(t)*p + sin(t)*vec2(-p.y,p.x);
}

vec3
h2rgb (float h)
{
  vec3 rgb = clamp( abs(mod(h*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0 );
  return rgb*rgb*(3.0-2.0*rgb);
}

uint
ihash (uint x)
{
  x ^= x >> 16;
  x *= 0x7feb352dU;
  x ^= x >> 15;
  x *= 0x846ca68bU;
  x ^= x >> 16;
  return x;
}

float
segment (vec2 p, vec2 a, vec2 b)
{
  p -= a; b -= a;
  float h = dot(p,b)/dot(b,b);
  h = clamp(h, 0.0, 1.0);
  return length(p-b*h);
}

#define key(code) (texelFetch(iChannel3, ivec2((code),2),0).x != 0.0)

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeys0; // p,a,r,c
uniform bvec4 iKeys1; // g,b,d,l

out vec4 fragColor;

vec2
reduce (vec2 p, mat2 A, mat2 Ainv)
{
  if (p == vec2(0)) return p;
  while (true)
  {
    vec2 p1 = A*p;
    if (dot(p1,p1) >= dot(p,p)) break;
    p = p1;
  }
  while (true)
  {
    vec2 p1 = Ainv*p;
    if (dot(p1,p1) > dot(p,p)) break;
    p = p1;
  }

  return p;
}

vec3
getcol (vec2 ix)
{
  uint h = uint(int(ix.x)*12345^int(ix.y));
  h = ihash(h);
  return 0.2+0.8*h2rgb(0.8*float(h)/exp2(32.0));
}

float
bisect (vec2 p, vec2 q)
{
  vec2 r = 0.5*(p+q);
  vec2 n = p-q;
  return dot(r,n)/length(n);
}

vec2
perturb (vec2 p)
{
  if (iKeys0.x) return vec2(0);
  return 0.3*cos(1234.0*p+iTime);
}

void
main ()
{
  mat2 A = mat2(2,1,1,1);
  if (iKeys0.y) A = mat2(0,-1,1,3);
  float a = A[0][0], b = A[1][0], c = A[0][1], d = A[1][1];
  assert(a+d > 2.0);
  assert(a*d - b*c == 1.0);
  float trace = a+d;
  float disc = trace*trace-4.0;
  float l0 = 0.5*(trace-sqrt(disc));
  float l1 = 0.5*(trace+sqrt(disc));
  vec2 e0 = normalize(vec2(-b,a-l0));
  vec2 e1 = normalize(vec2(-b,a-l1));
  mat2 P = mat2(e0,e1);
  mat2 Ainv = inverse(A);
  mat2 Pinv = inverse(P);
  float cycle = log(l1);

  vec2 p = (2.0*gl_FragCoord.xy-iResolution)/iResolution.y;
  if (!iKeys0.z)
  {
    p += 0.25; p = rotate(p,0.05*iTime); p -= 0.25;
  }
  p *= 10.0;
  if (iMouse.x > 10.0) p *= exp2(3.0*iMouse.x/iResolution.x-1.0);
  float px = fwidth(p.x);
  float t0 = fract(0.25*(iTime+2.0))-0.5;
  float k = pow(l1,t0);
  p *= vec2(k,1.0/k); p = P*p;
  vec2 ix = round(p); p -= ix;
  vec3 col = vec3(1,1,0.75);
  float radius = 0.8;
  if (iMouse.y > 10.0) radius = iMouse.y/iResolution.y;
  int t[18] = int[](-1,-1, -1,0, 0,-1, -1,1, 0,0, 1,-1, 0,1, 1,0, 1,1);
  float dmin = 1e8;
  vec2 ixmin,pmin;
  for (int ii = 0; ii < 18; ii += 2)
  {
    int i = t[ii], j = t[ii+1];
    vec2 ix1 = ix+vec2(i,j);
    vec2 p1 = p - vec2(i,j);
    p1 = Pinv*p1; p1 *= vec2(1.0/k,k);
    p1 += perturb(reduce(ix1,A,Ainv));
    if (length(p1) < dmin)
    {
      dmin = length(p1);
      ixmin = ix1;
      pmin = p1;
    }
  }
  if (!iKeys0.w)
    col = getcol(reduce(ixmin, A, Ainv));
  dmin = 1e8;
  for (int ii = 0; ii < 18; ii += 2)
  {
    int i = t[ii], j = t[ii+1];
    vec2 ix1 = ix+vec2(i,j);
    if (ix1 == ixmin) continue;
    vec2 p1 = p - vec2(i,j);
    p1 = Pinv*p1; p1 *= vec2(1.0/k,k);
    p1 += perturb(reduce(ix1,A,Ainv));
    dmin = min(dmin,bisect(p1,pmin));
  }
  if (iKeys1.x)
    col *= 0.5 + 0.5 * mod(ix.x + ix.y, 2.0);
  if (!iKeys1.y) col *= smoothstep(0.0, px, dmin - 0.02);
  if (iKeys1.z) col *= 0.5 + 0.5 * cos(50.0*dmin);
  if (iKeys1.w) col *= smoothstep(0.0, px, length(pmin) - 0.05);

  col = pow(col,vec3(0.4545));
  if (alert) col.r = 1.0;
  fragColor = vec4(col,1.0);
}
