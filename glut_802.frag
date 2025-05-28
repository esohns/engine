#version 130

// glut_802_common.glsl
#define R iResolution.xy
#define ss(a,b,t) smoothstep(a,b,t)
#define N normalize
#define T(uv) texture(iChannel0, uv).r

float
hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * .1031);
  p3 += dot(p3, p3.zyx + 31.32);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hash23 (vec3 p3)
{
  p3 = fract(p3 * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xx+p3.yz)*p3.zy);
}

float
gyroid (vec3 seed)
{
  return dot(sin(seed),cos(seed.yzx));
}
// glut_802_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

float
fbm (vec3 seed)
{
  float result = 0.;
  float a = .5;
  for (int i = 0; i < 4; ++i)
  {
    seed += result / 2.;
    seed.y -= .1*iTime/a;
    result += gyroid(seed/a)*a;

    a /= 3.;
  }
  return result;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec2 p = (2.*gl_FragCoord.xy-R)/R.y;

  float rng = hash13(vec3(gl_FragCoord.xy, iFrame));

  vec3 seed = vec3(p, length(p) + iTime) * 2.;
  float noise = fbm(seed);
  float a = noise * 3.14;

  vec3 unit = vec3(vec2(rng*.005), 0.);
  vec3 normal = normalize(vec3(T(uv-unit.xz)-T(uv+unit.xz),
                               T(uv-unit.zy)-T(uv+unit.zy),
                               unit.y));

  vec2 mask = vec2(1.-abs(uv.x-.5)*2., uv.y);

  vec2 mouse = iMouse.xy/R;
  float clic = step(0., iMouse.z);

  float shape = 1.;

  shape *= ss(.01,.0,abs(uv.y));

  shape *= mask.x;

  shape *= pow(rng, 4.);

  vec2 offset = vec2(0);
  offset -= vec2(cos(a),sin(a)) * fbm(seed+.195) * (1.-mask.y);

  offset -= normal.xy * mask.y;

  vec2 velocity = vec2(0);
  p -= (2.*iMouse.xy-R)/R.y;
  float mouseArea = ss(.3,.0,length(p)-.1);
  offset -= clic * normalize(p) * mouseArea * 0.2;
  velocity += (texture(iChannel0, vec2(0)).yz - mouse);
  if (length(velocity) > .001)
    velocity = clic * normalize(velocity) * mouseArea;

  velocity = clamp(texture(iChannel0, uv+velocity*.05).yz * .99 + velocity * .5,-1.,1.);

  offset -= vec2(0,1) * (1.-mask.y);

  offset += velocity;

  uv += .015 * offset * mask.x;

  vec4 frame = texture(iChannel0, uv);

  float fade = iTimeDelta*.02;
  shape = max(shape, frame.r - fade);

  shape = clamp(shape, 0., 1.);
  gl_FragColor = vec4(shape, velocity, 1);

  if (gl_FragCoord.x < 1. && gl_FragCoord.y < 1.)
    gl_FragColor = vec4(0, mouse, 1);
}
