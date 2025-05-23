#version 330

// glut_781_common.glsl
#define PI 3.14159265359
#define NEIGHBOR_DIST 6
#define DIFFUSION     1.12
#define ALIGNMENT     (0.7 + 0.3 * cos(PI * iTime/5.))
#define SEPARATION    (0.7 + 0.3 * cos(PI * iTime/6.))
#define COHESION      (0.7 + 0.3 * cos(PI * iTime/7.))

vec2
clamp_length (vec2 v, float r)
{
  if(length(v) > r) return r * normalize(v);
  return v;
}

uint
pack (vec2 x)
{
  x = 65535.0*clamp(0.5*x+0.5, 0., 1.);
  return uint(round(x.x)) + 65535u*uint(round(x.y));
}

vec2
unpack (uint a)
{
  vec2 x = vec2(a%65535u, a/65535u);
  return clamp(x/65535.0, 0.,1.)*2.0 - 1.0;
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
  float M;
};
    
particle
getParticle (vec4 data, vec2 pos)
{
  particle P;
  if (data == vec4(0)) return P;
  P.X = decode(data.x) + pos;
  P.M = data.y;
  P.V = data.zw;
  return P;
}

vec4
saveParticle (particle P, vec2 pos)
{
  vec2 x = clamp(P.X - pos, vec2(-0.5), vec2(0.5));
  return vec4(encode(x), P.M, P.V);
}

#define ITERATIONS 4

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec4(.1031, .1030, .0973, .1099)

float
hash11 (float p)
{
  vec3 p3  = fract(vec3(p) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

float
hash12 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE3);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec2
hash23 (vec3 p3)
{
  p3 = fract(p3 * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec3
hash31 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xxy+p3.yzz)*p3.zyx); 
}

vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yxz+19.19);
  return fract((p3.xxy+p3.yzz)*p3.zyx);
}

vec3
hash33 (vec3 p3)
{
  p3 = fract(p3 * HASHSCALE3);
  p3 += dot(p3, p3.yxz+19.19);
  return fract((p3.xxy + p3.yxx)*p3.zyx);
}

vec4
hash41 (float p)
{
  vec4 p4 = fract(vec4(p) * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash42 (vec2 p)
{
  vec4 p4 = fract(vec4(p.xyxy) * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash43 (vec3 p)
{
  vec4 p4 = fract(vec4(p.xyzx)  * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash44 (vec4 p4)
{
  p4 = fract(p4  * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}
// glut_781_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define MAX_SPEED 0.9
#define MAX_FORCE 0.05

void
main ()
{
  gl_FragColor = vec4(0);
  if(iFrame < 10)
  {
    float q = 2.*PI * hash12(1. + gl_FragCoord.xy);
    particle P;
    P.X = gl_FragCoord.xy;
    P.V = MAX_SPEED * vec2(cos(q), sin(q));
    P.M = 1.;
    gl_FragColor = saveParticle(P, gl_FragCoord.xy);
    return;
  }

  vec4 data = texture(iChannel0, gl_FragCoord.xy/iResolution.xy);
  particle P = getParticle(data, gl_FragCoord.xy);

  if(P.M < 0.01)
  {
    P.X = gl_FragCoord.xy;
    P.V = vec2(0);
    P.M = 0.05;
  }

  vec2 pos = P.X;
  vec2 vel = P.V;

  float nCount = 0.;

  vec2 alignment = vec2(0);
  vec2 cohesion = vec2(0);
  vec2 separation = vec2(0);

  for(int i = -NEIGHBOR_DIST; i <= NEIGHBOR_DIST; i++)
    for(int j = -NEIGHBOR_DIST; j <= NEIGHBOR_DIST; j++)
    {
      vec2 ij = vec2(i,j);
      if(ij == vec2(0) || length(ij) > float(NEIGHBOR_DIST)) continue;

      vec4 data2 = texture(iChannel0, fract((gl_FragCoord.xy + ij) / iResolution.xy));
      particle P2 = getParticle(data2, gl_FragCoord.xy + ij);
      vec2 pos2 = P2.X;
      vec2 vel2 = P2.V;
      float m = P2.M;

      separation += m * normalize(pos - pos2) / distance(pos, pos2);

      alignment += m * vel2;
      cohesion += m * pos2;
      nCount += m;

      data2 = textureLod(iChannel1, fract((gl_FragCoord.xy + ij) / iResolution.xy), 0.);
      if(data2.x > 0.001) separation += normalize(pos - data2.xy);

      vec2 coord = gl_FragCoord.xy + 16. * ij;
      data2 = textureLod(iChannel1, fract(coord / iResolution.xy), 4.);
      vel2 = data2.zw;
      separation += normalize(pos - coord) * length(vel2);
    }

  if (nCount > 0.)
    cohesion = cohesion / float(nCount) - pos;

  if(cohesion != vec2(0)) cohesion = clamp_length(MAX_SPEED * normalize(cohesion) - vel, MAX_FORCE);
  if(alignment != vec2(0)) alignment = clamp_length(MAX_SPEED * normalize(alignment) - vel, MAX_FORCE);
  if(separation != vec2(0)) separation = clamp_length(MAX_SPEED * normalize(separation) - vel, MAX_FORCE);

  vel += alignment * ALIGNMENT;
  vel += separation * SEPARATION;
  vel += cohesion * COHESION;
  vel -= 0.1 * textureLod(iChannel0, gl_FragCoord.xy / iResolution.xy, 7.).zw;
  P.V = clamp_length(vel, MAX_SPEED);
  gl_FragColor = saveParticle(P, gl_FragCoord.xy);
}
