#version 130

uniform vec2 iResolution;
uniform float iTime;

#define time iTime
#define time2 (time*2.1 + ((1.0+sin(time + sin(time*0.4+ cos(time*0.1)))))*1.5)
#define time3 (time*1. + ((1.0+sin(time*0.9 + sin(time*0.34+ cos(time*0.21)))))*1.5)
#define time4 (time*0.5 + ((1.0+sin(time*0.8 + sin(time*0.14+ cos(time*0.15)))))*1.2)

vec2
hash (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3.zxy, p3.yxz+19.19);
  return -1.0 + 2.0*fract(vec2(p3.x * p3.y, p3.z*p3.x));
}

float
noise (vec2 p)
{
  p *= 0.45;
  const float K1 = 0.366025404;
  const float K2 = 0.211324865;

  vec2 i = floor( p + (p.x+p.y)*K1 );

  vec2 a = p - i + (i.x+i.y)*K2;
  vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0);
  vec2 b = a - o + K2;
  vec2 c = a - 1.0 + 2.0*K2;

  vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

  vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

  return dot( n, vec3(38.0) );
}

mat2 rot (float a) { float c = cos(a), s = sin(a);return mat2(c,s,-s,c); }

float
fbm (vec2 p, vec2 of)
{
  p *= rot(time3*0.1);
  p += of;
  float z=2.;
  float rz = 0.;
  vec2 bp = p;
  for (float i= 1.;i <9.;i++ )
  {
    rz+= noise(p*rot(float(i)*2.3)+ time*0.5)/z;
    z *= 3.2;
    p *= 2.0;
  }
  return rz;
}

vec2
grdf (vec2 p, vec2 of)
{
  vec2 ep = vec2(0.0,0.0005);
  vec2 d = vec2(fbm(p - ep.yx, of) - fbm(p + ep.yx, of),
                fbm(p - ep.xy, of) - fbm(p + ep.xy, of));
  d /= length(d);
  return d;
}

void
main ()
{
  vec2 p = gl_FragCoord.xy / iResolution.xy-0.5;
  p.x *= iResolution.x/iResolution.y;
  p *= 1.75;

  float t1 = mod(time2*0.35,4.);
  float t2 = mod(time2*0.35 + 1.,4.);

  vec2 p1 = p*(4.0-t1);
  vec2 p2 = p*(4.0-t2);

  vec2 fld = grdf(p1, vec2(time4*0.2,time*0.0));
  vec2 fld2 = grdf(p2, vec2(time4*0.2,time*0.0) + 2.2);

  gl_FragColor = vec4(fld, fld2);
}
