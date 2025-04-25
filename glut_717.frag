#version 130

// glut_717_common.glsl
//#define USE_CINEMATIC_MODE
//#define USE_BUMPY_STREAMS_MODE
//#define USE_GENERATION_SEED 123

const int nParticles = 20;
const float particlesSize = 8.0;
const float collisionDamping = 0.5;
const float streamsFadingExp = 0.001;
const float gravityStrength = 1.6 / particlesSize;

const vec3 ambientLightDir = normalize(vec3(1.0, 2.0, 0.0));
const vec3 ambientLightCol = vec3(1.1, 1.0, 0.9);
const vec3 backgroundColor = vec3(0.65);
const float streamsGlossExp = 120.0;
const float spotlightsGlare = 0.0;

#ifdef USE_BUMPY_STREAMS_MODE
#define particlesSize mix(particlesSize, particlesSize * 0.5, (1.0 + sin(1.85 + iTime * 11.93805208)) * 0.5)
#endif

#ifdef USE_GENERATION_SEED
#define generationSeed float(USE_GENERATION_SEED) // a fixed seed will generate the same output (in respect of the viewport size)
#else
#define generationSeed iDate.w // if no custom seed is provided, POSIX time is used instead (producing different results every time)
#endif

const ivec2 cameraVelocity =
#ifdef USE_CINEMATIC_MODE
ivec2(1, 0);
#else
ivec2(0);
#endif
// glut_717_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iDate;
uniform sampler2D iChannel0;

float
rand (vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec2
randVec2 (vec2 co)
{
  return vec2(rand(co.xy + generationSeed * 0.0001), rand(-co.yx + generationSeed * 0.0001));
}

vec2
randNrm2 (vec2 fragCoord)
{
  vec2 n = vec2(-1.0) + randVec2(fragCoord) * 2.0;
  float l = length(n);
  if(l <= 0.0000001)
    n = vec2(0.0, (l = 1.0));

  return (n / l);
}

void
initParticle (vec2 fragCoord, inout vec2 particlePrevPosition, inout vec2 particleCurrPosition)
{
  particleCurrPosition = randVec2(fragCoord) * iResolution.xy;
  particlePrevPosition = particleCurrPosition - randNrm2(fragCoord) * particlesSize * 0.0625;
}

vec2
getParticlePosition (int particleID)
{
  int iChannel0_width = textureSize (iChannel0, 0).x;
  ivec2 particleCoord = ivec2(particleID % iChannel0_width, particleID / iChannel0_width);

  return texelFetch(iChannel0, particleCoord, 0).xy;
}

vec2
computeGravitation (int particleID, vec2 particlePosition)
{
  vec2 acceleration = vec2(0.0);

  for(int i = 0; i < nParticles; ++i) if(i != particleID)
  {
    vec2 v = (getParticlePosition(i) - particlePosition);
    float d = length(v);

    if(d > 0.0000001)
      acceleration += (v / d) / pow(max(d, particlesSize * 2.0) * gravityStrength, 2.0);
  }

  return acceleration;
}

void
solveCollisions (inout vec2 particlePrevPosition, inout vec2 particleCurrPosition)
{
  vec2 particleInertia = (particleCurrPosition - particlePrevPosition);
    
  if(particleCurrPosition.x < particlesSize || particleCurrPosition.x > iResolution.x - particlesSize)
  {
    particleCurrPosition.x = clamp(particleCurrPosition.x, particlesSize, iResolution.x - particlesSize);
    particlePrevPosition.x = particleCurrPosition.x + particleInertia.x * collisionDamping;
  }

  if(particleCurrPosition.y < particlesSize || particleCurrPosition.y > iResolution.y - particlesSize)
  {
    particleCurrPosition.y = clamp(particleCurrPosition.y, particlesSize, iResolution.y - particlesSize);
    particlePrevPosition.y = particleCurrPosition.y + particleInertia.y * collisionDamping;
  }
}

void
main ()
{
  int particleID = int(floor(gl_FragCoord.x) + iResolution.x * floor(gl_FragCoord.y));
  if(particleID >= nParticles)
    return;
    
  vec4 particleData = texelFetch(iChannel0, ivec2(gl_FragCoord.xy), 0);
  vec2 particlePrevPosition = particleData.zw;
  vec2 particleCurrPosition = particleData.xy;

  if(iFrame == 0)
    initParticle(gl_FragCoord.xy, particlePrevPosition, particleCurrPosition);

  vec2 particleAcceleration = computeGravitation(particleID, particleCurrPosition);
  vec2 particleInertia = particleCurrPosition - particlePrevPosition;
  vec2 particleVelocity = particleInertia + particleAcceleration;

  particlePrevPosition = particleCurrPosition;
  particleCurrPosition += particleVelocity;

  solveCollisions(particlePrevPosition, particleCurrPosition);

  gl_FragColor = vec4(particleCurrPosition, particlePrevPosition);
}
