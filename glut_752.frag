uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;

#define AA 1
#define ANIMATE_OBJECTS true
#define ROTATE_SCENE false

#define flag true

#define PI 3.14159265359

float time = 0.0;
float aTime = 0.0;
float sinTime = 0.0;

vec3
rotateX (vec3 p, float a)
{
  float sa = sin(a), ca = cos(a);
  return vec3(p.x, ca * p.y - sa * p.z, sa * p.y + ca * p.z);
}

vec3
rotateY (vec3 p, float a)
{
  float sa = sin(a), ca = cos(a);
  return vec3(ca * p.x + sa * p.z, p.y, -sa * p.x + ca * p.z);
}

vec3
rotateZ (vec3 p, float a)
{
  float sa = sin(a), ca = cos(a);
  return vec3(ca * p.x - sa * p.y, sa * p.x + ca * p.y, p.z);
}

float
length2 (vec2 p)
{
  return sqrt(p.x * p.x + p.y * p.y);
}

float
length6 (vec2 p)
{
  p = p * p * p;
  p = p * p;
  return pow(p.x + p.y, 1.0 / 6.0);
}

float
length8 (vec2 p)
{
  p = p * p;
  p = p * p;
  p = p * p;
  return pow(p.x + p.y, 1.0 / 8.0);
}

#define opUnion(a,b)      min(a,b)
#define opIntersect(a,b)  max(a,b)
#define opDifference(a,b) max(a,-b)
#define opSubstraction(a,b) max(-a,b)
#define opExklusiveOr(a,b)  min(max(-a,b),max(-b,a))

vec2
opU (vec2 d1, vec2 d2)
{
  return (d1.x < d2.x) ? d1 : d2;
}

float
smin (float a, float b, float k)
{
  float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
  return mix(b, a, h) - k * h * (1.0 - h);
}

// vec3 opRepeat (vec3 p, vec3 c)  {  return mod(p, c)-0.5*c;   }
#define opRepeat(p,c) (mod(p,c)-0.5*c)

vec3
opTwist (vec3 p, float angle)
{
  float c = cos(10.0 * p.y + angle);
  float s = sin(10.0 * p.y + angle);
  mat2 m = mat2(c, -s, s, c);
  return vec3(m * p.xz, p.y);
}

float
sdPlane1 (vec3 p)
{
  return p.y;
}

float
sdPlane (vec3 p, vec4 n)
{
  return dot(p, n.xyz) + n.w;
}

float
sdSphere (vec3 p, float radius)
{
  return length(p) - radius;
}

float
sdEllipsoid (vec3 p, vec3 r)
{
  float k0 = length(p / r);
  float k1 = length(p / (r * r));
  return k0 * (k0 - 1.0) / k1;
}

float
sdEgg1 (vec3 p, float radius)
{
  p.y = 0.8 * p.y + 0.15 * pow(1.9 * dot(p.xz, p.xz), 0.65);
  return length(p) - radius;
}

float
sdEgg2 (vec3 p, float r)
{
  p.y -= r * 0.5;
  p.y *= 0.8;
  return length(p) - 1.7 * r * cos(p.y + 1.36);
}

float
sdRoundCone_Egg (vec3 p, vec3 a, vec3 b, float r1, float r2)
{
  float pa = length(p - a);
  float pb = length(p - b);

  float fa = pa / (pa + pb);
  float fb = pb / (pa + pb);

  vec3 ps = a + (b - a) * fa;
  return length(p - ps) - (r2 * fa + r1 * fb);
}

float
sdBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
}

float
udBox (vec3 p, vec3 b)
{
  return length(max(abs(p) - b, 0.0));
}

float
udRoundBox (vec3 p, vec3 b, float r)
{
  return length(max(abs(p) - b, 0.0)) - r;
}

float
sdTorus (vec3 p, vec2 t)
{
  return length(vec2(length(p.xz) - t.x, p.y)) - t.y;
}

float
sdTorus82 (vec3 p, vec2 t)
{
  vec2 q = vec2(length2(p.xz) - t.x, p.y);
  return length8(q) - t.y;
}

float
sdTorus88 (vec3 p, vec2 t)
{
  vec2 q = vec2(length8(p.xz) - t.x, p.y);
  return length8(q) - t.y;
}

float
sdCapsule (vec3 p, vec3 a, vec3 b, float r)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h) - r;
}

float
sdRoundCone (vec3 p, float r1, float r2, float h)
{
  vec2 q = vec2(length(p.xz), p.y);

  float b = (r1 - r2) / h;
  float a = sqrt(1.0 - b * b);
  float k = dot(q, vec2(-b, a));

  if (k < 0.0)
    return length(q) - r1;
  if (k > a * h)
    return length(q - vec2(0.0, h)) - r2;

  return dot(q, vec2(a, b)) - r1;
}

float
dot2 (vec3 v)
{
  return dot(v, v);
}

float
sdRoundCone (vec3 p, vec3 a, vec3 b, float r1, float r2)
{
  vec3 ba = b - a;
  float l2 = dot(ba, ba);
  float rr = r1 - r2;
  float a2 = l2 - rr * rr;
  float il2 = 1.0 / l2;

  vec3 pa = p - a;
  float y = dot(pa, ba);
  float z = y - l2;
  float x2 = dot2(pa * l2 - ba * y);
  float y2 = y * y * l2;
  float z2 = z * z * l2;

  float k = sign(rr) * rr * rr * x2;
  if (sign(z) * a2 * z2 > k)
    return sqrt(x2 + z2) * il2 - r2;
  if (sign(y) * a2 * y2 < k)
    return sqrt(x2 + y2) * il2 - r1;
  return (sqrt(x2 * a2 * il2) + y * rr) * il2 - r1;
}

float
sdTrianglePrism (vec3 p, float radius, float height)
{
  vec3 q = abs(p);
#ifdef flag
  return max(q.z - height, max(q.x * 0.866025 + p.y * 0.5, -p.y) - radius * 0.5);
#else
  float d1 = q.z-height;
  float d2 = max(q.x*0.866025+p.y*0.5, -p.y) - radius*0.5;
  return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.0);
#endif
}

float
sdHexagonalPrism (vec3 p, float radius, float height)
{
  vec3 q = abs(p);
#ifdef flag
  return max(q.z - height, max((q.x * 0.866025 + q.y * 0.5), q.y) - radius);
#else
  float d1 = q.z-heighty;
  float d2 = max((q.x*0.866025 +q.y*0.5), q.y)-radius;
  return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
#endif
}

float
sdCylinder (vec3 p, vec2 h)
{
  vec2 d = abs(vec2(length(p.xz), p.y)) - h;
  return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float
sdCylinder (vec3 p, vec3 h)
{
  return length(p.xz - h.xy) - h.z;
}

float
sdCylinder6 (vec3 p, vec3 h)
{
  return max(length6(p.xz) - h.x, abs(p.y) - h.z);
}

float
sdCone (vec3 p, vec3 c)
{
  vec2 q = vec2(length(p.xz), p.y);
#if 0
    return max( max( dot(q, c.xy), p.y), -p.y -c.z );
#else
  float d1 = -p.y - c.z;
  float d2 = max(dot(q, c.xy), p.y);
  return length(max(vec2(d1, d2), 0.0)) + min(max(d1, d2), 0.);
#endif
}

const float h = (1. + sqrt(5.)) / 2.;
float pipeDodecahedron(vec3 p, float scale, float pipeRadius)
{
  vec3 a = vec3(0, 1. - h, h),
       m = normalize(vec3(-1, h - 2., h - 1.)),
       n = normalize(a - 1.);
  p = abs(p);
  p = (p.x > p.z) && (p.x > p.y) ? p.yzx 
                                 : (p.y > p.z) ? p.zxy : p;
  if (dot(p, m) < 0.)
    p = reflect(p, m);
  if (dot(p, n) < 0.)
    p = reflect(p, n);
  a *= scale;
  p.y -= clamp(p.y - a.y, 0., -2. * a.y);
  return length(p - a) - pipeRadius;
}

float
sdDodecahedron (vec3 p, float radius)
{
  const float phi = 1.61803398875;
  const vec3 n = normalize(vec3(phi, 1, 0));

  p = abs(p / radius);
  float a = dot(p, n.xyz);
  float b = dot(p, n.zxy);
  float c = dot(p, n.yzx);
  return (max(max(a, b), c) - n.x) * radius;
}

float
sdIcosahedron (vec3 p, float radius)
{
  const float q = 2.61803398875;
  const vec3 n1 = normalize(vec3(q, 1, 0));
  const vec3 n2 = vec3(0.57735026919);

  p = abs(p / radius);
  float a = dot(p, n1.xyz);
  float b = dot(p, n1.zxy);
  float c = dot(p, n1.yzx);
  float d = dot(p, n2) - n1.x;
  return max(max(max(a, b), c) - n1.x, d) * radius;
}

float
sdIcosDodecaStar1 (vec3 p, float radius)
{
  return min(sdDodecahedron(p, radius), sdIcosahedron(p.zyx, radius));
}

vec2
sdIcosDodecaStar2 (vec3 p, float radius)
{
  float sd = sdSphere(p, 0.3);
  if (sd > 0.3)
    return vec2(1.0, 0.0);
  float s1 = sdDodecahedron(p, radius);
  float s2 = sdIcosahedron(p.zyx, radius);
  int materialId = s1 < s2 ? 55 : 188;
  return vec2(max(sd, min(s1, s2)), materialId);
}

float
sdPyramid4 (vec3 p, vec3 h)
{
  float box = sdBox(p - vec3(0, -2.0 * h.z, 0), vec3(2.0 * h.z));

  float d = 0.0;
  d = max(d, abs(dot(p, vec3(-h.x, h.y, 0))));
  d = max(d, abs(dot(p, vec3(h.x, h.y, 0))));
  d = max(d, abs(dot(p, vec3(0, h.y, h.x))));
  d = max(d, abs(dot(p, vec3(0, h.y, -h.x))));
  float octa = d - h.z;
  return max(-box, octa);
}

float
sdPyramid (vec3 p, float h)
{
  vec3 q = abs(p);
  return max(-p.y, (q.x + q.y + q.z - h) / 3.0);
}

float
gridCube (vec4 cube, vec3 pos)
{
  cube.xyz -= pos;
  return max(max(abs(cube.x) - cube.w
                , abs(cube.y) - cube.w)
            , abs(cube.z) - cube.w);
}

vec2
Grid2Colors (vec3 p)
{
  float sd = sdSphere(p, 0.3);
  if (sd > 0.3)
    return vec2(1.0, 0.0);
  vec3 mp = mod(p, 0.1);
  mp.y = p.y;
  float s1 = gridCube(vec4(0.05, 0.05, 0.05, 0.025),
                      vec3(mp.x, mp.y + (sin(p.x * PI * 10.) * sin(p.z * PI * 10.)) * 0.025, 0.05));
  float s2 = gridCube(vec4(0.05, 0.05, 0.05, 0.025),
                      vec3(0.05, mp.y + (sin(p.x * PI * 10.) * -sin(p.z * PI * 10.)) * 0.025, mp.z));
  int materialId = s1 < s2 ? 177 : 71; // set color
  return vec2(max(sd, min(s1, s2)), materialId);
}

float
sdWaveSphere (vec3 p, float radius, int waves, float waveSize)
{
  float d = length(p) - radius * 2.2;
  if (d > 0.0)
    return 0.2;

  d = waveSize * (radius * radius - (p.y * p.y));
  radius += d * cos(atan(p.x, p.z) * float(waves));
  return 0.5 * (length(p) - radius);
}

float
sdBlob (vec3 pos, float r)
{
  vec3 v1 = pos * 6.0;
  return 0.05 * (r + 0.5 * (length(dot(v1, v1)) - 0.51 * (cos(4. * v1.x) + cos(4. * v1.y) + cos(4. * v1.z))));
}

float
sdCross (vec3 p, float s)
{
  float da = max(abs(p.x), abs(p.y));
  float db = max(abs(p.y), abs(p.z));
  float dc = max(abs(p.z), abs(p.x));
  return min(da, min(db, dc)) - s;
}

float
CrossInSphere (vec3 p, float r, float s)
{
  float d1 = sdCross(p, s); // cross
  float d2 = length(p) - r; // sphere
  return max(d1, d2); // intersection
}

float
sdBoxMinusSphere (vec3 pos, float radius)
{
  return opDifference(udRoundBox(pos, vec3(0.15), 0.05), sdSphere(  pos, radius - 0.012 + 0.02*sinTime));
}

float
sdRackWheel (vec3 pos)
{
  return opDifference(sdTorus82(pos, vec2(0.20, 0.1)), sdCylinder (opRepeat (vec3 (atan(pos.x, pos.z)/6.2831 + 0.1*aTime,pos.y,0.02+0.5*length(pos)),vec3(0.05, 1.0, 0.05)),vec2(0.02, 0.6)));
}

float
sdBallyBall (vec3 pos)
{
  return 0.7 * sdSphere(pos, 0.2) + 0.03 * sin(50.0 * pos.x) * sin(50.0 * pos.y + 8.0 * aTime) * sin(50.0 * pos.z);
}

float
sdTwistedTorus (vec3 pos, float angle)
{
  return 0.5 * sdTorus(opTwist(pos, angle), vec2(0.20, 0.05));
}

const vec3 p01 = vec3(-2.0, 0.25, -2.0);
const vec3 p02 = vec3(-1.0, 0.25, -2.0);
const vec3 p03 = vec3(0.0, 0.25, -2.0);
const vec3 p04 = vec3(1.0, 0.25, -2.0);
const vec3 p05 = vec3(2.0, 0.10, -2.0);

const vec3 p11 = vec3(-2.0, 0.25, -1.0);
const vec3 p12 = vec3(-1.0, 0.50, -1.0);
const vec3 p13 = vec3(0.0, 0.25, -1.0);
const vec3 p14 = vec3(1.0, 0.25, -1.0);
const vec3 p15 = vec3(2.0, 0.25, -1.0);

const vec3 p21 = vec3(-2.0, 0.20, 0.0);
const vec3 p22 = vec3(-1.0, 0.20, 0.0);
const vec3 p23 = vec3(0.0, 0.25, 0.0);
const vec3 p24 = vec3(1.0, 0.25, 0.0);
const vec3 p25 = vec3(2.0, 0.25, 0.0);

const vec3 p31 = vec3(-2.0, 0.20, 1.0);
const vec3 p32 = vec3(-1.0, 0.25, 1.0);
const vec3 p33 = vec3(0.0, 0.25, 1.0);
const vec3 p34 = vec3(1.0, 0.25, 1.0);
const vec3 p35 = vec3(2.0, 0.25, 1.0);

const vec3 p41 = vec3(-2.0, 0.25, 2.0);
const vec3 p42 = vec3(-1.0, 0.25, 2.0);
const vec3 p43 = vec3(0.0, 0.25, 2.0);
const vec3 p44 = vec3(1.0, 0.25, 2.0);
const vec3 p45 = vec3(2.0, 0.30, 2.0);

vec2
map (vec3 pos)
{
  vec3 r1, r2;

//vec2 res = vec2 (sdPlane1(pos), 1.0);
  vec2 res = vec2(sdPlane(pos, vec4(0, 1, 0, 0)), 1.0);

  res = opU(res, vec2(sdWaveSphere(pos - p01, 0.18, 10, sinTime * 0.4), 99));

  r1 = rotateY(pos - p02, aTime * 0.5);
//res = opU(res, vec2( sdDodecahedron (r1, 0.2), 55));
  res = opU(res, vec2(pipeDodecahedron(r1, 0.12, 0.02), 55));

  r1 = rotateX(pos - p03, sinTime * 0.8);
//res = opU(res, vec2( sdIcosDodecaStar1 (r1, 0.18), 177));
  res = opU(res, sdIcosDodecaStar2(r1, 0.18));

  r1 = rotateZ(pos - p04, sinTime * 0.8);
  res = opU(res, vec2(sdIcosahedron(r1, 0.16), 222));

  float h1 = 0.1 + 0.25 * abs(sinTime);
//  res = opU(res, vec2( sdPyramid4  (pos-p05, vec3(0.8, 0.6, h1) ), 8));
//  res = opU(res, vec2( sdPyramid  (pos-p05, h1 ), 8));

  res = opU(res, vec2(sdCylinder(pos - p05, vec2(0.12, h1)), 130));

  r1 = rotateY(pos - p11, aTime * 0.5);
  res = opU(res, vec2(sdTrianglePrism(r1, 0.1 * sinTime + 0.25, 0.05), 44));

  r1 = rotateX(pos - p12, sinTime * 0.8);
  res = opU(res, vec2(sdCone(r1, vec3(0.8, 0.5 + 0.2 * sinTime, 0.3)), 222));

  res = opU(res, vec2(sdBlob(pos - p13, -0.5 - 0.45 * sinTime), 244));

  r1 = rotateY(pos - p14, aTime * 0.5);
  res = opU(res, Grid2Colors(r1));

  vec3 a = vec3(0.1, 0.0, 0.0);
  float sr1 = 0.1 + 0.05 * sinTime;
  vec3 b = vec3(-0.1, 0.3, 0.1);
  float sr2 = 0.05;
  res = opU(res, vec2(sdRoundCone(pos - p15, a, b, sr1, sr2), 51.7));

  res = opU(res, vec2(CrossInSphere(pos - p21, 0.2 + 0.05 * sinTime, 0.05), 51));

  r1 = p22 + rotateY(vec3(-0.22, 0, 0), aTime);
  r2 = p22 - rotateY(vec3(-0.22, 0, 0), aTime);
  res = opU(res, vec2(sdCapsule(pos, r1, r2, 0.1), 32));

  r1 = vec3(0.0, 0.1 + 0.1 * sinTime, 0.0);
  res = opU(res, vec2(sdSphere(pos - p23 - r1, 0.25), 111));

  res = opU(res, vec2(sdBox(pos - p24, vec3(0.20)), 3));

  res = opU(res, vec2(sdEgg1(pos - p24 - r1, 0.21), 47));

  h1 = 0.2 + 0.06 * sinTime;
//  res = opU(res, vec2( sdEgg2      (pos-p25, 0.18+h1), 47));

  res = opU(res, vec2(sdRoundCone_Egg(pos - p25, vec3(0., 0.3 - h1, 0.), vec3(0., -0.2, 0.), 0.18, 0.1), 47));

  res = opU(res, vec2(sdBoxMinusSphere(pos - p31, 0.25), 13));

  r1 = rotateX(pos - p32, aTime * 0.5);
  res = opU(res, vec2(sdHexagonalPrism(r1, 0.25, 0.05), 17.0));

  r1 = rotateX(pos - p33, sinTime * 1.2);
  r1 = rotateZ(r1, aTime * 2.0);
  res = opU(res, vec2(sdTorus(r1, vec2(0.20, 0.05)), 25.0));

  res = opU(res, vec2(udRoundBox(pos - p34, vec3(0.12), 0.05 + 0.05 * +sinTime), 77));

  res = opU(res, vec2(sdBallyBall(pos - p35), 65.0));

  res = opU(res, vec2(sdTwistedTorus(pos - p41, 6.0 * sinTime), 46.7));

  r1 = rotateY(pos - p42, aTime * 0.25);
  res = opU(res, vec2(sdTorus88(r1, vec2(0.20, 0.05)), 43.0));

  res = opU(res, vec2(sdTorus82(pos - p43, vec2(0.20 - 0.05 * sinTime, 0.05)), 77.0));

  res = opU(res, vec2(sdRackWheel(pos - p44), 51));

  r1 = rotateY(pos - p45, aTime * 0.25);
  res = opU(res, vec2(sdCylinder6(r1, vec3(0.1, 0.05, 0.25)), 12));

  return res;
}

vec2
castRay (vec3 ro, vec3 rd)
{
  float tmin = 0.5;
  float tmax = 12.0;

  float tp1 = (0.0 - ro.y) / rd.y;
  if (tp1 > 0.0)
    tmax = min(tmax, tp1);
  float tp2 = (1.6 - ro.y) / rd.y;
  if (tp2 > 0.0)
  {
    if (ro.y > 1.6)
      tmin = max(tmin, tp2);
    else
      tmax = min(tmax, tp2);
  }

  float t = tmin;
  float m = -1.0;
  for (int i = 0; i < 64; i++)
  {
    float precis = 0.0004 * t;
    vec2 res = map(ro + rd * t);
    if (res.x < precis || t > tmax)
      break;
    t += res.x;
    m = res.y;
  }

  if (t > tmax)
    m = -1.0;
  return vec2(t, m);
}

#define ZERO (min(iFrame,0))
float
calcSoftshadow (vec3 ro, vec3 rd, float mint, float tmax)
{
  float tp = (0.8 - ro.y) / rd.y;
  if (tp > 0.0)
    tmax = min(tmax, tp);

  float res = 1.0;
  float t = mint;
  for (int i = ZERO; i < 16; i++)
  {
    float h = map(ro + rd * t).x;
    res = min(res, 8.0 * h / t);
    t += clamp(h, 0.02, 0.10);
    if (res < 0.001 || t > tmax)
      break;
  }
  return clamp(res, 0.0, 1.0);
}

vec3
calcNormal (vec3 pos)
{
  const vec2 e = vec2(0.0003, -0.0003);
  return normalize(e.xyy * map(pos + e.xyy).x +
                   e.yyx * map(pos + e.yyx).x +
                   e.yxy * map(pos + e.yxy).x +
                   e.xxx * map(pos + e.xxx).x);
}

float
calcAO (vec3 pos, vec3 nor)
{
  float occ = 0.0;
  float sca = 1.0;
  for (int i = 0; i < 4; i++)
  {
    float hr = 0.01 + 0.12 * float(i) / 4.0;
    vec3 aopos = nor * hr + pos;
    float dd = map(aopos).x;
    occ += -(dd - hr) * sca;
    sca *= 0.95;
  }
  return clamp(1.0 - 3.0 * occ, 0.0, 1.0);
}

float
checkersGradBox (vec2 p)
{
  vec2 w = fwidth(p) + 0.001;
  vec2 i = 2.0 * (abs(fract((p - 0.5 * w) * 0.5) - 0.5) - abs(fract((p + 0.5 * w) * 0.5) - 0.5)) / w;
  return 0.5 - 0.5 * i.x * i.y;
}

float
HexagonalGrid (vec2 position, float gridSize, float gridThickness)
{
  vec2 pos = position / gridSize;
  pos.x *= 0.57735 * 2.0;
  pos.y += 0.5 * mod(floor(pos.x), 2.0);
  pos = abs(fract(pos) - 0.5);
  float d = abs(max(pos.x * 1.5 + pos.y, pos.y * 2.0) - 1.0);
  return smoothstep(0.0, gridThickness, d);
}

const vec3 skyColor = vec3(0.7, 0.9, 1.0);
const vec3 planeColor = vec3(0.8, 0.7, 0.6);
vec3
render (vec3 ro, vec3 rd)
{
  vec3 col = skyColor;

  vec2 res = castRay(ro, rd);

  float t = res.x;
  float m = res.y;
  if (m > 0.0)
  {
    vec3 pos = ro + t * rd;
    vec3 nor = calcNormal(pos);
    vec3 ref = reflect(rd, nor);

    col = 0.45 + 0.35 * sin(vec3(0.05, 0.08, 0.10) * (m - 1.0));

    if (m == 1.0)
    {
//    float f = mod( floor(5.0*pos.z) + floor(5.0*pos.x), 2.0); // simple checkers
//    float f = checkersGradBox( 5.0*pos.xz );
//    col = 0.3 + f*vec3(0.1);
      float f = HexagonalGrid(4. * pos.xz, 0.8, 0.2);
      col = 0.2 + 0.2 * f * planeColor;
    }

    float occ = calcAO(pos, nor);
    vec3 lig = normalize(vec3(-0.4, 0.7, -0.6));
    vec3 hal = normalize(lig - rd);
    float amb = clamp(0.5 + 0.5 * nor.y, 0.0, 1.0);
    float dif = clamp(dot(nor, lig), 0.0, 1.0);
    float bac = clamp(dot(nor, normalize(vec3(-lig.x, 0.0, -lig.z))), 0.0, 1.0) * clamp(1.0 - pos.y, 0.0, 1.0);
    float dom = smoothstep(-0.2, 0.2, ref.y);
    float fre = pow(clamp(1.0 + dot(nor, rd), 0.0, 1.0), 2.0);

    dif *= calcSoftshadow(pos, lig, 0.02, 2.5);
    dom *= calcSoftshadow(pos, ref, 0.02, 2.5);

    float spe = pow(clamp(dot(nor, hal), 0.0, 1.0), 16.0) * dif * (0.04 + 0.96 * pow(clamp(1.0 + dot(hal, rd), 0.0, 1.0), 5.0));

    vec3 lin = vec3(0.0);
    lin += 1.40 * dif * vec3(1.00, 0.80, 0.55);
    lin += 0.20 * amb * vec3(0.40, 0.60, 1.00) * occ;
    lin += 0.40 * dom * vec3(0.40, 0.60, 1.00) * occ;
    lin += 0.50 * bac * vec3(0.25, 0.25, 0.25) * occ;
    lin += 0.25 * fre * vec3(1.00, 1.00, 1.00) * occ;
    col = col * lin;
    col += 9.00 * spe * vec3(1.00, 0.90, 0.70);

    col = mix(col, vec3(0.8, 0.9, 1.0), 1.0 - exp(-0.0002 * t * t * t));
  }
  return vec3(clamp(col, 0.0, 1.0));
}

mat3
setCamera (vec3 ro, vec3 ta, float cr)
{
  vec3 cw = normalize(ta - ro);
  vec3 cp = vec3(sin(cr), cos(cr), 0.0);
  vec3 cu = normalize(cross(cw, cp));
  vec3 cv = normalize(cross(cu, cw));
  return mat3(cu, cv, cw);
}

void
main ()
{
  vec2 mp = iMouse.xy / iResolution.xy;
  float time = 15.0 + iTime;
  float rtime = ROTATE_SCENE ? 0.1 * time : 0.0;
  aTime = ANIMATE_OBJECTS ? iTime : 0.0;
  sinTime = sin(aTime);

  float mx = 2.0 * PI * mp.x;
  vec3 ta = vec3(0.0, -0.1, 0.0);
  vec3 ro = vec3(4.0 * cos(rtime + mx), 1.0 + 2.6 * mp.y, 4.2 * sin(rtime + mx));
  mat3 ca = setCamera(ro, ta, 0.0);
//  mat3 ca = setCamera( ro, ta, 0.1*sin(iTime) );

  vec3 tot = vec3(0.0);
#if AA>1
  for (int m = ZERO; m < AA; m++)
    for (int n = ZERO; n < AA; n++)
    {
      vec2 o = vec2(float(m), float(n)) / float(AA) - 0.5;
      vec2 p = (-iResolution.xy + 2.0 * (gl_FragCoord.xy + o)) / iResolution.y;
#else
      vec2 p = (-iResolution.xy + 2.0 * gl_FragCoord.xy)/iResolution.y;
#endif

      vec3 rd = ca * normalize(vec3(p.xy, 2.0));

      vec3 col = render(ro, rd);

      col = pow(col, vec3(0.4545));

      tot += col;
#if AA>1
    }
  tot /= float(AA * AA);
#endif

  gl_FragColor = vec4(tot, 1.0);
}
