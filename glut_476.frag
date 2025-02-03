#version 130
//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define D_DEMO_FREE
//#define D_DEMO_SHOW_IMPROVEMENT_FLAT
//#define D_DEMO_SHOW_IMPROVEMENT_NOISE
//#define D_DEMO_SHOW_IMPROVEMENT_FLAT_NOVOLUMETRICSHADOW
//#define D_DEMO_SHOW_IMPROVEMENT_NOISE_NOVOLUMETRICSHADOW

#ifdef D_DEMO_FREE
#define D_FOG_NOISE 1.0
#define D_STRONG_FOG 0.0
#define D_VOLUME_SHADOW_ENABLE 1
#define D_USE_IMPROVE_INTEGRATION 1
#elif defined (D_DEMO_SHOW_IMPROVEMENT_FLAT)
#define D_STRONG_FOG 10.0
#define D_FOG_NOISE 0.0
#define D_VOLUME_SHADOW_ENABLE 1
#elif defined (D_DEMO_SHOW_IMPROVEMENT_NOISE)
#define D_STRONG_FOG 5.0
#define D_FOG_NOISE 1.0
#define D_VOLUME_SHADOW_ENABLE 1
#elif defined (D_DEMO_SHOW_IMPROVEMENT_FLAT_NOVOLUMETRICSHADOW)
#define D_STRONG_FOG 10.0
#define D_FOG_NOISE 0.0
#define D_VOLUME_SHADOW_ENABLE 0
#elif defined (D_DEMO_SHOW_IMPROVEMENT_NOISE_NOVOLUMETRICSHADOW)
#define D_STRONG_FOG 3.0
#define D_FOG_NOISE 1.0
#define D_VOLUME_SHADOW_ENABLE 0
#endif

// Used to control wether transmittance is updated before or after scattering (when not using improved integration)
// If 0 strongly scattering participating media will not be energy conservative
// If 1 participating media will look too dark especially for strong extinction (as compared to what it should be)
// Toggle only visible when not using the improved scattering integration
#define D_UPDATE_TRANS_FIRST 0

#define D_DETAILED_WALLS 1

#define D_MAX_STEP_LENGTH_ENABLE 1

#define LPOS vec3 (20.0 + 15.0 * sin (iTime), 15.0 + 12.0 * cos (iTime), -20.0)
#define LCOL (600.0 * vec3 (1.0, 0.9, 0.5))

float
displacementSimple (vec2 p)
{
  float f;
  f  = 0.5    * textureLod (iChannel0, p, 0.0).x; p = p * 2.0;
  f += 0.25   * textureLod (iChannel0, p, 0.0).x; p = p * 2.0;
  f += 0.125  * textureLod (iChannel0, p, 0.0).x; p = p * 2.0;
  f += 0.0625 * textureLod (iChannel0, p, 0.0).x; p = p * 2.0;
  return f;
}

vec3
getSceneColor (vec3 p, float material)
{
  if (material == 1.0)
    return vec3 (1.0, 0.5, 0.5);
  else if (material == 2.0)
    return vec3 (0.5, 1.0, 0.5);
  else if (material == 3.0)
    return vec3 (0.5, 0.5, 1.0);

  return vec3 (0.0, 0.0, 0.0);
}

float
getClosestDistance (vec3 p, out float material)
{
  float d = 0.0;
#if D_MAX_STEP_LENGTH_ENABLE
  float minD = 1.0;
#else
  float minD = 10000000.0;
#endif
  material = 0.0;

  float yNoise = 0.0;
  float xNoise = 0.0;
  float zNoise = 0.0;
#if D_DETAILED_WALLS
  yNoise = 1.0 * clamp (displacementSimple (p.xz * 0.005), 0.0, 1.0);
  xNoise = 2.0 * clamp (displacementSimple (p.zy * 0.005), 0.0, 1.0);
  zNoise = 0.5 * clamp (displacementSimple (p.xy * 0.01),  0.0, 1.0);
#endif

  d = max (0.0, p.y - yNoise);
  if (d < minD)
  {
    minD = d;
    material = 2.0;
  }

  d = max (0.0, p.x - xNoise);
  if (d < minD)
  {
    minD = d;
    material = 1.0;
  }

  d = max (0.0, 40.0 - p.x - xNoise);
  if (d < minD)
  {
    minD = d;
    material = 1.0;
  }

  d = max (0.0, -p.z - zNoise);
  if (d < minD)
  {
    minD = d;
    material = 3.0;
  }

  return minD;
}

vec3
calcNormal (vec3 pos)
{
  float material = 0.0;
  vec3 eps = vec3 (0.3, 0.0, 0.0);
  return normalize (vec3 (getClosestDistance (pos + eps.xyy, material) - getClosestDistance (pos - eps.xyy, material),
                          getClosestDistance (pos + eps.yxy, material) - getClosestDistance (pos - eps.yxy, material),
                          getClosestDistance (pos + eps.yyx, material) - getClosestDistance (pos - eps.yyx, material)));
}

vec3
evaluateLight (vec3 pos)
{
  vec3 lightPos = LPOS;
  vec3 lightCol = LCOL;
  vec3 L = lightPos - pos;
  return lightCol * 1.0 / dot (L, L);
}

vec3
evaluateLight (vec3 pos, vec3 normal)
{
  vec3 lightPos = LPOS;
  vec3 L = lightPos - pos;
  float distanceToL = length (L);
  vec3 Lnorm = L / distanceToL;
  return max (0.0, dot (normal, Lnorm)) * evaluateLight (pos);
}

void
getParticipatingMedia (out float sigmaS, out float sigmaE, vec3 pos)
{
  float heightFog = 7.0 + D_FOG_NOISE * 3.0 * clamp (displacementSimple (pos.xz * 0.005 + iTime * 0.01), 0.0, 1.0);
  heightFog = 0.3 * clamp ((heightFog - pos.y) * 1.0, 0.0, 1.0);

  const float fogFactor = 1.0 + D_STRONG_FOG * 5.0;

  const float sphereRadius = 5.0;
  float sphereFog = clamp ((sphereRadius - length (pos - vec3 (20.0, 19.0, -17.0))) / sphereRadius, 0.0, 1.0);

  const float constantFog = 0.02;

  sigmaS = constantFog + heightFog * fogFactor + sphereFog;

  const float sigmaA = 0.0;
  sigmaE = max (0.000000001, sigmaA + sigmaS);
}

float
phaseFunction ()
{
  return 1.0 / (4.0 * 3.14);
}

float
volumetricShadow (vec3 from, vec3 to)
{
#if D_VOLUME_SHADOW_ENABLE
  const float numStep = 16.0;
  float shadow = 1.0;
  float sigmaS = 0.0;
  float sigmaE = 0.0;
  float dd = length (to - from) / numStep;
  for (float s = 0.5; s < (numStep - 0.1); s += 1.0)
  {
    vec3 pos = from + (to - from) * (s / numStep);
    getParticipatingMedia (sigmaS, sigmaE, pos);
    shadow *= exp (-sigmaE * dd);
  }
  return shadow;
#else
  return 1.0;
#endif
}

void
traceScene (bool improvedScattering, vec3 rO, vec3 rD, inout vec3 finalPos, inout vec3 normal, inout vec3 albedo, inout vec4 scatTrans)
{
  const int numIter = 100;

  float sigmaS = 0.0;
  float sigmaE = 0.0;

  vec3 lightPos = LPOS;

  float transmittance = 1.0;
  vec3 scatteredLight = vec3 (0.0, 0.0, 0.0);

  float d = 1.0;
  float material = 0.0;
  vec3 p = vec3 (0.0, 0.0, 0.0);
  float dd = 0.0;
  for (int i = 0; i < numIter; ++i)
  {
    vec3 p = rO + d * rD;

    getParticipatingMedia (sigmaS, sigmaE, p);

#ifdef D_DEMO_FREE
    if (D_USE_IMPROVE_INTEGRATION > 0)
#else
    if (improvedScattering)
#endif
    {
      vec3 S = evaluateLight (p) * sigmaS * phaseFunction () * volumetricShadow (p, lightPos);
      vec3 Sint = (S - S * exp (-sigmaE * dd)) / sigmaE;
      scatteredLight += transmittance * Sint;

      transmittance *= exp (-sigmaE * dd);
    }
    else
    {
#if D_UPDATE_TRANS_FIRST
      transmittance *= exp(-sigmaE * dd);
#endif
      scatteredLight += sigmaS * evaluateLight(p) * phaseFunction() * volumetricShadow(p,lightPos) * transmittance * dd;
#if !D_UPDATE_TRANS_FIRST
      transmittance *= exp(-sigmaE * dd);
#endif
    }

    dd = getClosestDistance (p, material);
    if (dd < 0.2)
      break;
    d += dd;
  }

  albedo = getSceneColor (p, material);

  finalPos = rO + d * rD;

  normal = calcNormal (finalPos);

  scatTrans = vec4 (scatteredLight, transmittance);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;

  float hfactor = float (iResolution.y) / float (iResolution.x);
  vec2 uv2 = vec2 (2.0, 2.0 * hfactor) * gl_FragCoord.xy / iResolution.xy - vec2 (1.0, hfactor);

  vec3 camPos = vec3 (20.0, 18.0, -50.0);
  if (iMouse.x + iMouse.y > 0.0)
    camPos += vec3 (0.05, 0.12, 0.0) * (vec3 (iMouse.x, iMouse.y, 0.0) - vec3 (iResolution.xy * 0.5, 0.0));
  vec3 camX = vec3 (1.0, 0.0, 0.0);
  vec3 camY = vec3 (0.0, 1.0, 0.0);
  vec3 camZ = vec3 (0.0, 0.0, 1.0);

  vec3 rO = camPos;
  vec3 rD = normalize (uv2.x * camX + uv2.y * camY + camZ);
  vec3 finalPos = rO;
  vec3 albedo = vec3 (0.0, 0.0, 0.0);
  vec3 normal = vec3 (0.0, 0.0, 0.0);
  vec4 scatTrans = vec4 (0.0, 0.0, 0.0, 0.0);
  traceScene (gl_FragCoord.x > (iResolution.x / 2.0), rO, rD, finalPos, normal, albedo, scatTrans);

  vec3 color = (albedo / 3.14) * evaluateLight (finalPos, normal) * volumetricShadow (finalPos, LPOS);
  color = color * scatTrans.w + scatTrans.xyz;

  color = pow (color, vec3 (1.0 / 2.2));

#ifndef D_DEMO_FREE
  if (abs (gl_FragCoord.x - (iResolution.x * 0.5)) < 0.6)
    color.r = 0.5;
#endif

  gl_FragColor = vec4 (color, 1.0);
}
