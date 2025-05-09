#version 130

// glut_764_common.glsl
#define R iResolution

int
IHash (int a)
{
  a = (a ^ 61) ^ (a >> 16);
  a = a + (a << 3);
  a = a ^ (a >> 4);
  a = a * 0x27d4eb2d;
  a = a ^ (a >> 15);
  return a;
}

float
Hash (int a)
{
  return float(IHash(a)) / float(0x7FFFFFFF);
}

vec4
rand4 (int seed)
{
  return vec4(Hash(seed ^ 0x34F85A93),
              Hash(seed ^ 0x85FB93D5),
              Hash(seed ^ 0x6253DF84),
              Hash(seed ^ 0x25FC3625));
}

vec3
rand3 (int seed)
{
  return vec3(Hash(seed ^ 0x348CD593),
              Hash(seed ^ 0x8593FD5),
              Hash(seed ^ 0x62A5D384));
}

vec2
rand2 (int seed)
{
  return vec2(Hash(seed ^ 0x348C5F93),
              Hash(seed ^ 0x8593D5BB));
}

vec2
randn (vec2 randuniform)
{
  vec2 r = randuniform;
  r.x = sqrt(-2. * log(1e-9 + abs(r.x)));
  r.y *= 6.28318;
  r = r.x * vec2(cos(r.y), sin(r.y));
  return r;
}

float
dot2 (vec2 x)
{
  return dot(x, x);
}

int
cvt (float x)
{
  return int(x);
}

float
cvt (int x)
{
  return float(x);
}
// glut_764_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

vec4
A (int i)
{
  return texture(iChannel0, (vec2((i - 1) % int(R.x), (i - 1) / int(R.x)) + .5) / R.xy);
}

void
main ()
{
  vec2 I = gl_FragCoord.xy;
  vec4 O = texture(iChannel0, I / R.xy);

  O.xy = mod(O.xy, R.xy);
  vec4 r = rand4(int(I.x) + int(I.y) * 2048 + iFrame * 2048 * 2048);
  if (iFrame < 3)
  {
    O.xy = r.xy * R.xy;
    O.zw = .25 * cos(6.283 * (vec2(0, .25) + r.z));
  }

  vec4 a = texture(iChannel1, (.5 + floor(O.xy)) / R.xy);

  vec2 ns = vec2(0);
  vec2 df = vec2(0);
  for (int i = 0; i < 4; i++)
  {
    vec4 n = A(cvt(a[i]));
    ns += n.zw / 4.;
    vec2 D = mod(O.xy - n.xy + R.xy / 2., R.xy) - R.xy / 2.;
    if (length(D) > .005 && length(D) < 10.)
      df += normalize(D) * (1. / (length(D) + .03));
  }

  O.zw += df / 25.;
  O.zw = mix(O.zw, ns, .1);
  if (length(O.zw) > .001)
    O.zw = mix(O.zw, normalize(O.zw) / 4., .05);
  O.zw += randn(r.xy) / 1e2;

  if (iMouse.z > 0.)
  {
    vec2 D = O.xy - iMouse.xy;
    O.zw += normalize(D) * (1. / (length(D) + .03));
  }
  O.xy += O.zw;

  gl_FragColor = O;
}
