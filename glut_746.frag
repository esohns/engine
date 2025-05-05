#version 130

// glut_746_common.glsl
#define PI 3.141592653589793
#define TWOPI 6.283185307179586
#define PI_OVER_2 1.570796326794896
#define PI_OVER_4 0.7853981633974483

#define SQRT2 1.414213562373095
#define INV_SQRT2 0.7071067811865476

float
hash12 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * .1031);
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.x + p3.y) * p3.z);
}

float
unmix (float a, float b, float x)
{
  return (x - a) / (b - a);
}

float
isInInterval (float a, float b, float x)
{
  return step(a, x) * (1.0 - step(b, x));
}

vec2
paramUnitSquare (float t)
{
  float tmod4 = mod(t, 4.0);
  return vec2(
    clamp(2.0 * abs(tmod4 - 2.5) - 2.0, -1.0, 1.0),
    clamp(2.0 - 2.0 * abs(tmod4 - 1.5), -1.0, 1.0)
  );
}

mat3
oProd (vec3 n)
{
  float xy = n.x * n.y, xz = n.x * n.z, yz = n.y * n.z;
  return mat3(n.x * n.x, xy, xz, xy, n.y * n.y, yz, xz, yz, n.z * n.z);
}

mat3
glRotate (vec3 axis, float angle)
{
  float c = cos(angle), s = sin(angle);
  vec3 n = normalize(axis);
  return (
          (1. - c) * oProd(n) +
          mat3(c, s * n.z, -s * n.y, -s * n.z, c, s * n.x, s * n.y, -s * n.x, c)
    );
}

vec4
blendOnto (vec4 cFront, vec4 cBehind)
{
  return cFront + (1.0 - cFront.a) * cBehind;
}

vec4
blendOnto (vec4 cFront, vec3 cBehind)
{
  return cFront + (1.0 - cFront.a) * vec4(cBehind, 1.0);
}

#define DITHER
vec2
packColor (vec2 fragCoord, vec3 c24)
{
  float r = (c24.r == 1.0) ? 31.0 : floor(c24.r * 32.0);
  float g = (c24.g == 1.0) ? 63.0 : floor(c24.g * 64.0);
  float b = (c24.b == 1.0) ? 31.0 : floor(c24.b * 32.0);

#ifdef DITHER
  float tr = fract(c24.r * 32.0);
  float tg = fract(c24.g * 64.0);
  float tb = fract(c24.b * 32.0);

  float tarr[16];
  tarr[0] = 1.0;
  tarr[1] = 9.0;
  tarr[2] = 3.0;
  tarr[3] = 11.0;
  tarr[4] = 13.0;
  tarr[5] = 5.0;
  tarr[6] = 15.0;
  tarr[7] = 7.0;
  tarr[8] = 4.0;
  tarr[9] = 12.0;
  tarr[10] = 2.0;
  tarr[11] = 10.0;
  tarr[12] = 16.0;
  tarr[13] = 8.0;
  tarr[14] = 14.0;
  tarr[15] = 6.0;

  int tci = int(mod(fragCoord.x, 4.0));
  int tcj = int(mod(fragCoord.y, 4.0));

  float thresh = 1.0;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      if ((i == tci) && (j == tcj))
      {
        thresh = tarr[i * 4 + j] / 17.0;
      }
    }
  }

  if (tr > thresh)
  {
    r = min(r + 1.0, 31.0);
  }
  if (tg > thresh)
  {
    g = min(g + 1.0, 63.0);
  }
  if (tb > thresh)
  {
    b = min(b + 1.0, 31.0);
  }
#endif

  float gmin = mod(g, 8.0);
  float gmax = floor(g / 8.0);

  float x = (r + 32.0 * gmin) / 255.0;
  float y = (b + 32.0 * gmax) / 255.0;

  return vec2(x, y);
}

vec3
unpackColor (vec2 c16)
{
  float rpack = floor(255.0 * c16.x);
  float bpack = floor(255.0 * c16.y);

  float gmin = floor(rpack / 32.0);
  float gmax = floor(bpack / 32.0);
  float g = (gmin + 8.0 * gmax) / 63.0;

  float r = mod(rpack, 32.0) / 31.0;
  float b = mod(bpack, 32.0) / 31.0;

  return vec3(r, g, b);
}

float
sdSubtract (float d1, float d2)
{
  return max(-d1, d2);
}

float
sdBox (vec3 boxCenter, vec3 boxRadii, vec3 p)
{
  vec3 q = boxRadii - abs(p - boxCenter);
  return length(min(q, 0.0)) - max(min(min(q.x, q.y), q.z), 0.0);
}

float
hitIntervalFromInside (float xMin, float xMax, float x, float dx)
{
  float tMin = (xMin - x) / dx;
  float tMax = (xMax - x) / dx;
  return max(tMin, tMax);
}

float
hitBox2FromInside (vec2 boxMin, vec2 boxMax, vec2 p, vec2 v)
{
  return min(
    hitIntervalFromInside(boxMin.x, boxMax.x, p.x, v.x),
    hitIntervalFromInside(boxMin.y, boxMax.y, p.y, v.y)
  );
}

void
hitBox2FromOutside (vec2 boxMin, vec2 boxMax, vec2 p, vec2 v, out float t, out float didHit)
{
  vec2 tb0 = (boxMin - p) / v;
  vec2 tb1 = (boxMax - p) / v;
  vec2 tmin = min(tb0, tb1);
  vec2 tmax = max(tb0, tb1);

  vec2 tRange = vec2(
    max(tmin.x, tmin.y),
    min(tmax.x, tmax.y)
  );

  didHit = step(tRange.s, tRange.t) * step(0.0, tRange.s);
  t = tRange.s;
}

float
minPointLine (vec3 a, vec3 nv, vec3 p)
{
  return dot(p - a, nv);
}

vec2
minLineLine (vec3 a, vec3 nv, vec3 b, vec3 nw)
{
  vec3 d = a - b;
  float vw = dot(nv, nw);
  float dv = dot(d, nv);
  float dw = dot(d, nw);
  return vec2(dv - dw * vw, -dw + dv * vw) / (vw * vw - 1.0);
}

float
dsq (vec3 p, vec3 q)
{
  vec3 d = p - q;
  return dot(d, d);
}

float
dsqPointLine (vec3 a, vec3 p, vec3 nv)
{
  float tMinA = minPointLine(p, nv, a);
  return dsq(a, p + tMinA * nv);
}

float
dsqPointRay (vec3 a, vec3 p, vec3 nv)
{
  float tMinA = max(0.0, minPointLine(p, nv, a));
  return dsq(a, p + tMinA * nv);
}

float
dsqSegmentLine (vec3 a, vec3 b, vec3 p, vec3 nv)
{
  float tMinA = minPointLine(p, nv, a);
  float tMinB = minPointLine(p, nv, b);
  float dsqMin = min(
        dsq(a, p + tMinA * nv),
        dsq(b, p + tMinB * nv)
    );

  float lenSeg = length(a - b);
  vec3 nvSeg = (a - b) / lenSeg;

  vec2 minInterior = minLineLine(b, nvSeg, p, nv);
  if (minInterior.s > 0.0 && minInterior.s < lenSeg)
  {
    vec3 cpSeg = b + minInterior.s * nvSeg;
    vec3 cpLine = p + minInterior.t * nv;
    dsqMin = min(dsqMin, dsq(cpSeg, cpLine));
  }

  return dsqMin;
}

float
dsqSegmentRay (vec3 a, vec3 b, vec3 p, vec3 nv)
{
  float tMinA = max(0.0, minPointLine(p, nv, a));
  float tMinB = max(0.0, minPointLine(p, nv, b));
  float dsqMin = min(
    dsq(a, p + tMinA * nv),
    dsq(b, p + tMinB * nv)
  );

  float lenSeg = length(a - b);
  vec3 nvSeg = (a - b) / lenSeg;

  float sMinP = clamp(minPointLine(b, nvSeg, p), 0.0, lenSeg);
  dsqMin = min(dsqMin, dsq(p, b + sMinP * nvSeg));

  vec2 minInterior = minLineLine(b, nvSeg, p, nv);
  if (minInterior.s > 0.0 && minInterior.s < lenSeg && minInterior.t > 0.0)
  {
    vec3 cpSeg = b + minInterior.s * nvSeg;
    vec3 cpLine = p + minInterior.t * nv;
    dsqMin = min(dsqMin, dsq(cpSeg, cpLine));
  }

  return dsqMin;
}
// glut_746_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform bvec4 iKeyboard; // s, d, f
uniform sampler2D iChannel0;

#define PILLAR_COLOR vec3(0.5, 0.1, 0.0)
#define TERRAIN_COLOR vec3(1.0, 0.5, 0.2)
#define LIGHT_COLOR vec3(1.0, 0.7, 0.5)
#define GLOW_COLOR vec3(0.2, 0.5, 1.0)

vec3
pal (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos(TWOPI * (c * t + d));
}

vec3
colormapPhi (float t)
{
  return pal(t, vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.10, 0.20));
}

vec3
colormapTerrainGrass (float h)
{
  return mix(vec3(0.3, 0.2, 0.1), vec3(0.3, 0.6, 0.2), h);
}

vec3
colormapTerrainStone (float h)
{
  return mix(vec3(0.4), vec3(0.7, 0.6, 0.5), h);
}

vec4
glowPremulFromDsq (float dsq)
{
  float glow = 1.0 / (1.0 + 500.0 * dsq);
  vec3 glowColor = mix(GLOW_COLOR, vec3(1.0), smoothstep(0.0, 1.0, glow));
  return glow * vec4(glowColor, 1.0);
}

vec4
skyboxLocal (vec2 phiTheta)
{
  vec2 phiThetaT = vec2(phiTheta.s + 0.05 * iTime, phiTheta.t);
  float a = textureLod(iChannel0, phiThetaT * vec2(1.0, 3.0), 0.0).r;
  a = 0.75 * a * a;
  return a * vec4(vec3(1.0), 1.0);
}

vec3
skyboxGlobal (vec3 nvDir)
{
  float t = abs(nvDir.y);
  return colormapPhi(t * (1.0 - 0.5 * t));
}

#define MAT_SKY 0
#define MAT_PILLAR 1
#define MAT_TERRAIN 2
#define MAT_WATER 3

void
computeLighting(
    int matId,
    vec3 lightColor,
    vec3 texColor,
    vec3 nvNormal,
    vec3 nvFragToLight,
    vec3 nvFragToCam,
    out vec3 litColor
)
{
  float ambientCoefficient = 0.0;
  float diffuseCoefficient = 0.0;
  float specularCoefficient = 0.0;
  float specularExponent = 1.0;
  switch (matId)
  {
    case MAT_PILLAR:{
        ambientCoefficient = 0.0;
        diffuseCoefficient = 0.9;
        specularCoefficient = 0.9;
        specularExponent = 20.0;
        break;
      }
    case MAT_TERRAIN:{
        ambientCoefficient = 0.0;
        diffuseCoefficient = 0.9;
        specularCoefficient = 0.15;
        specularExponent = 10.0;
        break;
      }
    case MAT_WATER:{
        ambientCoefficient = 0.3;
        diffuseCoefficient = 0.5;
        specularCoefficient = 0.6;
        specularExponent = 50.0;
        break;
      }
  }

  float valDiffuse = max(0.0, dot(nvNormal, nvFragToLight));
  valDiffuse *= diffuseCoefficient;

  vec3 blinnH = normalize(nvFragToLight + nvFragToCam);
  float valSpecular = pow(max(0.0, dot(nvNormal, blinnH)), specularExponent);
  valSpecular *= specularCoefficient;

  litColor = clamp((ambientCoefficient + valDiffuse) * texColor * lightColor + valSpecular * lightColor, 0.0, 1.0);
}

#define EPS 1e-4
#define T_MAX 25.0

#define SDF_STEPS 50
#define SDF_EPS 5e-3
#define SDF_N_EPS 0.015

#define TERRAIN_POM_DEPTH 0.15
#define TERRAIN_POM_STEPS 16
#define TERRAIN_N_EPS 0.01

#define HALF_WIDTH 0.5
#define HALF_INNER_DIAM 1.0
#define HALF_OUTER_DIAM 2.4

#define HALF_DEPTH 1.0
#define PILLAR_RADIUS 0.05
#define ARCH_B_VERT_OFFSET (0.25*HALF_DEPTH)
#define LASER_Y (-HALF_DEPTH + 1.5*ARCH_B_VERT_OFFSET)

#define ID_NOTHING 0
#define ID_PORTAL_LOOP 1
#define ID_PORTAL_SIDE 2
#define ID_PORTAL_INNER 3
#define ID_PORTAL_OUTER 4
#define ID_PILLAR 5
#define ID_PLANAR 6

void
updateHit (inout float tMin, inout int idMin, float t, int id)
{
  if (t < tMin)
  {
    tMin = t;
    idMin = id;
  }
}

#define _SDF_NORMAL(sdfFn, p) \
    normalize(vec3( \
        sdfFn( p+vec3(SDF_N_EPS,0.0,0.0) ) - sdfFn( p-vec3(SDF_N_EPS,0.0,0.0) ), \
        sdfFn( p+vec3(0.0,SDF_N_EPS,0.0) ) - sdfFn( p-vec3(0.0,SDF_N_EPS,0.0) ), \
        sdfFn( p+vec3(0.0,0.0,SDF_N_EPS) ) - sdfFn( p-vec3(0.0,0.0,SDF_N_EPS) )  \
    ))

#define _SDF_HIT(sdfFn, p, pDist, shadowAmount) \
	float tPillar = 0.0; \
    float didHitPillar = 0.0; \
    vec3 pSdf = p; \
    for (int i=0; i<SDF_STEPS; i++) { \
        float d = sdfFn(pSdf); \
        float curShadowAmount = clamp(1.0 - abs(d)/( 0.015 + 0.04*(pDist+tPillar) ), 0.0, 1.0); \
        shadowAmount = max(shadowAmount, curShadowAmount); \
        if (abs(d) < SDF_EPS || tPillar > t) { \
            didHitPillar = 1.0; \
            break; \
        } \
        tPillar += d; \
        pSdf += d * nv; \
    } \
    if (didHitPillar > 0.5) { \
        updateHit(t, id, tPillar, ID_PILLAR); \
    }

#define _DSQ_POINT_FACE(iFace, p) \
	(iFace == 0 ? dsq0Point(p) : \
     iFace == 1 ? dsq1Point(p) : \
     iFace == 2 ? dsq2Point(p) : \
                  dsq3Point(p) )

#define _DSQ_NORMAL(iFace, p) \
	(iFace == 0 ? _SDF_NORMAL(dsq0Point, p) : \
     iFace == 1 ? _SDF_NORMAL(dsq1Point, p) : \
     iFace == 2 ? _SDF_NORMAL(dsq2Point, p) : \
                  _SDF_NORMAL(dsq3Point, p) )

#define _UV_FACE(iFace, p) \
	(iFace == 0 ? uvFace0(p) : \
     iFace == 1 ? uvFace1(p) : \
     iFace == 2 ? uvFace2(p) : \
                  uvFace3(p) )

#define _DPHI_FACE(iFace, p) \
	(iFace == 0 ? vec2(1.0, 0.0) : \
     iFace == 1 ? ( abs(p.x) > abs(p.z) ? vec2(-sign(p.x), 0.0) : vec2(0.0, -sign(p.z)) ) : \
     iFace == 2 ? vec2(1.0, 0.0) : \
                  ( abs(p.x) > abs(p.z) ? vec2( sign(p.x), 0.0) : vec2(0.0,  sign(p.z)) ) )

float
floorH (int face, vec2 xz)
{
  switch (face)
  {
    case 1:
    case 3:
    {
      return textureLod(iChannel0, xz * 0.15, 0.0).g;
    }
    case 0:
    case 2:
    {
      float loopdist = face == 0 ? (4.0 * HALF_OUTER_DIAM) : (8.0 * HALF_INNER_DIAM);
      vec2 xzSlow = (xz + vec2(0.0, iTime * 0.66)) / loopdist;
      vec2 xzFast = (xz + vec2(0.0, iTime * 1.00)) / loopdist + vec2(0.4, 0.0);

      float hFast = textureLod(iChannel0, xzFast, 0.0).g;
      float h = textureLod(iChannel0, xzSlow + 0.05 * vec2(0.0, hFast), 0.0).g;

      float ts = smoothstep(0.0, 0.8, abs(xz.x) / HALF_WIDTH);
      return mix(h, hFast, mix(0.5, 0.2, ts));
    }
  }

  return 1.0;
}

float
sdPillar (float height, float halfLen, float zCenter, vec3 p)
{
  float baseHeight = PILLAR_RADIUS * 0.8;
  float baseRadius = PILLAR_RADIUS * 1.25;

  float dBase = sdBox(
        vec3(halfLen, -HALF_DEPTH + baseHeight * 0.5 - 10.0, zCenter),
        vec3(baseRadius, baseHeight * 0.5 + 10.0, baseRadius),
        p
    );
  float dTop = sdBox(
        vec3(halfLen, -HALF_DEPTH + height - baseHeight * 0.5, zCenter),
        vec3(baseRadius, baseHeight * 0.5, baseRadius),
        p
    );
  float dCol = distance(p.xz, vec2(halfLen, zCenter)) - PILLAR_RADIUS;
  return min(dCol, min(dBase, dTop));
}

float
sdArchA (float halfLen, float zCenter, vec3 p)
{
  float baseHeight = PILLAR_RADIUS * 0.0;
  float baseRadius = PILLAR_RADIUS * 0.8;

  float pillarBaseY = baseHeight;
  float dVert = sdPillar(2.0 * HALF_DEPTH - pillarBaseY, halfLen, zCenter, vec3(p.x, p.y - pillarBaseY, p.z));

  float dArchBase = sdBox(
        vec3(0.0, -HALF_DEPTH + baseHeight * 0.5 - 10.0, zCenter),
        vec3(halfLen, baseHeight * 0.5 + 10.0, baseRadius),
        p
    );

  return min(dVert, dArchBase);
}

float
sdArchB (float halfLen, float zCenter, vec3 p)
{
  float topHeight = PILLAR_RADIUS * 0.8;
  float baseHeight = ARCH_B_VERT_OFFSET;

  float baseRadius = PILLAR_RADIUS * 0.8;
  float topRadius = PILLAR_RADIUS * 4.25;

  float archHeight = HALF_INNER_DIAM;
  float archCenterY = HALF_DEPTH - 0.5 * archHeight;

  float pillarBaseY = baseHeight;
  float dVert = sdPillar(2.0 * HALF_DEPTH - archHeight - pillarBaseY, halfLen, zCenter, vec3(p.x, p.y - pillarBaseY, p.z));

  float dArchBase = sdBox(
        vec3(0.0, -HALF_DEPTH + baseHeight * 0.5 - 10.0, zCenter),
        vec3(halfLen + baseRadius, baseHeight * 0.5 + 10.0, baseRadius),
        p
    );
  float dArchTop = sdBox(
        vec3(0.0, HALF_DEPTH - topHeight * 0.5, zCenter),
        vec3(halfLen + topRadius, topHeight * 0.5, topRadius),
        p
    );

  float dSoFar = min(dVert, min(dArchBase, dArchTop));

  float dArchBody = sdBox(
        vec3(0.0, archCenterY, zCenter),
        vec3(halfLen + PILLAR_RADIUS, archHeight * 0.5, PILLAR_RADIUS),
        p
    );
  float rArchSubY = archHeight - topHeight * 2.0;
  float rArchSubX = halfLen - PILLAR_RADIUS;
  float dArchSub = distance(
        vec2(0.0, archCenterY - archHeight * 0.5),
        vec2(p.x * (rArchSubY / rArchSubX), p.y)
    ) - rArchSubY;

  float dDecor = sdSubtract(dArchSub, dArchBody);

  return min(dSoFar, dDecor);
}

#define G12_P vec3(-0.5, LASER_Y,  1.5)
#define G12_V vec3( INV_SQRT2, 0.0, -INV_SQRT2)
#define G08_P vec3(-0.5, LASER_Y,  9.1)
#define G08_V vec3( INV_SQRT2, 0.0, -INV_SQRT2)
#define G06_P vec3( 0.5, LASER_Y, 11.1)
#define G06_V vec3(-INV_SQRT2, 0.0, -INV_SQRT2)
#define G02_P vec3( 0.5, LASER_Y, 18.7)
#define G02_V vec3(-INV_SQRT2, 0.0, -INV_SQRT2)

float
dsq0Point (vec3 p)
{
  float dsq12 = dsqPointLine(p, G12_P, G12_V);
  float dsq08 = dsqPointLine(p, G08_P, G08_V);
  float dsq06 = dsqPointLine(p, G06_P, G06_V);
  float dsq02 = dsqPointLine(p, G02_P, G02_V);
  return min(min(dsq12, dsq08), min(dsq02, dsq06));
}

float
dsq0Segment (vec3 p, vec3 q)
{
  float dsq12 = dsqSegmentLine(p, q, G12_P, G12_V);
  float dsq08 = dsqSegmentLine(p, q, G08_P, G08_V);
  float dsq06 = dsqSegmentLine(p, q, G06_P, G06_V);
  float dsq02 = dsqSegmentLine(p, q, G02_P, G02_V);
  return min(min(dsq12, dsq08), min(dsq02, dsq06));
}

float
sdFace0 (vec3 p)
{
  float fiz = mod(p.z, 2.0 * HALF_OUTER_DIAM);
  vec3 pArch = vec3(abs(fiz - HALF_OUTER_DIAM), p.y, abs(p.x));
  float dArch = sdArchA(HALF_OUTER_DIAM, HALF_WIDTH, pArch);

  return dArch;
}

vec2 uvFace0(vec3 p)
{
  float tWidth = unmix(-HALF_WIDTH, HALF_WIDTH, p.x);
  float phi = tWidth / 4.0;

  vec2 usq = paramUnitSquare(p.z / (2.0 * HALF_OUTER_DIAM));
  float theta = 0.5 + 0.5 * atan(usq.y, usq.x) / PI;

  return vec2(phi, theta);
}

void traverseFace0(
    in bool navOnly,
    inout vec3 p, inout vec3 nv, inout float tRemaining,
    inout vec4 color, inout float glowDsq, inout float shadowAmount,
    out bool finished, out int matHit, out vec3 nvHit, out int nextFace
)
{
  int id = ID_NOTHING;
  float t = tRemaining;

  float tPortalS = hitIntervalFromInside(-HALF_WIDTH, HALF_WIDTH, p.x, nv.x);
  updateHit(t, id, tPortalS, ID_PORTAL_SIDE);

  float tPortalL = hitIntervalFromInside(0.0, 8.0 * HALF_OUTER_DIAM, p.z, nv.z);
  updateHit(t, id, tPortalL, ID_PORTAL_LOOP);

  if (!navOnly)
  {
    float tPlanar = hitIntervalFromInside(-HALF_DEPTH - TERRAIN_POM_DEPTH, HALF_DEPTH, p.y, nv.y);
    updateHit(t, id, tPlanar, ID_PLANAR);

    _SDF_HIT(sdFace0, p, T_MAX-tRemaining, shadowAmount)
  }

  vec3 pNext = p + t * nv;
  glowDsq = min(glowDsq, dsq0Segment(p, pNext));
  p = pNext;
  tRemaining -= t;

  finished = true;
  nextFace = 0;

  if (id == ID_PLANAR)
  {
    if (p.y > 0.0)
    {
      tRemaining = 0.0;
      vec2 phiTheta = uvFace0(p);
      color = blendOnto(color, skyboxLocal(phiTheta));
      matHit = MAT_SKY;

    }
    else
    {
      nvHit = vec3(0.0, 1.0, 0.0);
      color = blendOnto(color, PILLAR_COLOR);
      matHit = MAT_PILLAR;

    }
  }
  else if (id == ID_PILLAR)
  {
    nvHit = _SDF_NORMAL(sdFace0, p);
    color = blendOnto(color, PILLAR_COLOR);
    matHit = MAT_PILLAR;
  }
  else if (id == ID_PORTAL_LOOP)
  {
    finished = false;
    nextFace = 0;

    if (p.z - EPS < 0.0)
    {
      p.z += 8.0 * HALF_OUTER_DIAM;
    }
    else if (p.z + EPS > 8.0 * HALF_OUTER_DIAM)
    {
      p.z -= 8.0 * HALF_OUTER_DIAM;
    }
  }
  else if (id == ID_PORTAL_SIDE)
  {
    finished = false;

    float fi = floor(p.z / (2.0 * HALF_OUTER_DIAM));
    float fiz = mod(p.z, 2.0 * HALF_OUTER_DIAM);

    if (p.x - EPS < -HALF_WIDTH)
    {
      nextFace = 3;

      if (fi < 1.0)
      {
        p.xz = vec2(HALF_OUTER_DIAM, -HALF_OUTER_DIAM + fiz);
      }
      else if (fi < 2.0)
      {
        p.xz = vec2(HALF_OUTER_DIAM - fiz, HALF_OUTER_DIAM);
        nv.xz = vec2(-nv.z, nv.x);
      }
      else if (fi < 3.0)
      {
        p.xz = vec2(-HALF_OUTER_DIAM, HALF_OUTER_DIAM - fiz);
        nv.xz = -nv.xz;
      }
      else
      {
        p.xz = vec2(-HALF_OUTER_DIAM + fiz, -HALF_OUTER_DIAM);
        nv.xz = vec2(nv.z, -nv.x);
      }
    }
    else if (p.x + EPS > HALF_WIDTH)
    {
      nextFace = 1;

      if (fi < 1.0)
      {
        p.xz = vec2(HALF_OUTER_DIAM, HALF_OUTER_DIAM - fiz);
        nv.xz = -nv.xz;
      }
      else if (fi < 2.0)
      {
        p.xz = vec2(HALF_OUTER_DIAM - fiz, -HALF_OUTER_DIAM);
        nv.xz = vec2(-nv.z, nv.x);
      }
      else if (fi < 3.0)
      {
        p.xz = vec2(-HALF_OUTER_DIAM, -HALF_OUTER_DIAM + fiz);
      }
      else
      {
        p.xz = vec2(-HALF_OUTER_DIAM + fiz, HALF_OUTER_DIAM);
        nv.xz = vec2(nv.z, -nv.x);
      }
    }
  }
}

#define G10_P vec3(-0.5, LASER_Y, 2.5)
#define G10_V vec3( INV_SQRT2, 0.0, -INV_SQRT2)
#define G04_P vec3( 0.5, LASER_Y, 6.5)
#define G04_V vec3(-INV_SQRT2, 0.0, -INV_SQRT2)

float dsq2Point(vec3 p)
{
  float dsq10 = dsqPointLine(p, G10_P, G10_V);
  float dsq04 = dsqPointLine(p, G04_P, G04_V);
  return min(dsq10, dsq04);
}

float dsq2Segment(vec3 p, vec3 q)
{
  float dsq10 = dsqSegmentLine(p, q, G10_P, G10_V);
  float dsq04 = dsqSegmentLine(p, q, G04_P, G04_V);
  return min(dsq10, dsq04);
}

float sdFace2(vec3 p)
{
  float fiz = mod(p.z, 2.0 * HALF_INNER_DIAM);
  vec3 pArch = vec3(abs(fiz - HALF_INNER_DIAM), p.y, abs(p.x));
  float dArch = sdArchB(HALF_INNER_DIAM, HALF_WIDTH, pArch);

  return dArch;
}

vec2 uvFace2(vec3 p)
{
  float tWidth = unmix(-HALF_WIDTH, HALF_WIDTH, p.x);
  float phi = 0.5 + tWidth / 4.0;

  vec2 usq = paramUnitSquare(p.z / (2.0 * HALF_INNER_DIAM));
  float theta = 0.5 + 0.5 * atan(usq.y, usq.x) / PI;

  return vec2(phi, theta);
}

void traverseFace2(
    in bool navOnly,
    inout vec3 p, inout vec3 nv, inout float tRemaining,
    inout vec4 color, inout float glowDsq, inout float shadowAmount,
    out bool finished, out int matHit, out vec3 nvHit, out int nextFace
)
{
  int id = ID_NOTHING;
  float t = tRemaining;

  float tPortalS = hitIntervalFromInside(-HALF_WIDTH, HALF_WIDTH, p.x, nv.x);
  updateHit(t, id, tPortalS, ID_PORTAL_SIDE);

  float tPortalL = hitIntervalFromInside(0.0, 8.0 * HALF_INNER_DIAM, p.z, nv.z);
  updateHit(t, id, tPortalL, ID_PORTAL_LOOP);

  if (!navOnly)
  {
    float tPlanar = hitIntervalFromInside(-HALF_DEPTH + ARCH_B_VERT_OFFSET - TERRAIN_POM_DEPTH, HALF_DEPTH, p.y, nv.y);
    updateHit(t, id, tPlanar, ID_PLANAR);

    _SDF_HIT(sdFace2, p, T_MAX-tRemaining, shadowAmount)
  }

  vec3 pNext = p + t * nv;
  glowDsq = min(glowDsq, dsq2Segment(p, pNext));
  p = pNext;
  tRemaining -= t;

  finished = true;
  nextFace = 2;

  if (id == ID_PLANAR)
  {
    if (p.y > 0.0)
    {
      tRemaining = 0.0;
      vec2 phiTheta = uvFace2(p);
      color = blendOnto(color, skyboxLocal(phiTheta));
      matHit = MAT_SKY;
    }
    else
    {
      nvHit = vec3(0.0, 1.0, 0.0);
      color = blendOnto(color, PILLAR_COLOR);
      matHit = MAT_PILLAR;
    }
  }
  else if (id == ID_PILLAR)
  {
    nvHit = _SDF_NORMAL(sdFace2, p);
    color = blendOnto(color, PILLAR_COLOR);
    matHit = MAT_PILLAR;
  }
  else if (id == ID_PORTAL_LOOP)
  {
    finished = false;
    nextFace = 2;

    if (p.z - EPS < 0.0)
    {
      p.z += 8.0 * HALF_INNER_DIAM;
    }
    else if (p.z + EPS > 8.0 * HALF_INNER_DIAM)
    {
      p.z -= 8.0 * HALF_INNER_DIAM;
    }
  }
  else if (id == ID_PORTAL_SIDE)
  {
    finished = false;

    float fi = floor(p.z / (2.0 * HALF_INNER_DIAM));
    float fiz = mod(p.z, 2.0 * HALF_INNER_DIAM);

    if (p.x - EPS < -HALF_WIDTH)
    {
      nextFace = 1;

      if (fi < 1.0)
      {
        p.xz = vec2(HALF_INNER_DIAM, HALF_INNER_DIAM - fiz);
        nv.xz = -nv.xz;
      }
      else if (fi < 2.0)
      {
        p.xz = vec2(HALF_INNER_DIAM - fiz, -HALF_INNER_DIAM);
        nv.xz = vec2(-nv.z, nv.x);
      }
      else if (fi < 3.0)
      {
        p.xz = vec2(-HALF_INNER_DIAM, -HALF_INNER_DIAM + fiz);
      }
      else
      {
        p.xz = vec2(-HALF_INNER_DIAM + fiz, HALF_INNER_DIAM);
        nv.xz = vec2(nv.z, -nv.x);
      }
    }
    else if (p.x + EPS > HALF_WIDTH)
    {
      nextFace = 3;

      if (fi < 1.0)
      {
        p.xz = vec2(HALF_INNER_DIAM, -HALF_INNER_DIAM + fiz);
      }
      else if (fi < 2.0)
      {
        p.xz = vec2(HALF_INNER_DIAM - fiz, HALF_INNER_DIAM);
        nv.xz = vec2(-nv.z, nv.x);
      }
      else if (fi < 3.0)
      {
        p.xz = vec2(-HALF_INNER_DIAM, HALF_INNER_DIAM - fiz);
        nv.xz = -nv.xz;
      }
      else
      {
        p.xz = vec2(-HALF_INNER_DIAM + fiz, -HALF_INNER_DIAM);
        nv.xz = vec2(nv.z, -nv.x);
      }
    }
  }
}

#define G01_P vec3(2.4, LASER_Y, 1.9)
#define G01_V vec3(-INV_SQRT2, 0.0, INV_SQRT2)
#define G05_P (vec3(-1.0, LASER_Y,  0.5) + vec3(0.25, 0.0, 0.25))
#define G05_V vec3(-INV_SQRT2, 0.0, -INV_SQRT2)
#define G09_P (vec3( 0.5, LASER_Y, -1.0) + vec3(0.25, 0.0, 0.25))
#define G09_VR vec3(-INV_SQRT2, 0.0, -INV_SQRT2)

float dsq1Point(vec3 p)
{
  float dsq01 = dsqPointLine(p, G01_P, G01_V);
  float dsq05 = dsqPointRay(p, G05_P, G05_V);
  float dsq09r = dsqPointRay(p, G09_P, G09_VR);
  return min(dsq01, min(dsq05, dsq09r));
}

float dsq1Segment(vec3 p, vec3 q)
{
  float dsq01 = dsqSegmentLine(p, q, G01_P, G01_V);
  float dsq05 = dsqSegmentRay(p, q, G05_P, G05_V);
  float dsq09r = dsqSegmentRay(p, q, G09_P, G09_VR);
  return min(dsq01, min(dsq05, dsq09r));
}

float sdFace1(vec3 p)
{
  vec3 pArch = vec3(min(abs(p.x), abs(p.z)), p.y, max(abs(p.x), abs(p.z)));
  float dO = sdArchA(HALF_OUTER_DIAM, HALF_OUTER_DIAM, pArch);
  float dI = sdArchB(HALF_INNER_DIAM, HALF_INNER_DIAM, pArch);

  return min(dI, dO);
}

vec2 uvFace1(vec3 p)
{
  float tWidth = unmix(HALF_OUTER_DIAM, HALF_INNER_DIAM, max(abs(p.x), abs(p.z)));
  float phi = 0.25 + tWidth / 4.0;

  float theta = 0.5 + 0.5 * atan(-p.z, p.x) / PI;

  return vec2(phi, theta);
}

void traverseFace1(
    in bool navOnly,
    inout vec3 p, inout vec3 nv, inout float tRemaining,
    inout vec4 color, inout float glowDsq, inout float shadowAmount,
    out bool finished, out int matHit, out vec3 nvHit, out int nextFace
)
{
  int id = ID_NOTHING;
  float t = tRemaining;

  float tPortalO = hitBox2FromInside(
        -vec2(HALF_OUTER_DIAM), vec2(HALF_OUTER_DIAM),
        p.xz, nv.xz
    );
  updateHit(t, id, tPortalO, ID_PORTAL_OUTER);

  float tPortalI;
  float didHitPortalI;
  hitBox2FromOutside(
        -vec2(HALF_INNER_DIAM), vec2(HALF_INNER_DIAM),
        p.xz, nv.xz, tPortalI, didHitPortalI
    );
  if (didHitPortalI > 0.5)
  {
    updateHit(t, id, tPortalI, ID_PORTAL_INNER);
  }

  if (!navOnly)
  {
    float tPlanar = hitIntervalFromInside(-HALF_DEPTH - TERRAIN_POM_DEPTH, HALF_DEPTH, p.y, nv.y);
    updateHit(t, id, tPlanar, ID_PLANAR);

        _SDF_HIT(sdFace1, p, T_MAX-tRemaining, shadowAmount)
  }

  vec3 pNext = p + t * nv;
  glowDsq = min(glowDsq, dsq1Segment(p, pNext));
  p = pNext;
  tRemaining -= t;

  finished = true;
  nextFace = 1;

  if (id == ID_PLANAR)
  {
    if (p.y > 0.0)
    {
      tRemaining = 0.0;
      vec2 phiTheta = uvFace1(p);
      color = blendOnto(color, skyboxLocal(phiTheta));
      matHit = MAT_SKY;
    }
    else
    {
      nvHit = vec3(0.0, 1.0, 0.0);
      color = blendOnto(color, PILLAR_COLOR);
      matHit = MAT_PILLAR;
    }
  }
  else if (id == ID_PILLAR)
  {
    nvHit = _SDF_NORMAL(sdFace1, p);
    color = blendOnto(color, PILLAR_COLOR);
    matHit = MAT_PILLAR;
  }
  else if (id == ID_PORTAL_INNER)
  {
    finished = false;
    nextFace = 2;

    if (p.x + EPS > HALF_INNER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, HALF_INNER_DIAM - p.z);
      nv.xz = -nv.xz;
    }
    else if (p.x - EPS < -HALF_INNER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, 4.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM + p.z));
    }
    else if (p.z - EPS < -HALF_INNER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, 2.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM - p.x));
      nv.xz = vec2(nv.z, -nv.x);
    }
    else
    {
      p.xz = vec2(-HALF_WIDTH, 6.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM + p.x));
      nv.xz = vec2(-nv.z, nv.x);
    }
  }
  else if (id == ID_PORTAL_OUTER)
  {
    finished = false;
    nextFace = 0;

    if (p.x + EPS > HALF_OUTER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, HALF_OUTER_DIAM - p.z);
      nv.xz = -nv.xz;
    }
    else if (p.x - EPS < -HALF_OUTER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, 4.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM + p.z));
    }
    else if (p.z - EPS < -HALF_OUTER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, 2.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM - p.x));
      nv.xz = vec2(nv.z, -nv.x);
    }
    else
    {
      p.xz = vec2(HALF_WIDTH, 6.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM + p.x));
      nv.xz = vec2(-nv.z, nv.x);
    }
  }
}

#define G07_P vec3(-2.4, LASER_Y, 1.9)
#define G07_V vec3(INV_SQRT2, 0.0, INV_SQRT2)
#define G11_P (vec3( 1.0, LASER_Y,  0.5) + vec3(-0.25, 0.0, 0.25))
#define G11_V vec3(INV_SQRT2, 0.0, -INV_SQRT2)
#define G03_P (vec3(-0.5, LASER_Y, -1.0) + vec3(-0.25, 0.0, 0.25))
#define G03_VR vec3(INV_SQRT2, 0.0, -INV_SQRT2)

float dsq3Point(vec3 p)
{
  float dsq07 = dsqPointLine(p, G07_P, G07_V);
  float dsq11 = dsqPointRay(p, G11_P, G11_V);
  float dsq03r = dsqPointRay(p, G03_P, G03_VR);
  return min(dsq07, min(dsq11, dsq03r));
}

float dsq3Segment(vec3 p, vec3 q)
{
  float dsq07 = dsqSegmentLine(p, q, G07_P, G07_V);
  float dsq11 = dsqSegmentRay(p, q, G11_P, G11_V);
  float dsq03r = dsqSegmentRay(p, q, G03_P, G03_VR);
  return min(dsq07, min(dsq11, dsq03r));
}

float sdFace3(vec3 p)
{
  vec3 pArch = vec3(min(abs(p.x), abs(p.z)), p.y, max(abs(p.x), abs(p.z)));
  float dO = sdArchA(HALF_OUTER_DIAM, HALF_OUTER_DIAM, pArch);
  float dI = sdArchB(HALF_INNER_DIAM, HALF_INNER_DIAM, pArch);

  return min(dI, dO);
}

vec2 uvFace3(vec3 p)
{
  float tWidth = unmix(HALF_INNER_DIAM, HALF_OUTER_DIAM, max(abs(p.x), abs(p.z)));
  float phi = 0.75 + tWidth / 4.0;

  float theta = 0.5 + 0.5 * atan(p.z, p.x) / PI;

  return vec2(phi, theta);
}

void traverseFace3(
    bool navOnly,
    inout vec3 p, inout vec3 nv, inout float tRemaining,
    inout vec4 color, inout float glowDsq, inout float shadowAmount,
    out bool finished, out int matHit, out vec3 nvHit, out int nextFace
)
{
  int id = ID_NOTHING;
  float t = tRemaining;

  float tPortalO = hitBox2FromInside(
        -vec2(HALF_OUTER_DIAM), vec2(HALF_OUTER_DIAM),
        p.xz, nv.xz
    );
  updateHit(t, id, tPortalO, ID_PORTAL_OUTER);

  float tPortalI;
  float didHitPortalI;
  hitBox2FromOutside(
        -vec2(HALF_INNER_DIAM), vec2(HALF_INNER_DIAM),
        p.xz, nv.xz, tPortalI, didHitPortalI
    );
  if (didHitPortalI > 0.5)
  {
    updateHit(t, id, tPortalI, ID_PORTAL_INNER);
  }

  if (!navOnly)
  {
    float tPlanar = hitIntervalFromInside(-HALF_DEPTH - TERRAIN_POM_DEPTH, HALF_DEPTH, p.y, nv.y);
    updateHit(t, id, tPlanar, ID_PLANAR);

    _SDF_HIT(sdFace3, p, T_MAX-tRemaining, shadowAmount)
  }

  vec3 pNext = p + t * nv;
  glowDsq = min(glowDsq, dsq3Segment(p, pNext));
  p = pNext;
  tRemaining -= t;

  finished = true;
  nextFace = 3;

  if (id == ID_PLANAR)
  {
    if (p.y > 0.0)
    {
      tRemaining = 0.0;
      vec2 phiTheta = uvFace3(p);
      color = blendOnto(color, skyboxLocal(phiTheta));
      matHit = MAT_SKY;
    }
    else
    {
      nvHit = vec3(0.0, 1.0, 0.0);
      color = blendOnto(color, PILLAR_COLOR);
      matHit = MAT_PILLAR;
    }
  }
  else if (id == ID_PILLAR)
  {
    nvHit = _SDF_NORMAL(sdFace3, p);
    color = blendOnto(color, PILLAR_COLOR);
    matHit = MAT_PILLAR;
  }
  else if (id == ID_PORTAL_INNER)
  {
    finished = false;
    nextFace = 2;

    if (p.x + EPS > HALF_INNER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, HALF_INNER_DIAM + p.z);
    }
    else if (p.x - EPS < -HALF_INNER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, 4.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM - p.z));
      nv.xz = -nv.xz;
    }
    else if (p.z - EPS < -HALF_INNER_DIAM)
    {
      p.xz = vec2(HALF_WIDTH, 6.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM + p.x));
      nv.xz = vec2(-nv.z, nv.x);
    }
    else
    {
      p.xz = vec2(HALF_WIDTH, 2.0 * HALF_INNER_DIAM + (HALF_INNER_DIAM - p.x));
      nv.xz = vec2(nv.z, -nv.x);
    }
  }
  else if (id == ID_PORTAL_OUTER)
  {
    finished = false;
    nextFace = 0;

    if (p.x + EPS > HALF_OUTER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, HALF_OUTER_DIAM + p.z);
    }
    else if (p.x - EPS < -HALF_OUTER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, 4.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM - p.z));
      nv.xz = -nv.xz;
    }
    else if (p.z - EPS < -HALF_OUTER_DIAM)
    {
      p.xz = vec2(-HALF_WIDTH, 6.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM + p.x));
      nv.xz = vec2(-nv.z, nv.x);
    }
    else
    {
      p.xz = vec2(-HALF_WIDTH, 2.0 * HALF_OUTER_DIAM + (HALF_OUTER_DIAM - p.x));
      nv.xz = vec2(nv.z, -nv.x);
    }
  }
}

#define FIXED_UP vec3(0.0, 1.0, 0.0)
#define TAN_HALF_FOVY 0.7002075382097097
#define CAM_Z_NEAR 0.1
#define CAM_Z_FAR 50.0

mat4 getClipToWorld(float aspectWoverH, vec3 nvCamFw)
{
  mat4 clipToEye = mat4(
        aspectWoverH * TAN_HALF_FOVY, 0.0, 0.0, 0.0,
        0.0, TAN_HALF_FOVY, 0.0, 0.0,
        0.0, 0.0, 0.0, (CAM_Z_NEAR - CAM_Z_FAR) / (2.0 * CAM_Z_NEAR * CAM_Z_FAR),
        0.0, 0.0, -1.0, (CAM_Z_NEAR + CAM_Z_FAR) / (2.0 * CAM_Z_NEAR * CAM_Z_FAR)
    );

  vec3 nvCamRt = normalize(cross(nvCamFw, FIXED_UP));
  vec3 nvCamUp = cross(nvCamRt, nvCamFw);
  mat4 eyeToWorld = mat4(
         nvCamRt, 0.0,
         nvCamUp, 0.0,
        -nvCamFw, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

  return eyeToWorld * clipToEye;
}

void castRay(
    inout vec3 pRay, inout vec3 nvRay, inout int rayFace,
    out vec4 cFinal, out float glowDsq, out float shadowAmount,
    out float dHit, out int matHit, out vec3 nvHit
)
{
  float tRemaining = T_MAX;
  cFinal = vec4(0.0);
  glowDsq = 1e5;
  shadowAmount = 0.0;

  for (int i = 0; i < 100; i++)
  {
    bool finished;

    if (rayFace == 0)
    {
      traverseFace0(false, pRay, nvRay, tRemaining, cFinal, glowDsq, shadowAmount, finished, matHit, nvHit, rayFace);
    }
    else if (rayFace == 1)
    {
      traverseFace1(false, pRay, nvRay, tRemaining, cFinal, glowDsq, shadowAmount, finished, matHit, nvHit, rayFace);
    }
    else if (rayFace == 2)
    {
      traverseFace2(false, pRay, nvRay, tRemaining, cFinal, glowDsq, shadowAmount, finished, matHit, nvHit, rayFace);
    }
    else
    {
      traverseFace3(false, pRay, nvRay, tRemaining, cFinal, glowDsq, shadowAmount, finished, matHit, nvHit, rayFace);
    }

    if (finished)
      break;
  }

  dHit = T_MAX - tRemaining;
}

void navigate(inout vec3 sphPos, inout int sphFace, inout vec3 nvSphDir, float dist)
{
  float tRemaining = dist;
  vec3 pCur = sphPos;
  vec3 nvCur = nvSphDir;
  int face = sphFace;

  for (int i = 0; i < 100; i++)
  {
    vec4 c;
    float ga;
    float sa;
    bool finished;
    int matHit;
    vec3 nvHit;

    if (face == 0)
    {
      traverseFace0(true, pCur, nvCur, tRemaining, c, ga, sa, finished, matHit, nvHit, face);
    }
    else if (face == 1)
    {
      traverseFace1(true, pCur, nvCur, tRemaining, c, ga, sa, finished, matHit, nvHit, face);
    }
    else if (face == 2)
    {
      traverseFace2(true, pCur, nvCur, tRemaining, c, ga, sa, finished, matHit, nvHit, face);
    }
    else if (face == 3)
    {
      traverseFace3(true, pCur, nvCur, tRemaining, c, ga, sa, finished, matHit, nvHit, face);
    }

    if (finished)
    {
      break;
    }
  }

  sphPos = pCur;
  nvSphDir = nvCur;
  sphFace = face;
}

void
main ()
{
  bool isMousePressed = clamp(iMouse.z, 0.0, 1.0) > 0.0;
  bool hideGlow = iKeyboard.y; // d
  bool hideLight = iKeyboard.z; // f

  float LOOPS = 3.0;
  float geodesicLength = sqrt(2.0) * 12.6;
  float tLoop = isMousePressed ? (iMouse.x / iResolution.x) : mod(0.02 * iTime, 1.0);

  vec3 sphPos = vec3(0.5);
  int sphFace = 0;
  vec3 nvSphDir = normalize(vec3(1.0, 0.0, -1.0));

  navigate(sphPos, sphFace, nvSphDir, tLoop * LOOPS * geodesicLength);
  sphPos = vec3(sphPos.x, mix(0.2, 0.55, 0.5 + 0.5 * sin(tLoop * TWOPI)), sphPos.z);

  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec2 angRtUp = vec2(-0.35 - 0.1 * PI * sin(tLoop * TWOPI), PI * sin(tLoop * TWOPI));

  vec3 nvCamRt = normalize(cross(nvSphDir, FIXED_UP));
  mat3 rotRt = glRotate(nvCamRt, angRtUp.s);
  mat3 rotUp = glRotate(vec3(0.0, 1.0, 0.0), angRtUp.t);
  vec3 nvCamFw = rotUp * (rotRt * nvSphDir);

  mat4 clipToWorld = getClipToWorld(iResolution.x / iResolution.y, nvCamFw);
  vec4 vWorld = clipToWorld * vec4(uv * 2.0 - 1.0, 1.0, 1.0);
  vec3 nvCamDir = normalize(vWorld.xyz / vWorld.w);

  vec4 cFinal;
  float glowDsq;
  float shadowAmount;
  float dHit;
  int matHit;
  vec3 nvHit;
  castRay(
        sphPos, nvCamDir, sphFace,
        cFinal, glowDsq, shadowAmount, dHit, matHit, nvHit
    );

  float fogAmount = clamp(dHit / T_MAX, 0.0, 1.0);
  if (hideGlow)
  {
    glowDsq = 1e5;
  }
  vec4 glowPremul = glowPremulFromDsq(glowDsq);

  float pomTop = -HALF_DEPTH;
  if (sphFace == 2)
  {
    pomTop = -HALF_DEPTH + ARCH_B_VERT_OFFSET;
  }

  if (sphPos.y < pomTop)
  {
    float dSurf = (pomTop - sphPos.y) / nvCamDir.y;
    vec3 sphPosSurf = sphPos + dSurf * nvCamDir;
    float dHitSurf = dHit + dSurf;

    float dMax = -(TERRAIN_POM_DEPTH - EPS) / nvCamDir.y;
    vec3 sphPosBase = sphPosSurf + dMax * nvCamDir;
    float dHitBase = dHitSurf + dMax;

    int i = 0;
    float it = 0.0;
    float iMidstep = 0.0;
    float yOffsetPrev = 0.0;
    for (; i < TERRAIN_POM_STEPS; i++)
    {
      it = float(i) / float(TERRAIN_POM_STEPS);

      vec3 sphPosStep = mix(sphPosSurf, sphPosBase, it);
      float texY = mix(sphPosBase.y, sphPosSurf.y, floorH(sphFace, sphPosStep.xz));

      float yOffset = sphPosStep.y - texY;
      if (yOffset < 0.0)
      {
        float iMidstep = mix(float(i) - 1.0, float(i), unmix(yOffsetPrev, yOffset, 0.0));
        it = iMidstep / float(TERRAIN_POM_STEPS);
        break;
      }
      yOffsetPrev = yOffset;
    }

    float dHitStep = mix(dHitSurf, dHitBase, it);
    if (dHitStep < dHit)
    {
      sphPos = mix(sphPosSurf, sphPosBase, it);
      dHit = mix(dHitSurf, dHitBase, it);

      float hSA = floorH(sphFace, sphPos.xz + vec2(-TERRAIN_N_EPS, 0.0));
      float hSB = floorH(sphFace, sphPos.xz + vec2(TERRAIN_N_EPS, 0.0));
      float hTA = floorH(sphFace, sphPos.xz + vec2(0.0, -TERRAIN_N_EPS));
      float hTB = floorH(sphFace, sphPos.xz + vec2(0.0, TERRAIN_N_EPS));
      vec2 gradh = vec2(hSB - hSA, hTB - hTA) * (TERRAIN_POM_DEPTH / (2.0 * TERRAIN_N_EPS));
      nvHit = normalize(vec3(-gradh.x, 1.0, -gradh.y));

      if (sphFace == 0 || sphFace == 2)
      {
        vec3 waterColor;
        if (sphFace == 0)
        {
          waterColor = mix(1.0, 0.5, it) * 0.7 * vec3(0.3, 0.7, 1.0);
        }
        else
        {
          waterColor = mix(1.0, 0.5, it) * 1.0 * vec3(1.0, 0.9, 0.8);
        }

        cFinal = blendOnto(vec4(waterColor, 1.0), cFinal);
        matHit = MAT_WATER;
      }
      else
      {
        vec3 terrainColor;
        if (sphFace == 1)
          terrainColor = colormapTerrainStone(1.0 - it);
        else
          terrainColor = colormapTerrainGrass(1.0 - it);

        cFinal = vec4(terrainColor, 1.0);
        matHit = MAT_TERRAIN;
      }
    }
  }

  if (matHit != MAT_SKY)
  {
    vec3 ambientColor = 0.15 * cFinal.rgb;
        
    float lSA = _UV_FACE(sphFace, sphPos + vec3(-0.05, 0.0, 0.0)).s;
    float lSB = _UV_FACE(sphFace, sphPos + vec3( 0.05, 0.0, 0.0)).s;
    float lTA = _UV_FACE(sphFace, sphPos + vec3(0.0, 0.0, -0.05)).s;
    float lTB = _UV_FACE(sphFace, sphPos + vec3(0.0, 0.0,  0.05)).s;

    vec2 gradlLight = normalize(vec2(lSB - lSA, lTB - lTA));
    vec3 nvFragToLight = normalize(vec3(gradlLight.x, 0.8, gradlLight.y));

    vec2 gradlShadow = _DPHI_FACE(sphFace, sphPos) + vec2(0.0001, 0.001);
    vec3 nvFragToLightShadow = normalize(vec3(gradlShadow.x, 0.8, gradlShadow.y));
        
    vec3 nvCamDir2 = nvFragToLightShadow;
    vec3 sphPos2 = sphPos + (5.0 * SDF_EPS) * nvHit;

    int sphFace2 = sphFace;
    vec4 cFinal2;
    float glowDsq2;
    float shadowAmount2;
    float dHit2;
    int matHit2;
    vec3 nvHit2;

    castRay(
            sphPos2, nvCamDir2, sphFace2,
            cFinal2, glowDsq2, shadowAmount2, dHit2, matHit2, nvHit2
        );

    float hardShadowAmount = 1.0 - step(T_MAX, dHit2);
    float totalShadow = max(shadowAmount2, hardShadowAmount);
        
    vec3 litColor;
    computeLighting(
            matHit,
            1.25 * (1.0 - totalShadow) * LIGHT_COLOR, cFinal.rgb,
            nvHit, nvFragToLight, -nvCamDir,
            litColor
        );
    if (hideLight)
      litColor = vec3(0.0);
        
    float dsqLaser = _DSQ_POINT_FACE(sphFace, sphPos);
    float dsqInt = 1.0 / (1.0 + 1.5 * dsqLaser);

    vec3 dsqNormal = _DSQ_NORMAL(sphFace, sphPos);

    vec3 litColor2;
    computeLighting(
            matHit,
            1.25 * dsqInt * mix(GLOW_COLOR, vec3(1.0), 0.25), cFinal.rgb,
            nvHit, -dsqNormal, -nvCamDir,
            litColor2
        );
    if (hideGlow)
      litColor2 = vec3(0.0);

    cFinal = vec4(ambientColor + litColor + litColor2, 1.0);
    cFinal = blendOnto(glowPremul, cFinal);
    cFinal *= (1.0 - fogAmount);
  }
  else
    cFinal = blendOnto(glowPremul * 0.4, cFinal);

  cFinal = blendOnto(cFinal, skyboxGlobal(nvCamDir));

  gl_FragColor = vec4(packColor(gl_FragCoord.xy, clamp(cFinal.rgb, 0.0, 1.0)), glowPremul.a, dHit / T_MAX);
}
