#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

vec2
Rot (vec2 p, float t)
{
  float c = cos(t); float s = sin(t);
  return vec2(p.x*c+p.y*s,
             -p.x*s+p.y*c);
}

vec3
random3 (vec3 c)
{
  float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
  vec3 r;
  r.z = fract(512.0*j);
  j *= .125;
  r.x = fract(512.0*j);
  j *= .125;
  r.y = fract(512.0*j);
  r = r-0.5;
  
  float t = -iTime*.5;
  r.xy = Rot(r.xy,t);

  return r;
}

const float F3 =  0.3333333;
const float G3 =  0.1666667;

float
noise (vec3 p)
{
  vec3 s = floor(p + dot(p, vec3(F3)));
  vec3 x = p - s + dot(s, vec3(G3));

  vec3 e = step(vec3(0.0), x - x.yzx);
  vec3 i1 = e*(1.0 - e.zxy);
  vec3 i2 = 1.0 - e.zxy*(1.0 - e);

  vec3 x1 = x - i1 + G3;
  vec3 x2 = x - i2 + 2.0*G3;
  vec3 x3 = x - 1.0 + 3.0*G3;

  vec4 w, d;

  w.x = dot(x, x);
  w.y = dot(x1, x1);
  w.z = dot(x2, x2);
  w.w = dot(x3, x3);

  w = max(0.6 - w, 0.0);

  d.x = dot(random3(s), x);
  d.y = dot(random3(s + i1), x1);
  d.z = dot(random3(s + i2), x2);
  d.w = dot(random3(s + 1.0), x3);

  w *= w;
  w *= w;
  d *= w;

  return dot(d, vec4(52.0));
}

vec2
RotCS (vec2 p, float c, float s)
{
  return vec2(p.x * c + p.y * s,
             -p.x * s + p.y * c);
}

vec2
hash (vec2 p)
{
  p = vec2( dot(p,vec2(127.1,311.7)),
            dot(p,vec2(269.5,183.3)) );

  vec2 h = -1.0 + 2.0*fract(sin(p)*43758.5453123);

#if 1
  float t = -iTime*0.7;
  float co = cos(t); float si = sin(t);	
  h = RotCS(h,co,si);
#endif

  return h;
}

float
noise (vec2 p)
{
  const float K1 = 0.366025404;
  const float K2 = 0.211324865;

  vec2 i = floor( p + (p.x+p.y)*K1 );

  vec2 a = p - i + (i.x+i.y)*K2;
  vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0);
  vec2 b = a - o + K2;
  vec2 c = a - 1.0 + 2.0*K2;

#if 1
  float t = iTime*.5;
  float co = cos(t); float si = sin(t);	
  a = RotCS(a,co,si);
  b = RotCS(b,co,si);
  c = RotCS(c,co,si);
#endif

  vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

  vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

  return dot( n, vec3(70.0) );
}

float
pot (vec2 pos)
{
  float t = iTime*.1;

  vec3 p = vec3(pos+vec2(iTime*.4,0.),t);

  float n = noise(p);
  n += 0.5 *noise(p*2.13);
  n += 3. * noise(pos*0.333);
  
  return n;
}

vec2
field (vec2 pos)
{
  float s = 1.5;
  pos *= s;

  float n = pot(pos);

  float e = 0.1;
  float nx = pot(vec2(pos+vec2(e,0.)));
  float ny = pot(vec2(pos+vec2(0.,e)));

  return vec2(-(ny-n),nx-n)/e;
}

void
main () 
{
  float lod = 0.;

  vec2 uv = gl_FragCoord.xy;
  uv /= iResolution;
  uv.x *= iResolution.x/iResolution.y;
  uv.y = 1. - uv.y;
  vec2 src_uv = uv;

  vec3 d = vec3(0.);
  vec3 e = vec3(0.);
  for (int i=0; i<25; i++)
  {
    d += texture(iChannel0,uv+iTime*0.05,lod).xyz;
    e += texture(iChannel0,-uv.yx*3.+iTime*0.0125,lod).xyz;

    vec2 new_uv = field(uv)*.00625*.5;

    lod += length(new_uv)*5.;
    uv += new_uv;
  }

  vec3 c = texture(iChannel0,uv*.1+iTime*0.025,lod).xyz;

  d *= (1./50.);
  e *= (1./50.);
  c = mix(c,d,length(d));
  c = mix(c,e,length(e));

  gl_FragColor = vec4(c,1);
}
