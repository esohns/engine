#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

//#define ULTRAVIOLET
//#define DITHERING

#define pi 3.14159265
#define R(p, a) p=cos(a)*p+sin(a)*vec2(p.y, -p.x)

float freqs[4];

float
hash (float n)
{
  return fract(sin(n) * 13.5453123);
}

float
pn (vec3 p)
{
  vec3 ip = floor(p);
  p = fract(p);
  p *= p * (3.0 - 2.0 * p);
  vec2 uv = (ip.xy + vec2(37.0, 17.0) * ip.z) + p.xy;
  uv = texture(iChannel0, (uv + 0.5) / 256.0, -100.0).yx;
  return mix(uv.x, uv.y, p.z);
}

float
fpn (vec3 p)
{
  return pn(p * .06125) * .57 + pn(p * .125) * .28 + pn(p * .25) * .15;
}

float
rand (vec2 co)
{
  return fract(sin(dot(co * 0.123, vec2(12.9898, 78.233))) * 43758.5453);
}

float
cosNoise (vec2 p)
{
  return 0.3 * (sin(p.x) + sin(p.y));
}

const mat2 m2 = mat2 (1.6, -1.2,
                      1.2,  1.6);

float
sdTorus (vec3 p, vec2 t)
{
  return length(vec2(length(p.xz) - t.x * 1.2, p.y)) - t.y;
}

float
smin (float a, float b, float k)
{
  float h = clamp(1.0 + 0.5 * (b - a) / k, 0.0, 1.0);
  return mix(b, a, h) - k * h * (1.0 - h);
}

float
SunSurface (vec3 pos)
{
  float h = 0.0;
  vec2 q = pos.xz * 0.5;

  float s = 0.5;

  float d2 = 0.0;
  vec3 ipos = floor(pos);
  float id = hash(ipos.x * 1.0 + ipos.y * 10.0 + ipos.z * 20.0);

  for (int i = 0; i < 4; i++)
  {
    h += s * cosNoise(q);
    q = m2 * q * 1.05;
    q += vec2(2.41, 8.13);
    s *= 0.48 + 0.5 * h;
  }
  h += 0.4 * (freqs[0] * clamp(1.0 - abs(id - 0.20) / 0.30, 0.0, 1.0));
  h += 0.4 * (freqs[1] * clamp(1.0 - abs(id - 0.40) / 0.30, 0.0, 1.0));
  h += 0.4 * (freqs[2] * clamp(1.0 - abs(id - 0.60) / 0.30, 0.0, 1.0));
  h += 0.4 * (freqs[3] * clamp(1.0 - abs(id - 0.80) / 0.30, 0.0, 1.0));
  h *= 3.0;

  float d1 = pos.y - h;

  vec3 r1 = mod(2.3 + pos + 1.0, 10.0) - 5.0;
  r1.y = pos.y - 0.1 - 0.7 * h + 0.5 * sin(3.0 * iTime + pos.x + 3.0 * pos.z);
  float c = cos(pos.x);
  float s1 = 1.0; //sin(pos.x);
  r1.xz = c * r1.xz + s1 * vec2(r1.z, -r1.x);
  d2 = sdTorus(r1.xzy, vec2(clamp(abs(pos.x / pos.z), 0.7, 5.0), 0.50));

  return smin(d1, d2, 1.0);
}

float
map (vec3 p)
{
  p.z += 1.;
  R(p.yz, -25.5); // -1.0+iMouse.y*0.003);
  R(p.xz, iMouse.x*0.008*pi+iTime*0.1);
  return SunSurface(p) + fpn(p * 10. + iTime * 25.) * 0.45;
}

vec3
firePalette (float i)
{
  float T = 1400. + 1300. * i;
  vec3 L = vec3(7.4, 5.6, 4.4);
  L = pow(L, vec3(5.0)) * (exp(1.43876719683e5 / (T * L)) - 1.0);
  return 1.0 - exp(-5e8 / L);
}

void
main ()
{
  freqs[0] = texture(iChannel1, vec2(0.01, 0.25)).x;
  freqs[1] = texture(iChannel1, vec2(0.07, 0.25)).x;
  freqs[2] = texture(iChannel1, vec2(0.15, 0.25)).x;
  freqs[3] = texture(iChannel1, vec2(0.30, 0.25)).x;

  vec3 rd = normalize(vec3((gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y, 1.));
  vec3 ro = vec3(0., 0., -22.);

  float ld = 0.2, td = 0.2, w = 0.2;

  float d = 1., t = 1.;

  const float h = .2;

  vec3 tc = vec3(0.);

#ifdef DITHERING
   vec2 pos = (gl_FragCoord.xy / iResolution.xy );
   vec2 seed = pos + fract(iTime);
   //t=(1.+0.2*rand(seed));
#endif

  for (int i = 0; i < 56; i++)
  {
    if (td > (1. - 1. / 200.) || d < 0.001 * t || t > 40.)
      break;

    d = map(ro + t * rd);

    ld = (h - d) * step(d, h);
    w = (1. - td) * ld;

    tc += w * w + 1. / 60.;
    td += w + 1. / 200.;

#ifdef DITHERING  
#ifdef ULTRAVIOLET
      d = max(d, 0.04);
      d=abs(d)*(1.+0.28*rand(seed*vec2(i)));
#else
      d=abs(d)*(.8+0.28*rand(seed*vec2(i)));
      d = max(d, 0.04);
#endif 
#else
    d = max(d, 0.04);
#endif

    t += d * 0.5;
  }

  tc = firePalette(tc.x);
  tc *= 1. / 1.1;
#ifdef ULTRAVIOLET
   tc *= 1. / exp( ld * 2.82 ) * 1.05;
#endif

  gl_FragColor = vec4(tc, 1.0);
}
