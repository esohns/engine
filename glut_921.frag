#version 130

// glut_921_common.glsl
#define FREQ_STEP (0.001953125 * 3.0)

#define PI 3.1415927
#define TWO_PI 6.283185

#define BLUR_STRENGTH 1.0
#define BLUR_RANGE 2.5

float
pow3 (float x)
{
  return x*x*x;   
}

float
hash1_2 (vec2 x)
{
  return fract(sin(dot(x, vec2(52.127, 61.2871))) * 521.582);   
}

vec2
hash2_3 (vec3 x)
{
  return fract(sin(x * mat2x3(20.5283, 24.1994, 70.2913, 
                              89.9132, 57.1454, 45.1211)) * 492.194);
}
                 
vec2
noise2_3 (vec3 coord)
{
  vec3 f = smoothstep(0.0, 1.0, fract(coord));
  
  vec3 uv000 = floor(coord);
  vec3 uv001 = uv000 + vec3(0,0,1);
  vec3 uv010 = uv000 + vec3(0,1,0);
  vec3 uv011 = uv000 + vec3(0,1,1);
  vec3 uv100 = uv000 + vec3(1,0,0);
  vec3 uv101 = uv000 + vec3(1,0,1);
  vec3 uv110 = uv000 + vec3(1,1,0);
  vec3 uv111 = uv000 + vec3(1,1,1);
    
  vec2 v000 = hash2_3(uv000);
  vec2 v001 = hash2_3(uv001);
  vec2 v010 = hash2_3(uv010);
  vec2 v011 = hash2_3(uv011);
  vec2 v100 = hash2_3(uv100);
  vec2 v101 = hash2_3(uv101);
  vec2 v110 = hash2_3(uv110);
  vec2 v111 = hash2_3(uv111);
    
  vec2 v00 = mix(v000, v001, f.z);
  vec2 v01 = mix(v010, v011, f.z);
  vec2 v10 = mix(v100, v101, f.z);
  vec2 v11 = mix(v110, v111, f.z);
    
  vec2 v0 = mix(v00, v01, f.y);
  vec2 v1 = mix(v10, v11, f.y);
  vec2 v = mix(v0, v1, f.x);
    
  return v;
}

float
noise1_2 (vec2 uv)
{
  vec2 f = fract(uv);
    
  vec2 uv00 = floor(uv);
  vec2 uv01 = uv00 + vec2(0,1);
  vec2 uv10 = uv00 + vec2(1,0);
  vec2 uv11 = uv00 + 1.0;
    
  float v00 = hash1_2(uv00);
  float v01 = hash1_2(uv01);
  float v10 = hash1_2(uv10);
  float v11 = hash1_2(uv11);
    
  float v0 = mix(v00, v01, f.y);
  float v1 = mix(v10, v11, f.y);
  float v = mix(v0, v1, f.x);
    
  return v;
}

vec4
blur (sampler2D sampler, vec2 fragCoord, vec2 resolution)
{
  vec2 uv = fragCoord / resolution;

  float blurStrength = distance(uv, vec2(0.5));
  blurStrength = pow(blurStrength, BLUR_RANGE) * (resolution.x / 100.0) * BLUR_STRENGTH;

  vec4 sum = vec4(0.0);
  vec2 pixelSize = vec2(1.0) / resolution;
  for (float x = -1.0; x <= 1.0; x += 1.0)
    for (float y = -1.0; y <= 1.0; y += 1.0)
       sum += texture(sampler, uv + vec2(x, y) * pixelSize * blurStrength);

  return sum / 9.0;
}
// glut_921_common.glsl

uniform vec2 iResolution;
uniform float iTime;

#define RANDOMIZED_SIZE
#define PARTICLE_ITERATIONS 6.0

#define PARTICLE_RADIUS 0.15
#define PARTICLE_RADIUS2 0.3
#define PARTICLE_SIZE_VARIATION 0.2
#define PARTICLE_COLOR vec3(0.3, 0.9, 0.9) * 1.0
#define BRIGHTNESS 0.45

vec2
cellPointFromRootUV (vec2 rootUV, vec2 originalUV, out float len)
{
  vec2 displacement = (noise2_3(vec3(rootUV * 0.07 + iTime * 0.3, 0.5 * (iTime + 0.1) * 1.0 + noise1_2(originalUV * 0.04))) - 0.5);
  len = dot(displacement, displacement);
  return displacement * 3.0 * (PARTICLE_ITERATIONS) + 0.5 + rootUV;   
}

float
particleFromUVAndPoint (vec2 uv, vec2 point_, vec2 rootUV, float pixelSize)
{
  float dist = distance(uv, point_);
#ifdef RANDOMIZED_SIZE
  dist += (hash1_2(rootUV * 10.0) - 0.5) * PARTICLE_SIZE_VARIATION;
#endif
  float particle = 1.0 - smoothstep(PARTICLE_RADIUS - dist * 0.05, PARTICLE_RADIUS2 - dist * 0.05 + pixelSize, dist);
  return particle * particle;
}

vec3
surfaceParticles (vec2 uv, float pixelSize)
{
  vec3 particles = vec3(0.0);
  vec2 rootUV = floor(uv);

  vec2 tempRootUV;
  vec2 pointUV;
  float dist;
  vec3 color;
  for (float x = -PARTICLE_ITERATIONS; x <= PARTICLE_ITERATIONS; x += 1.0)
    for (float y = -PARTICLE_ITERATIONS; y <= PARTICLE_ITERATIONS; y += 1.0)
    {
      tempRootUV = rootUV + vec2(x, y);
      pointUV = cellPointFromRootUV(tempRootUV, uv, dist);
      color = mix(vec3(0), PARTICLE_COLOR, pow(smoothstep(0.3, 0.0, dist), 4.0));
      particles += particleFromUVAndPoint(uv, pointUV, tempRootUV, pixelSize) * color;
    }
    
  return particles;
}

void
main ()
{
  vec2 uv = (2.0 * gl_FragCoord.xy - iResolution) / iResolution.x;
    
  float vignette = 1.0 - smoothstep(0.5, 1.3, length(uv* vec2(1.0, iResolution.x / iResolution.y)));

  float pixelSize = 1.5 / iResolution.x;	

  uv *= 70.0;
  pixelSize *= 70.0;

  vec3 particles = surfaceParticles(uv, pixelSize) * BRIGHTNESS;

  particles = smoothstep(-0.2, 0.8, particles * vignette);

  gl_FragColor = vec4(particles, 1.0);
}
