#version 330

// glut_744_common.glsl
#define Bf(p) p
#define Bi(p) ivec2(p)
#define texel(a, p) texelFetch(a, Bi(p), 0)
#define pixel(a, p) texture(a, (p)/R)
#define ch0 iChannel0
#define ch1 iChannel1
#define ch2 iChannel2
#define ch3 iChannel3

#define PI 3.14159265

#define loop(i,x) for(int i = 0; i < x; i++)
#define range(i,a,b) for(int i = a; i <= b; i++)

#define dt 1.5

#define border_h 5.

vec2 R;
vec4 Mouse;
float time;

float
Pf (vec2 rho)
{
  float GF = smoothstep(0.49, 0.5, 1. - rho.y);
  return mix(0.5 * rho.x, 0.04 * rho.x * (rho.x / 0.2 - 1. + 0.1 * rho.y), GF);
}

mat2
Rot (float ang)
{
  return mat2(cos(ang), -sin(ang), sin(ang), cos(ang));
}

vec2
Dir (float ang)
{
  return vec2(cos(ang), sin(ang));
}

float
sdBox (vec2 p, vec2 b)
{
  vec2 d = abs(p) - b;
  return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float
border (vec2 p)
{
  float bound = -sdBox(p - R * 0.5, R * vec2(0.5, 0.5));
  float box = 1e10 + sdBox(Rot(0.4 * time) * (p - R * 0.5), R * vec2(0.005, 0.2));
  float drain = -sdBox(p - R * vec2(0.9, 0.05), vec2(0));
  return max(drain, min(bound, box));
}

#define h 1.
vec3
bN (vec2 p)
{
  vec3 dx = vec3(-h, 0, h);
  vec4 idx = vec4(-1. / h, 0., 1. / h, 0.25);
  vec3 r = idx.zyw * border(p + dx.zy)
           + idx.xyw * border(p + dx.xy)
           + idx.yzw * border(p + dx.yz)
           + idx.yxw * border(p + dx.yx);
  return vec3(normalize(r.xy), r.z + 1e-4);
}

uint
pack (vec2 x)
{
  x = 65534.0 * clamp(0.5 * x + 0.5, 0., 1.);
  return uint(round(x.x)) + 65535u * uint(round(x.y));
}

vec2
unpack (uint a)
{
  vec2 x = vec2(a % 65535u, a / 65535u);
  return clamp(x / 65534.0, 0., 1.) * 2.0 - 1.0;
}

vec2
decode (float x)
{
  uint X = floatBitsToUint(x);
  return unpack(X);
}

float
encode (vec2 x)
{
  uint X = pack(x);
  return uintBitsToFloat(X);
}

struct particle
{
  vec2 X;
  vec2 V;
  vec2 M;
};
    
particle
getParticle (vec4 data, vec2 pos)
{
  particle P;
  P.X = decode(data.x) + pos;
  P.V = decode(data.y);
  P.M = data.zw;
  return P;
}

vec4
saveParticle (particle P, vec2 pos)
{
  P.X = clamp(P.X - pos, vec2(-0.5), vec2(0.5));
  return vec4(encode(P.X), encode(P.V), P.M);
}

vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yxz + 33.33);
  return fract((p3.xxy + p3.yzz) * p3.zyx);
}

float
G (vec2 x)
{
  return exp(-dot(x, x));
}

float
G0 (vec2 x)
{
  return exp(-length(x));
}

#define dif 1.1
vec3
distribution (vec2 x, vec2 p, float K)
{
  vec4 aabb0 = vec4(p - 0.5, p + 0.5);
  vec4 aabb1 = vec4(x - K * 0.5, x + K * 0.5);
  vec4 aabbX = vec4(max(aabb0.xy, aabb1.xy), min(aabb0.zw, aabb1.zw));
  vec2 center = 0.5 * (aabbX.xy + aabbX.zw);
  vec2 size = max(aabbX.zw - aabbX.xy, 0.);
  float m = size.x * size.y / (K * K);
  return vec3(center, m);
}

void
Reintegration (sampler2D ch, inout particle P, vec2 pos)
{
  float Mi = 0.;
  range(i, -2, 2) range(j, -2, 2)
  {
    vec2 tpos = pos + vec2(i, j);
    vec4 data = texel(ch, tpos);

    particle P0 = getParticle(data, tpos);

    P0.X += P0.V * dt;

    vec3 D = distribution(P0.X, pos, dif);
    vec3 D1 = distribution(P0.X, pos, 1.);

    float m = P0.M.x * D.z;

    P.X += D.xy * m;
    P.V += P0.V * m;

    P.M.y += P0.M.y * P0.M.x * D1.z;

    P.M.x += m;
    Mi += P0.M.x * D1.z;
  }

  if (P.M.x != 0.)
  {
    P.X /= P.M.x;
    P.V /= P.M.x;
  }
  if (Mi != 0.)
    P.M.y /= Mi;
}

void
Simulation (sampler2D ch, inout particle P, vec2 pos)
{
  vec2 F = vec2(0.);
  vec3 avgV = vec3(0.);
  range(i, -2, 2) range(j, -2, 2)
  {
    vec2 tpos = pos + vec2(i, j);
    vec4 data = texel(ch, tpos);
    particle P0 = getParticle(data, tpos);
    vec2 dx = P0.X - P.X;
    float avgP = 0.5 * P0.M.x * (Pf(P.M) + Pf(P0.M));
    F -= 1. * G(1. * dx) * avgP * dx;
    avgV += P0.M.x * G(1. * dx) * vec3(P0.V, 1.);
  }
  avgV.xy /= avgV.z;

  F += 0. * P.M.x * (avgV.xy - P.V);

  F += P.M.x * vec2(0., -0.0005) + P.M.x * step(0.5, P.M.y) * vec2(0., 0.005);

  if (Mouse.z > 0.)
  {
    vec2 dm = (Mouse.xy - Mouse.zw) / 10.;
    float d = distance(Mouse.xy, P.X) / 20.;
    F += 0.001 * dm * exp(-d * d);
    P.M.y += 0.1 * exp(-40. * d * d);
  }

  P.V += F * dt / P.M.x;

  vec3 N = bN(P.X);
  float vdotN = step(N.z, border_h) * dot(-N.xy, P.V);
  P.V += 1. * (N.xy * vdotN + N.xy * abs(vdotN));
  P.V += P.M.x * N.xy * step(abs(N.z), border_h) * exp(-N.x);

  if (length(vec2(1., 1.) * (P.X - R * vec2(0.5, 0.1))) < 10.)
    P.M.y = mix(P.M.y, 1., 0.06);
  if (N.z < 2. * border_h)
    P.M.y *= 0.9;

  float v = length(P.V);
  P.V /= (v > 1.) ? v : 1.;
}
// glut_744_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define mass 1.

void
main ()
{
  vec4 U;
  vec2 pos = gl_FragCoord.xy;

  R = iResolution.xy;
  time = iTime;
  Mouse = iMouse;
  ivec2 p = ivec2(pos);

  vec2 F = vec2(0.);

  vec4 data = texel(ch0, pos);

  particle P;

  Reintegration(ch0, P, pos);

  if (iFrame < 1)
  {
    vec3 rand = hash32(pos);
    if (rand.z < 0.3)
    {
      P.X = pos;
      P.V = 0.5 * (rand.xy - 0.5) + vec2(0., 0.);
      P.M = vec2(mass, 0.);
    }
    else
    {
      P.X = pos;
      P.V = vec2(0.);
      P.M = vec2(0.);
    }
  }

  U = saveParticle(P, pos);

  gl_FragColor = U;
}
