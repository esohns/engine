#version 130

// glut_727_common.glsl
#define dt 0.25
#define prad 1.4
#define decay 0.15

float pspeed = 6.;

float sdist = 10.;
float sst = 10.;
float sangl = 0.3; //radians

#define pdens 2.
#define size iResolution.xy
#define pixel(a, p) texture(a, p/vec2(textureSize(a,0)))
#define texel(a, p) texelFetch(a, ivec2(p-0.5), 0)
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3
#define PI 3.14159265

float
hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

float
hash12 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * .1031);
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.xx + p3.yz) * p3.zy);
}

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.xx + p3.yz) * p3.zy);
}

float
gauss (vec2 x, float r)
{
  return exp(-pow(length(x) / r, 2.));
}

float
interpolate (float val, float y0, float x0, float y1, float x1)
{
  return (val - x0) * (y1 - y0) / (x1 - x0) + y0;
}

float
base (float val)
{
  if (val <= -0.75)
    return 0.0;
  else if (val <= -0.25)
    return interpolate(val, 0.0, -0.75, 1.0, -0.25);
  else if (val <= 0.25)
    return 1.0;
  else if (val <= 0.75)
    return interpolate(val, 1.0, 0.25, 0.0, 0.75);

  return 0.0;
}

vec3
jet_colormap (float v)
{
  return vec3(base(v - 0.5), base(v), base(v + 0.5));
}

vec3
jet_range (float v, float a, float b)
{
  return jet_colormap(2. * clamp((v - a) / (b - a), 0., 1.) - 1.);
}

vec4
Laplace (sampler2D ch, vec2 p)
{
  vec3 dx = vec3(-1, 0., 1);
  return texel(ch, p+dx.xy) + texel(ch, p+dx.yx) + texel(ch, p+dx.zy) + texel(ch, p+dx.yz) - 4. * texel(ch, p);
}

vec2
Grad (sampler2D ch, vec2 p)
{
  vec3 dx = vec3(-1, 0., 1);
  return vec2(length(texel(ch, p+dx.zy)), length(texel(ch, p+dx.yz))) - length(texel(ch, p));
}
// glut_727_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

vec2
loop_d (vec2 pos)
{
  return mod(pos + size * 0.5, size) - size * 0.5;
}

vec2
loop (vec2 pos)
{
  return mod(pos, size);
}

void
Check (inout vec4 U, vec2 pos, vec2 dx)
{
  vec4 Unb = texel(ch0, loop(pos+dx));
  if (length(loop_d(Unb.xy - pos)) < length(loop_d(U.xy - pos)))
    U = Unb;
}

void
CheckRadius (inout vec4 U, vec2 pos, float r)
{
  Check(U, pos, vec2(-r, 0));
  Check(U, pos, vec2(r, 0));
  Check(U, pos, vec2(0, -r));
  Check(U, pos, vec2(0, r));
}

void
main ()
{
  vec4 U;
  vec2 pos = gl_FragCoord.xy;

  vec2 muv = iMouse.xy / size;

  if (length(muv.xy) > 0.)
  {
    sdist *= muv.x;
    sst *= muv.y;
  }
  else
  {
    sdist *= 0.8;
    sst *= 0.05;
  }

  U = texel(ch0, pos);
    
  CheckRadius(U, pos, 1.);
  CheckRadius(U, pos, 2.);
  CheckRadius(U, pos, 3.);
  CheckRadius(U, pos, 4.);
  CheckRadius(U, pos, 5.);
   
  U.xy = loop(U.xy);

  if (length(U.xy - pos) > 10.)
    U.xy += 1. * (hash22(pos) - 0.5);

  vec2 sleft = U.xy + sdist * vec2(cos(U.z + sangl), sin(U.z + sangl));
  vec2 sright = U.xy + sdist * vec2(cos(U.z - sangl), sin(U.z - sangl));

  float dangl = (pixel(ch1, sleft).x - pixel(ch1, sright).x);
  U.z += dt * sst * tanh(3. * dangl);

  vec2 pvel = pspeed * vec2(cos(U.z), sin(U.z)) + 0.1 * (hash22(U.xy + iTime) - 0.5);;

  U.xy += dt * pvel;

  U.xy = loop(U.xy);

  if (iFrame < 1)
  {
    U.xy = vec2(pdens * round(pos.x / pdens), pdens * round(pos.y / pdens));
    U.zw = hash22(U.xy) - 0.5;
  }

  gl_FragColor = U;
}
