#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

//#define VARIANT
//#define DH_VARIANT

#define TIME        iTime
#define RESOLUTION  iResolution
#define PI          3.141592654
#define TAU         (2.0*PI)

float
height (vec2 p)
{
  vec2 n = mod(floor(p), 2.0);
  p = fract(p);
  p = mix(p, 1.0-p, n);
  vec4 tcol = texture(iChannel0, p);
  float h = 1.25*(sqrt(abs(tcol.x))); 

  return h;
}

float
circle (vec2 p, float r)
{
#if defined(VARIANT)
  p *= p;
  return pow(dot(p, p), 0.25) - r;
#else
  return length(p) - r;
#endif
}

float
cell0 (vec2 p)
{
  float d0 = circle(p-0.5, 0.5);
  float d1 = circle(p+0.5, 0.5);
  float d = d0;
  d = min(d, d1);
  d = abs(d);

  return d;
}

float
hash (vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,58.233))) * 13758.5453);
}

vec3
layer (vec3 col, vec3 bcol, vec2 p, vec2 o, float z, float co, float aa)
{
  p /= z;
#if defined(VARIANT)
  p += 0.5;
#endif
  vec2 np = round(p);
  vec2 cp = p - np;
  vec2 rcp = vec2(cp.y, -cp.x);
  vec2 hp = np*z*(0.25*0.25);
  hp += o;
  float h = height(hp);
  float h0 = hash(np);
  if (h0 > 0.5)
    cp = rcp;
  float fo = dot(cp, cp);
  float d = cell0(cp)-0.01;
  d *= z;
  if (h > co)
    col = mix(col, bcol-0.5*fo, smoothstep(aa, -aa, d));

  return col;
}

#define ROT(a) mat2(cos(a), sin(a), -sin(a), cos(a))

vec3
effect (vec2 p, vec2 pp)
{
  float aa = 2.0/RESOLUTION.y;
  vec3 col = vec3(0.0);
  const float maxco = 1.0;
  const float minco = 0.0;
  const float maxi  = 9.0;
  const float amp   = 8.0;
  const mat2 rot    = ROT(0.2); 
  vec2 op = p;
  vec2 o = amp*cos(vec2(1.0, 0.5)*TAU*(TIME+20.0)/(amp*30.0));
  p += o;
  p *= rot;
  for (float i = 0.0; i <= maxi; ++i)
  {
#ifdef DH_VARIANT
    p+= i*.002*iTime;
#endif    
    vec3 bcol = 0.5*(1.0+cos(vec3(0.0, 1.0, 2.0)+0.5*i-0.5*TIME+length(p)));
    float z = pow(0.5, i)*4.0;
    float co = mix(minco, maxco, i/(maxi));

    col = layer(col, bcol, p, vec2(0.0), z, co, aa);
  }
  col *= smoothstep(1.5, 0.5, length(pp));
  col = max(col, 0.0);
  col = sqrt(col);

  return col;
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/RESOLUTION;
  vec2 p = -1. + 2. * q;
  vec2 pp = p;
  p.x *= RESOLUTION.x/RESOLUTION.y;
  vec3 col = effect(p, pp);  

  gl_FragColor = vec4(col, 1.0);
}
