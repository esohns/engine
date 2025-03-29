#version 130

// glut_580_common.glsl
const int textureSize = 256;
const vec3 backgroundColor = vec3(0.2);
const float transitionTime = 5.0;
const float transitionPercent = 0.3;
const int octaves = 7;
const float attenuation = 0.995;
const float strenght = 0.25;
const float minTotalFlow = 0.0001;
const float initialWaterLevel = 0.05;

mat2
rot (float ang)
{
   return mat2(
      cos(ang), -sin(ang),
      sin(ang),  cos(ang));
}

float
hash12 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * .1031);
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.x + p3.y) * p3.z);
}

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * .1031);
  p3 += dot(p3, p3.zyx + 31.32);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
boxIntersection (vec3 ro, vec3 rd, vec3 rad, out vec3 oN)
{
  vec3 m = 1.0 / rd;
  vec3 n = m * ro;
  vec3 k = abs(m) * rad;
  vec3 t1 = -n - k;
  vec3 t2 = -n + k;

  float tN = max( max( t1.x, t1.y ), t1.z );
  float tF = min( min( t2.x, t2.y ), t2.z );

  if( tN > tF || tF < 0.0) return vec2(-1.0); // no intersection

  oN = -sign(rd)*step(t1.yzx, t1.xyz) * step(t1.zxy, t1.xyz);

  return vec2( tN, tF );
}

vec3
applyFog (vec3 rgb, vec3 fogColor, float distance)
{
  float fogAmount = exp( -distance );
  return mix( fogColor, rgb, fogAmount );
}
// glut_580_common.glsl

uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

float
boxNoise (vec2 p, float z)
{
  vec2 fl = floor(p);
  vec2 fr = fract(p);
  fr = smoothstep(0.0, 1.0, fr);    
  float res = mix(mix( hash13(vec3(fl, z)),             hash13(vec3(fl + vec2(1,0), z)),fr.x),
                  mix( hash13(vec3(fl + vec2(0,1), z)), hash13(vec3(fl + vec2(1,1), z)),fr.x),fr.y);
  return res;
}

float
Terrain (vec2 p, float z, int octaveNum)
{
  float a = 1.0;
  float f = .0;
  for (int i = 0; i < octaveNum; i++)
  {
    f += a * boxNoise(p, z);
    a *= 0.45;
    p = 2.0 * rot(radians(41.0)) * p;
  }
  return f;
}

vec2
readHeight (ivec2 p)
{
  p = clamp(p, ivec2(0), ivec2(textureSize - 1));
  return texelFetch(iChannel0, p, 0).xy;
} 

vec4
readOutFlow (ivec2 p)
{
  if(p.x < 0 || p.y < 0 || p.x >= textureSize || p.y >= textureSize)
    return vec4(0);
  return texelFetch(iChannel1, p, 0);
} 

void
main ()
{
  if( max(gl_FragCoord.x, gl_FragCoord.y) > float(textureSize) )
    discard;

  vec2 uv = gl_FragCoord.xy / float(textureSize);
  float t = iTime / transitionTime;
  float terrainElevation = mix(Terrain(uv * 4.0, floor(t), octaves), Terrain(uv * 4.0, floor(t) + 1.0, octaves), smoothstep(1.0 - transitionPercent, 1.0, fract(t))) * 0.5;

  float waterDept = initialWaterLevel;
  if(iFrame != 0)
  {
    ivec2 p = ivec2(gl_FragCoord.xy);
    vec2 height = readHeight(p);
    vec4 OutFlow = texelFetch(iChannel1, p, 0);
    float totalOutFlow = OutFlow.x + OutFlow.y + OutFlow.z + OutFlow.w;
    float totalInFlow = 0.0;
    totalInFlow += readOutFlow(p  + ivec2( 1,  0)).z;
    totalInFlow += readOutFlow(p  + ivec2( 0,  1)).w;
    totalInFlow += readOutFlow(p  + ivec2(-1,  0)).x;
    totalInFlow += readOutFlow(p  + ivec2( 0, -1)).y;
    waterDept = height.y - totalOutFlow + totalInFlow;
  }

  gl_FragColor = vec4(terrainElevation, waterDept, 0, 1);
}
