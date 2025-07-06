#version 130

// glut_891_common.glsl
#define PI 3.1415926535

#define SAT(x) clamp(x, 0., 1.)

#define TERRAIN_FREQ .1
#define TERRAIN_HEIGHT 3.
#define HQ_OCTAVES 12
#define MQ_OCTAVES 7

#define CAMERA_NEAR .001
#define CAMERA_FAR 200.
#define CAMERA_FOV 75.
#define CAMERA_HEIGHT 1.6
#define CAMERA_PITCH .15
#define CAMERA_ZOOM -2.
#define CAMERA_DEPTH -1125.

#define FOG_B .3
#define FOG_C .1

#define SUN_INTENSITY 6.66
#define SUN_COLOR vec3(1.2, 1., .6)
#define SKY_COLOR vec3(.25, .5, 1.75)
#define SUN_SPEED .04

#define EARTH_RADIUS 6378100. 
#define CLOUD_BOTTOM 3200.
#define CLOUD_TOP 4800.
#define CLOUD_COVERAGE .48
#define CLOUD_BASE_FREQ .00005
#define CLOUD_DETAIL_FREQ .0015
#define CLOUD_STEPS 18
#define CLOUD_LIGHT_STEPS 6
#define CLOUD_TOP_OFFSET 250.
#define CLOUD_ABSORPTION_TOP 1.8
#define CLOUD_ABSORPTION_BOTTOM 3.6

#define WIND_DIR vec3(.4, .1, 1.)
#define WIND_SPEED 75.

#define CLOUDS_AMBIENT_TOP vec3(1., 1.2, 1.6)
#define CLOUDS_AMBIENT_BOTTOM vec3(.6, .4, .8)

#define BAYER_LIMIT 16
#define BAYER_LIMIT_H 4

const int bayerFilter[BAYER_LIMIT] = int[]
(
   0,  8,  2, 10,
  12,  4, 14,  6,
   3, 11,  1,  9,
  15,  7, 13,  5
);

struct Ray
{
  vec3 origin, direction;   
};
    
float
remap (float x, float a, float b, float c, float d)
{
  return (((x - a) / (b - a)) * (d - c)) + c;
}

float
remap01 (float x, float a, float b)
{
  return ((x - a) / (b - a));   
}

bool
writeToPixel (vec2 fragCoord, int iFrame_)
{
  ivec2 iFragCoord = ivec2(fragCoord);
  int index = iFrame_ % BAYER_LIMIT;
  return (((iFragCoord.x + BAYER_LIMIT_H * iFragCoord.y) % BAYER_LIMIT) == bayerFilter[index]);
}

mat3
getCameraMatrix (vec3 origin, vec3 target)
{
  vec3 lookAt = normalize(target - origin);
  vec3 right = normalize(cross(lookAt, vec3(0., 1., 0.)));
  vec3 up = normalize(cross(right, lookAt));
  return mat3(right, up, -lookAt);
}

Ray
getCameraRay (vec2 uv, float t)
{
  uv *= (CAMERA_FOV / 360.) * PI;
  vec3 origin = vec3(0., CAMERA_HEIGHT, CAMERA_DEPTH);
  vec3 target = vec3(0., origin.y + CAMERA_PITCH,  CAMERA_DEPTH - 1.2);
  mat3 camera = getCameraMatrix(origin, target);
  vec3 direction = normalize(camera * vec3(uv, CAMERA_ZOOM));
  return Ray(origin, direction);
}

vec3
getSun (vec2 mouse, float iTime_)
{
  vec2 sunPos = mouse;
    
  if (mouse.y < -.95)
  {
    sunPos = vec2(cos(mod(iTime_ * SUN_SPEED, PI)) * .7, 0.);
    sunPos.y = 1. - 3.05 * sunPos.x * sunPos.x;
  }

  float sunHeight = (max(0., sunPos.y * .75 + .25));

  return vec3(sunPos, sunHeight);
}

vec3
miePhase (float dist, vec3 sunL)
{
  return max(exp(-pow(dist, .3)) * sunL - .4, 0.);
}

vec3
atmosphericScattering (vec2 uv, vec2 sunPos, bool isSun)
{
  float sunDistance = distance(uv, sunPos);
  float scatterMult = SAT(sunDistance);
  float dist = uv.y;
  dist = (.5 * mix(scatterMult, 1., dist)) / dist;
  vec3 mieScatter = miePhase(sunDistance, vec3(1.)) * SUN_COLOR;
  vec3 color = dist * SKY_COLOR;
  color = max(color, 0.);
  vec3 sun = .0002 / pow(length(uv-sunPos), 1.7) * SUN_COLOR;
    
  color = max(mix(pow(color, .8 - color),
  color / (2. * color + .5 - color * 1.3),
  SAT(sunPos.y * 2.5)), 0.)
  + (isSun ? (sun + mieScatter) : vec3(0.));

  color *=  (pow(1. - scatterMult, 5.) * 10. * SAT(.666 - sunPos.y)) + 1.5;
  float underscatter = distance(sunPos.y, 1.);
  color = mix(color, vec3(0.), SAT(underscatter));

  return color;	
}

#define UI0 1597334673U
#define UI1 3812015801U
#define UI2 uvec2(UI0, UI1)
#define UI3 uvec3(UI0, UI1, 2798796415U)
#define UIF (1. / float(0xffffffffU))

vec3
hash33 (vec3 p)
{
  uvec3 q = uvec3(ivec3(p)) * UI3;
  q = (q.x ^ q.y ^ q.z)*UI3;
  return -1. + 2. * vec3(q) * UIF;
}

float
hash13 (vec3 p)
{
  uvec3 q = uvec3(ivec3(p)) * UI3;
  q *= UI3;
  uint n = (q.x ^ q.y ^ q.z) * UI0;
  return float(n) * UIF;
}

float
hash12 (vec2 p)
{
  uvec2 q = uvec2(ivec2(p)) * UI2;
  uint n = (q.x ^ q.y) * UI0;
  return float(n) * UIF;
}

vec3
valueNoiseDerivative (vec2 x, sampler2D smp)
{
  vec2 f = fract(x);
  vec2 u = f * f * (3. - 2. * f);

#if 1
  ivec2 p = ivec2(floor(x));
  float a = texelFetch(smp, (p + ivec2(0, 0)) & 255, 0).x;
  float b = texelFetch(smp, (p + ivec2(1, 0)) & 255, 0).x;
  float c = texelFetch(smp, (p + ivec2(0, 1)) & 255, 0).x;
  float d = texelFetch(smp, (p + ivec2(1, 1)) & 255, 0).x;
#else    
  vec2 p = floor(x);
  float a = textureLod(smp, (p + vec2(.5, .5)) / 256., 0.).x;
  float b = textureLod(smp, (p + vec2(1.5, .5)) / 256., 0.).x;
  float c = textureLod(smp, (p + vec2(.5, 1.5)) / 256., 0.).x;
  float d = textureLod(smp, (p + vec2(1.5, 1.5)) / 256., 0.).x;
#endif
    
  return vec3(a + (b - a) * u.x + (c - a) * u.y + (a - b - c + d) * u.x * u.y,
              6. * f * (1. - f) * (vec2(b - a, c - a) + (a - b - c + d) * u.yx));
}

float
valueNoise (vec3 x, float freq)
{
  vec3 i = floor(x);
  vec3 f = fract(x);
  f = f * f * (3. - 2. * f);
  
  return mix(mix(mix(hash13(mod(i + vec3(0, 0, 0), freq)),  
                     hash13(mod(i + vec3(1, 0, 0), freq)), f.x),
                 mix(hash13(mod(i + vec3(0, 1, 0), freq)),  
                     hash13(mod(i + vec3(1, 1, 0), freq)), f.x), f.y),
             mix(mix(hash13(mod(i + vec3(0, 0, 1), freq)),  
                     hash13(mod(i + vec3(1, 0, 1), freq)), f.x),
                 mix(hash13(mod(i + vec3(0, 1, 1), freq)),  
                     hash13(mod(i + vec3(1, 1, 1), freq)), f.x), f.y), f.z);
}

float
worleyNoise (vec3 uv, float freq, bool tileable)
{    
  vec3 id = floor(uv);
  vec3 p = fract(uv);
  float minDist = 10000.;
    
  for (float x = -1.; x <= 1.; ++x)
    for(float y = -1.; y <= 1.; ++y)
      for(float z = -1.; z <= 1.; ++z)
      {
        vec3 offset = vec3(x, y, z);
        vec3 h = vec3(0.);
        if (tileable)
          h = hash33(mod(id + offset, vec3(freq))) * .4 + .3;
        else
          h = hash33(id + offset) * .4 + .3;
        h += offset;
        vec3 d = p - h;
        minDist = min(minDist, dot(d, d));
      }
    
  return 1. - minDist;
}

float
perlinFbm (vec3 p, float freq, int octaves)
{
  float G = exp2(-.85);
  float amp = 1.;
  float noise = 0.;
  for (int i = 0; i < octaves; ++i)
  {
    noise += amp * valueNoise(p * freq, freq);
    freq *= 2.;
    amp *= G;
  }
    
  return noise;
}

float
worleyFbm (vec3 p, float freq, bool tileable)
{
  float fbm = worleyNoise(p * freq, freq, tileable) * .625 +
              worleyNoise(p * freq * 2., freq * 2., tileable) * .25 +
              worleyNoise(p * freq * 4., freq * 4., tileable) * .125;
  return max(0., fbm * 1.1 - .1);
}
// glut_891_common.glsl

uniform vec2 iResolution;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

bool
resolutionChanged ()
{
  return int(texelFetch(iChannel1, ivec2(0), 0).r) != int(iResolution.x);
}

void
main ()
{
  if (resolutionChanged ())
  {
    vec2 uv = gl_FragCoord.xy / iResolution;
    vec4 col = vec4(0.);
    col.r += perlinFbm(vec3(uv, .4), 4., 15) * .5;
    col.r = abs(col.r * 2. - 1.);
    col.r = remap(col.r,  worleyFbm(vec3(uv, .2), 4., true) - 1., 1., 0., 1.);
    col.g += worleyFbm(vec3(uv, .5), 8., true) * .625 + 
             worleyFbm(vec3(uv, .5), 16., true) * .25  +
             worleyFbm(vec3(uv, .5), 32., true) * .125;
    col.b = 1. - col.g;
    gl_FragColor = col;
  }
  else
    gl_FragColor = texelFetch(iChannel0, ivec2(gl_FragCoord.xy), 0);
}
