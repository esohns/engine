#version 130

uniform vec2 iResolution;
uniform float iTime;

#define TIME        iTime
#define RESOLUTION  iResolution
#define PI          3.141592654
#define TAU         (2.0*PI)
#define ROT(a)      mat2(cos(a), sin(a), -sin(a), cos(a))

const int max_iter = 5;

vec2
mod2 (inout vec2 p, vec2 size)
{
  vec2 c = floor((p + size*0.5)/size);
  p = mod(p + size*0.5,size) - size*0.5;
  return c;
}

float
hash (vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,58.233))) * 13758.5453);
}

float
box (vec2 p, vec2 b)
{
  vec2 d = abs(p)-b;
  return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float
pmin (float a, float b, float k)
{
  float h = clamp(0.5+0.5*(b-a)/k, 0.0, 1.0);
  return mix(b, a, h) - k*h*(1.0-h);
}

float
pabs (float a, float k)
{
  return -pmin(a, -a, k);
}

vec2
toPolar (vec2 p)
{
  return vec2(length(p), atan(p.y, p.x));
}

vec2
toRect (vec2 p)
{
  return vec2(p.x*cos(p.y), p.x*sin(p.y));
}

float
modMirror1 (inout float p, float size)
{
  float halfsize = size*0.5;
  float c = floor((p + halfsize)/size);
  p = mod(p + halfsize,size) - halfsize;
  p *= mod(c, 2.0)*2.0 - 1.0;
  return c;
}

float
smoothKaleidoscope (inout vec2 p, float sm, float rep)
{
  vec2 hp = p;

  vec2 hpp = toPolar(hp);
  float rn = modMirror1(hpp.y, TAU/rep);

  float sa = PI/rep - pabs(PI/rep - abs(hpp.y), sm);
  hpp.y = sign(hpp.y)*(sa);

  hp = toRect(hpp);

  p = hp;

  return rn;
}

float
shape (vec2 p)
{
  const float amp = 10.0;
  p += amp*sin(vec2(1.0, sqrt(0.5))*0.026*TIME*TAU/amp);
  vec2 cp = p;
  vec2 np = round(p);
  cp -= np;

  float h0 = hash(np+123.4); 
  if (h0 > 0.5)
    cp = vec2(-cp.y, cp.x);

  vec2 cp0 = cp;
  cp0 -= -0.5;
  float d0 = (length(cp0)-0.5);
  vec2 cp1 = cp;
  cp1 -= 0.5;
  float d1 = (length(cp1)-0.5);
  
  float d = d0;
  d = min(d, d1);
  d = abs(d)-0.125;
  return d;
}

vec2
df (vec2 p, out int ii, out bool inside)
{
  float sz = 0.9;
  float ds = shape(p);
  vec2 pp = p;

  float r = 0.0;

  ii = max_iter;
  for (int i=0; i<max_iter; ++i) {
    pp = p;
    vec2 nn = mod2(pp, vec2(sz));

    vec2 cp = nn*sz;
    float d = shape(cp);
    
    r = sz*0.5; 

    if (abs(d) > 0.5*sz*sqrt(2.0))
    {
      ii = i;
      inside = d < 0.0;
      break;
    }

    sz /= 3.0;
  }
  
  float aa = 0.25*sz;

  float d0 = box(pp, vec2(r-aa))-aa; 
  float d1 = length(pp);
  return vec2(d0, d1);
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/RESOLUTION.xy;
  vec2 p = -1. + 2. * q;
  vec2 pp = p;
  p.x *= RESOLUTION.x/RESOLUTION.y;
  float aa = 4.0/RESOLUTION.y;
  vec2 op = p;
  mat2 rot =ROT(0.0125*TIME); 
  p *= rot;
  smoothKaleidoscope(p, 0.025, 34.0);
  p *= ROT(0.25*length(op));
  p *= transpose(rot);

  int i;
  bool inside;
  vec2 d2 = df(p, i, inside);
  float ii = float(i)/float(max_iter);
  vec3 col = vec3(0.0);
  vec3  rgb = 0.5*(1.0+cos(0.5*TIME-0.5*PI*length(p)+vec3(0.0, 1.0, 2.0)+ PI*ii+(inside?(2.0*(dot(p,pp)+1.0)):0.0)));
  rgb += 0.0025/max(d2.y, 0.005);
  col = mix(col, rgb, smoothstep(0.0, -aa, d2.x));
  col -= vec3(0.25)*(length(op)+0.0);
  col *= smoothstep(1.5, 0.5, length(pp));
  col = sqrt(col);

  gl_FragColor = vec4(col, 1.0);
}
