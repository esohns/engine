#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

//#define LONGER

mat2 rot2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(27, 57, 111)));   
  return fract(vec3(2097152, 262144, 32768)*n)*2. - 1.; 
}

float
tetraNoise (vec3 p)
{
  vec3 i = floor(p + dot(p, vec3(1./3.)) );  p -= i - dot(i, vec3(1./6.));

  vec3 i1 = step(p.yzx, p), i2 = max(i1, 1. - i1.zxy); i1 = min(i1, 1. - i1.zxy);    

  vec3 p1 = p - i1 + 1./6., p2 = p - i2 + 1./3., p3 = p - .5;

  vec4 v = max(.5 - vec4(dot(p, p), dot(p1, p1), dot(p2, p2), dot(p3, p3)), 0.);

  vec4 d = vec4(dot(p, hash33(i)), dot(p1, hash33(i + i1)), 
                dot(p2, hash33(i + i2)), dot(p3, hash33(i + 1.)));

  return clamp(dot(d, v*v*v*8.)*1.732 + .5, 0., 1.);
}

float
fBm (vec3 p)
{
  float n = 0., sum = 0., a = 1.;
  vec3 offs = vec3(0, .23, .07);
  for(int i = min(0, iFrame); i<3; i++)
  {
    n += tetraNoise(p*exp2(float(i)) + offs.x)*a;
    sum += a;
    a *= .5;
    offs = offs.yzx;
  }

  return n/sum;
}

float
flow (vec3 p)
{
  p.z -= dot(p, p)*.5; 
  p.xy *= rot2(iTime/16.);
#ifdef LONGER
  p.z += .1*iTime;
#else
  p.z += .15*iTime;
#endif

  return fBm(p*1.5);
}

#ifdef LONGER
const float nFrames = 32.;
#else
const float nFrames = 16.;
#endif

void
main ()
{
  vec2 iR = iResolution.xy;
  vec2 uv = gl_FragCoord.xy/iR;
  vec2 uva = (gl_FragCoord.xy - iR/2.)/iR.y;

  vec2 e = vec2(.005, 0);
  vec3 p = vec3(uva, 0);
  float dx = (flow(p + e.xyy) - flow(p - e.xyy))/(2.*e.x);
  float dy = (flow(p + e.yxy) - flow(p - e.yxy))/(2.*e.x);
  vec2 curl = vec2(dy, -dx);

  uv += curl*.006*vec2(iR.y/iR.x, 1);

  vec3 val = texture(iChannel0, uv).rgb;

  float snNs = dot(sin(uv*8. - cos(uv.yx*12.)), vec2(.25)) + .5;
  vec3 col = .5 + .45*cos(6.2831*snNs/6. + vec3(0, 1.2, 2)*.8);

  col *= flow(p)*2. - .5;
  //col *= flow(vec3(uv, 0))*2. - .5;

  if(iFrame>0) col = mix(val, col, 1./nFrames);

  gl_FragColor = vec4(max(col, 0.), 1);
}
