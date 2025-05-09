#version 130

// glut_765_common.glsl
const float PI05 = 1.5707963267948966192313216916398;
const float PI = 3.1415926535897932384626433832795;
const float PI2 = 6.283185307179586476925286766559;
const float PIRECIP = 0.31830988618379067153776752674503;
const float PIPER180 = 0.01745329251994329576923690768489;
const float SQRT2 = 1.4142135623730950488016887242097;
const float E_NUMBER = 2.7182818284590452353602874713527;
const float LN2 = 0.69314718055994530941723212145817658;

#define EPS 0.0001

const float g_fGridSizeInMeter = 5.0;
const float g_fElapsedTimeInSec = 1.0;
const float g_fAdvectSpeed = -1.0;
const float g_fG = 10.0;
const float g_fHackBlurDepth = 1.0;

#define UI0 1597334673U
#define UI1 3812015801U
#define UI2 uvec2(UI0, UI1)
#define UI3 uvec3(UI0, UI1, 2798796415U)
#define UI4 uvec4(UI3, 1979697957U)
#define UIF (1.0 / float(0xffffffffU))

float
hash12 (vec2 p)
{
  uvec2 q = uvec2(ivec2(p)) * UI2;
  uint n = (q.x ^ q.y) * UI0;
  return float(n) * UIF;
}

vec2
hash (vec2 p)
{
  p = vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
  return -1.0 + 2.0 * fract(sin(p) * 43758.5453123);
}

float
noise (vec2 p)
{
  const float K1 = 0.366025404;
  const float K2 = 0.211324865;

  vec2 i = floor(p + (p.x + p.y) * K1);
  vec2 a = p - i + (i.x + i.y) * K2;
  float m = step(a.y, a.x);
  vec2 o = vec2(m, 1.0 - m);
  vec2 b = a - o + K2;
  vec2 c = a - 1.0 + 2.0 * K2;
  vec3 h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
  vec3 n = h * h * h * h * vec3(dot(a, hash(i + 0.0)), dot(b, hash(i + o)), dot(c, hash(i + 1.0)));
  return dot(n, vec3(70.0));
}

float
SampleDepth (sampler2D sampler, vec2 uv, vec2 vRes)
{
  float fRet = 0.0;
    
  vec2 vUV = uv * vRes.x / vRes.y * 3.0 + 1.0;
  vUV += vec2(noise(vUV + 2.0), noise(vUV + 1.0)) * 0.2;
  mat2 m = mat2(2.0, 1.2, -1.2, 2.0);
  fRet = 0.5000 * noise(vUV) * 2.0;
  vUV = m * vUV;
  fRet += 0.2500 * noise(vUV) * 1.0;
  vUV = m * vUV;
  vUV += vec2(noise(vUV + 0.5), noise(vUV + 2.5)) * 0.2;
  fRet += 0.1250 * noise(vUV) * 1.0;
  vUV = m * vUV;
  fRet += 0.0625 * noise(vUV) * 1.0;
  vUV = m * vUV;
  fRet += 0.0312 * noise(vUV) * 1.0;
  vUV = m * vUV;

  fRet = fRet * 0.6 - 0.4;
  fRet -= (uv.x - 0.6) * 3.0;

  return fRet;
}

float
SampleColor (sampler2D sampler, vec2 uv, vec2 vRes)
{
  float fRet = 0.0;

  vec2 vUV = uv * vRes.x / vRes.y * 3.0 + 1.0;
  vUV += vec2(noise(vUV + 2.0), noise(vUV + 1.0)) * 0.2;
  mat2 m = mat2(2.0, 1.2, -1.2, 2.0);
  fRet = 0.0;
  vUV = m * vUV;
  fRet += 0.0;
  vUV = m * vUV;
  vUV += vec2(noise(vUV + 0.5), noise(vUV + 2.5)) * 0.2;
  fRet += 0.1250 * noise(vUV) * 0.5;
  vUV = m * vUV;
  fRet += 0.0625 * noise(vUV) * 0.4;
  vUV = m * vUV;
  fRet += 0.0312 * noise(vUV) * 0.4;
  vUV = m * vUV;

  return -fRet * 4.0;
}

#define Gamma( v ) pow( v, vec3( 2.2, 2.2, 2.2 ) )
#define DeGamma( v ) pow( v, vec3( 1.0/2.2, 1.0/2.2, 1.0/2.2 ) )
// glut_765_common.glsl

uniform vec2 iResolution;
uniform sampler2D iChannel0;

void
main ()
{
  ivec2 tc = ivec2(gl_FragCoord.xy);
  vec4 vTexC = texelFetch(iChannel0, tc, 0);

  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  float dt = -g_fAdvectSpeed * g_fElapsedTimeInSec / g_fGridSizeInMeter;
  vec2 v1 = vTexC.xy;
  vec2 v2 = textureLod(iChannel0, uv - (0.5 * v1 * dt) / iResolution.xy, 0.0).xy;
  vec2 v3 = textureLod(iChannel0, uv - (0.5 * v2 * dt) / iResolution.xy, 0.0).xy;
  vec2 v4 = textureLod(iChannel0, uv - (v3 * dt) / iResolution.xy, 0.0).xy;
  vec2 v = (1.0 * v1 + 2.0 * v2 + 2.0 * v3 + 1.0 * v4) / 6.0;

  vec4 vTex = textureLod(iChannel0, uv + (v * dt) / iResolution.xy, 0.0);

  gl_FragColor = vTexC;
}
