#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform vec4 iKeys;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

#define LAST_FRAME iChannel0
#define SKYBOX(v) texture(iChannel1, v).rgb

// ===============================
// Generic Helpers/Constants
// ===============================

#define PI 3.141592653589793
#define TWOPI 6.283185307179586
#define HALFPI 1.570796326794896
#define SQRT2INV 0.7071067811865475

#define POLAR(theta) vec3(cos(theta), 0.0, sin(theta))
#define SPHERICAL(theta, phi) (sin(phi)*POLAR(theta) + vec3(0.0, cos(phi), 0.0))

vec3
refractFix (vec3 I, vec3 N, float eta)
{
  float k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I));
  return k < 0.0
    ? reflect(I, N) // <- 'refract' returns 0 here
    : eta * I - (eta * dot(N, I) + sqrt(k)) * N;
}

vec4
blendOnto (vec4 cFront, vec4 cBehind)
{
  return cFront + (1.0 - cFront.a)*cBehind;
}

vec4
blendOnto (vec4 cFront, vec3 cBehind)
{
  return cFront + (1.0 - cFront.a)*vec4(cBehind, 1.0);
}

#define QID vec4(0.0, 0.0, 0.0, 1.0)

vec4
slerp (vec4 a, vec4 b, float t)
{
  float d = dot(a, b);
  vec4 a2 = a;

  if (d < 0.0)
  {
    d = -d;
    a2 = -a;
  }
  if (d > 0.999)
  {
    return normalize(mix(a2, b, t));
  }

  float theta = acos(d);
  return (sin((1.-t)*theta)*a2 + sin(t*theta)*b) / sin(theta);
}

vec4
qMul (vec4 a, vec4 b)
{
  return vec4(
      a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
      a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
      a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
      a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
  );
}

vec4
qConj (vec4 q)
{
  return vec4(-q.xyz, q.w);
}

vec4
qRot (vec3 nvAxis, float angle)
{
  return vec4(nvAxis*sin(angle*0.5), cos(angle*0.5));
}

mat3
qToMat (vec4 q)
{
  float wx = q.w*q.x, wy = q.w*q.y, wz = q.w*q.z;
  float xx = q.x*q.x, xy = q.x*q.y, xz = q.x*q.z;
  float yy = q.y*q.y, yz = q.y*q.z, zz = q.z*q.z;
  return mat3(
    1. - 2.*(yy + zz),
         2.*(xy + wz),
         2.*(xz - wy),

         2.*(xy - wz),
    1. - 2.*(xx + zz),
         2.*(yz + wx),

         2.*(xz + wy),
         2.*(yz - wx),
    1. - 2.*(xx + yy)
  );
}

struct state
{
  vec3 p;
  vec3 q;
  vec3 v;
  vec3 L;
  vec4 pr;
};

state
readState ()
{
  state s = state(
    vec3(0.0),
    vec3(0.0),
    vec3(0.0, -cos(0.25*PI), sin(0.25*PI)),
    vec3(0.0, 0.5, 0.0),
    QID
  );

  if (iFrame > 0) {
    s.p = texelFetch(LAST_FRAME, ivec2(0, 0), 0).xyz;
    s.q = texelFetch(LAST_FRAME, ivec2(1, 0), 0).xyz;
    s.v = texelFetch(LAST_FRAME, ivec2(2, 0), 0).xyz;
    s.L = texelFetch(LAST_FRAME, ivec2(3, 0), 0).xyz;
    s.pr = texelFetch(LAST_FRAME, ivec2(4, 0), 0);
  }

  return s;
}

void
writeState (state s, vec2 fragCoord, inout vec4 fragColor)
{
  if (abs(fragCoord.y - 0.0-0.5) < 0.5)
  {
    if (abs(fragCoord.x - 0.0-0.5) < 0.5)
    {
      fragColor = vec4(s.p, 1.0);
    }
    else if (abs(fragCoord.x - 1.0-0.5) < 0.5)
    {
      fragColor = vec4(s.q, 1.0);
    }
    else if (abs(fragCoord.x - 2.0-0.5) < 0.5)
    {
      fragColor = vec4(s.v, 1.0);
    }
    else if (abs(fragCoord.x - 3.0-0.5) < 0.5)
    {
      fragColor = vec4(s.L, 1.0);
    }
    else if (abs(fragCoord.x - 4.0-0.5) < 0.5)
    {
      fragColor = s.pr;
    }
  }
}

#define RES iResolution
#define TAN_HALF_FOVY 0.5773502691896257

vec3
nvCamDirFromClip (vec3 nvFw, vec2 clip)
{
  vec3 nvRt = normalize(cross(nvFw, vec3(0.,1.,0.)));
  vec3 nvUp = cross(nvRt, nvFw);
  return normalize(TAN_HALF_FOVY*(clip.x*(RES.x/RES.y)*nvRt + clip.y*nvUp) + nvFw);
}

void
getCamera (state s, vec2 uv, out vec3 camPos, out vec3 nvCamDir)
{
  vec2 mouseAng = vec2(HALFPI*0.75, PI*0.45) + 0.2*vec2(cos(0.5*iTime),sin(0.5*iTime));
  camPos = vec3(0.0, 2.0, 0.0) + 5.0 * SPHERICAL(mouseAng.x, mouseAng.y);

  vec3 lookTarget = mix(vec3(0.0), s.p, 0.05);
  vec3 nvCamFw = normalize(lookTarget - camPos);

  nvCamDir = nvCamDirFromClip(nvCamFw, uv*2. - 1.);
}

void
updateState (inout state s)
{
  if (iMouse.z > 0.5)
  {
    vec2 uvMouse = iMouse.xy / iResolution.xy;
    vec3 camPos;
    vec3 nvCamDir;
    getCamera(s, uvMouse, camPos, nvCamDir);

    float t = -camPos.y/nvCamDir.y;
    if (t > 0.0 && t < 50.0)
    {
      vec3 center = vec3(0.0);
      s.q = camPos + t*nvCamDir;
      float qToCenter = distance(center, s.q);
      if (qToCenter > 5.0)
      {
        s.q = mix(center, s.q, 5.0/qToCenter);
      }
    }
  }

  float tmod = mod(iTime+6.0, 9.0);
  vec4 qr = (
    tmod < 3.0 ? qRot(vec3( SQRT2INV, 0.0, SQRT2INV), 0.75*PI) :
    tmod < 6.0 ? qRot(vec3(-SQRT2INV, 0.0, SQRT2INV), 0.5*PI) :
    QID
  );

  s.p += 0.25*(s.q - s.p);
  s.pr = normalize(slerp(s.pr, qr, 0.075));

  vec3 a = -0.25*(s.q - s.p) + vec3(0.0, -1.0, 0.0);
  mat3 prMatInv = qToMat(qConj(s.pr));
  a = prMatInv*a;

  vec3 T = cross(s.v, a);
  s.L = 0.96*s.L + 0.2*T;

  vec3 w = s.L;
  float ang = 0.25*length(w);
  if (ang > 0.0001)
  {
    mat3 m = qToMat(qRot(normalize(w), ang));
    s.v = normalize(m*s.v);
  }
}

#define BOUNDING_SPHERE_RADIUS 4.0
#define GLASS_THICKNESS 0.25

float
sdfPlane (vec3 planePoint, vec3 nvPlaneN, vec3 p)
{
  return dot(p - planePoint, nvPlaneN);
}

float
sdfInterval (float a, float b, float x)
{
  return abs(x - 0.5*(a+b)) - 0.5*(b-a);
}

float opSubtraction( float d1, float d2 ) { return max(-d1,d2); }
float
opSmoothSubtraction (float d1, float d2, float k)
{
  float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
  return mix( d2, -d1, h ) + k*h*(1.0-h);
}

float
sdfContainer (vec3 p, state s)
{
  vec3 d = abs(p - s.p);
  vec3 d2 = d*d;
  float sdBase = pow(dot(d2,d2), 0.25) - 2.0;

  float sdInner = length(d.xz) - 0.6;

  return opSmoothSubtraction(sdInner, sdBase, 0.5);
}

float
sdfWater (vec3 p, state s)
{
  float dglass = sdfContainer(p, s);
  float depth = -0.25;
  float dsurf = sdfPlane(s.p + depth*s.v, -s.v, p);
  return opSubtraction(opSmoothSubtraction(dsurf, dglass, 0.15), dglass);
}

float
sdfGlass (vec3 p, state s)
{
  float etchDepth = 0.0;
  return sdfInterval(0.0, GLASS_THICKNESS - etchDepth, sdfContainer(p, s));
}

#define SDF_N_EPS 0.005
#define SDF_NORMAL(sdfFn, p, s) \
    normalize(vec3( \
        sdfFn( p+vec3(SDF_N_EPS,0.0,0.0), s ) - sdfFn( p-vec3(SDF_N_EPS,0.0,0.0), s ), \
        sdfFn( p+vec3(0.0,SDF_N_EPS,0.0), s ) - sdfFn( p-vec3(0.0,SDF_N_EPS,0.0), s ), \
        sdfFn( p+vec3(0.0,0.0,SDF_N_EPS), s ) - sdfFn( p-vec3(0.0,0.0,SDF_N_EPS), s )  \
    ))

#define SDF_EPS 0.01
#define DSTEP_ADJUST_EPS 0.02
#define STEPS 80

#define LIGHT_COLOR vec3(1.0)

#define GLASS_COLOR vec3(0.01, 0.0, 0.4)
#define GLASS_OPACITY 0.6
#define WATER_COLOR vec3(1.0)
#define WATER_OPACITY 0.15

#define IR_AIR 1.0
#define IR_GLASS 1.5
#define IR_WATER 1.333

#define SUBSTANCE_AIR 0
#define SUBSTANCE_GLASS 1
#define SUBSTANCE_WATER 2

vec4
computeSpecular (float specularCoefficient, float specularExponent, vec3 nvNormal, vec3 nvFragToLight, vec3 nvFragToCam)
{
  vec3 blinnH = normalize(nvFragToLight + nvFragToCam);
  float valSpecular = pow(max(0.0, dot(nvNormal, blinnH)), specularExponent);
  valSpecular *= specularCoefficient;

  return valSpecular*vec4(LIGHT_COLOR, 1.0);
}

void
march (state s, vec3 pRay, vec3 nvRayIn, out vec4 color, out vec3 nvRayOut)
{
  bool skipOpacity = iKeys.x > 0.5;
  bool skipRefraction = iKeys.y > 0.5;
  bool debugSteps = iKeys.z > 0.5;
  bool debugNormal = iKeys.w > 0.5;

  vec3 pLightO = pRay + vec3(0.0, 10.0, 0.0);

  mat3 prMatInv = qToMat(qConj(s.pr));
  vec3 pCam = prMatInv*(pRay - s.p) + s.p;
  vec3 pLight = prMatInv*(pLightO - s.p) + s.p;

  vec3 pCur = pCam;
  vec3 nvRayCur = prMatInv*nvRayIn;

  color = vec4(0.0);
  int curSubstance = SUBSTANCE_AIR;

  int i=0;
  for (; i<STEPS; i++)
  {
    vec3 centerToCur = pCur - s.p;
    if (
        (length(centerToCur) > BOUNDING_SPHERE_RADIUS) &&
        (dot(nvRayCur, centerToCur) > 0.0)
    ) { break; }

    if (color.a > 0.95) { break; }

    float sdGlass = sdfGlass(pCur, s);
    float sdWater = sdfWater(pCur, s);
    vec3 dpStep = abs(min(sdGlass, sdWater))*nvRayCur;

    vec3 nvGlass = SDF_NORMAL(sdfGlass, pCur, s);
    vec3 nvWater = SDF_NORMAL(sdfWater, pCur, s);

    if (curSubstance == SUBSTANCE_AIR)
    {
      if (sdGlass < SDF_EPS && dot(nvGlass,nvRayCur) < 0.0)
      {
        curSubstance = SUBSTANCE_GLASS;

        vec4 sColor = computeSpecular(0.8, 80.0, nvGlass, normalize(pLight-pCur), normalize(pCam-pCur));
        color = blendOnto(color, sColor);

        float cosHitAngle = clamp(dot(nvGlass, -nvRayCur), 0.0, 1.0);
        float r0 = pow((IR_GLASS-IR_AIR)/(IR_GLASS+IR_AIR), 2.0);
        float valRefl = mix(r0, 1.0, pow(clamp(1.0 - cosHitAngle, 0.0, 1.0), 3.0));

        vec3 nvRefl = reflect(nvRayCur, nvGlass);
        color = blendOnto(color, valRefl*vec4(SKYBOX(nvRefl), 1.0));

        dpStep = sdGlass*nvRayCur;
        dpStep += -DSTEP_ADJUST_EPS*nvGlass;
        if (!skipRefraction)
        {
          nvRayCur = refractFix(nvRayCur, nvGlass, IR_AIR/IR_GLASS);
        }
      }
      else if (sdWater < SDF_EPS && dot(nvWater,nvRayCur) < 0.0)
      {
        curSubstance = SUBSTANCE_WATER;

        vec4 sColor = computeSpecular(1.0, 40.0, nvWater, normalize(pLight-pCur), normalize(pCam-pCur));
        color = blendOnto(color, sColor);

        float cosHitAngle = clamp(dot(nvWater, -nvRayCur), 0.0, 1.0);
        float r0 = pow((IR_WATER-IR_AIR)/(IR_WATER+IR_AIR), 2.0);
        float valRefl = mix(r0, 1.0, pow(clamp(1.0 - cosHitAngle, 0.0, 1.0), 5.0));

        vec3 nvRefl = reflect(nvRayCur, nvWater);
        color = blendOnto(color, valRefl*vec4(SKYBOX(nvRefl), 1.0));

        dpStep = sdWater*nvRayCur;
        dpStep += -DSTEP_ADJUST_EPS*nvWater;
        if (!skipRefraction)
        {
          nvRayCur = refractFix(nvRayCur, nvWater, IR_AIR/IR_WATER);
        }
      }
    }
    else if (curSubstance == SUBSTANCE_GLASS)
    {
      float sdGlassInv = -sdGlass;
      vec3 nvGlassInv = -nvGlass;

      dpStep = abs(sdGlassInv)*nvRayCur;

      if (!skipOpacity)
      {
        color = blendOnto(color, clamp(GLASS_OPACITY*sdGlassInv,0.0,1.0)*vec4(GLASS_COLOR, 1.0));
      }

      if (sdGlassInv < SDF_EPS && dot(nvGlassInv,nvRayCur) < 0.0)
      {
        curSubstance = SUBSTANCE_AIR;

        dpStep = sdGlassInv*nvRayCur;
        dpStep += -DSTEP_ADJUST_EPS*nvGlassInv;
        if (!skipRefraction)
        {
          nvRayCur = refractFix(nvRayCur, nvGlassInv, IR_GLASS/IR_AIR);
        }
      }
    }
    else if (curSubstance == SUBSTANCE_WATER)
    {
      float sdWaterInv = -sdWater;
      vec3 nvWaterInv = -nvWater;

      dpStep = abs(sdWaterInv)*nvRayCur;

      if (!skipOpacity)
      {
        color = blendOnto(color, clamp(WATER_OPACITY*sdWaterInv,0.0,1.0)*vec4(WATER_COLOR, 1.0));
      }

      if (sdWaterInv < SDF_EPS && dot(nvWaterInv,nvRayCur) < 0.0)
      {
        curSubstance = SUBSTANCE_AIR;

        dpStep = sdWaterInv*nvRayCur;
        dpStep += -DSTEP_ADJUST_EPS*nvWaterInv;
        if (!skipRefraction)
        {
          nvRayCur = refractFix(nvRayCur, nvWaterInv, IR_WATER/IR_AIR);
        }
      }
    }
    pCur += dpStep;
  }

  nvRayOut = qToMat(s.pr)*nvRayCur;

  if (debugSteps)
  {
    color = vec4( vec3(float(i)/float(STEPS)), 1.0 );
  }
  else if (debugNormal)
  {
    color = vec4( 0.5 + 0.5*nvRayOut, 1.0 );
  }
}

vec4 
mainColor (vec2 fragCoord, state s)
{
  vec3 camPos;
  vec3 nvCamDir;
  vec2 uv = fragCoord / RES.xy;
  getCamera(s, uv, camPos, nvCamDir);

  vec4 color;
  vec3 nvRayOut;
  march(s, camPos, nvCamDir, color, nvRayOut);

  return blendOnto(color, SKYBOX(nvRayOut));
}

void
main ()
{
  state s = readState();

  gl_FragColor = mainColor(gl_FragCoord.xy, s);
  updateState (s);

  writeState(s, gl_FragCoord.xy, gl_FragColor);
}
