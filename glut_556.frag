#version 130

uniform float iTime;
uniform int iFrame;
uniform vec4 iDate;
uniform sampler2D iChannel0;
uniform vec2 iChannel0Resolution;

vec4 QtMul (vec4 q1, vec4 q2);
mat3 QtToRMat (vec4 q);
vec4 RMatToQt (mat3 m);
vec4 EulToQt (vec3 e);
mat3 LpStepMat (vec3 a);
float Hashff (float p);
vec4 Loadv4 (int idVar);
void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord);

float todCur, nStep, ringRad, grav;
const int nObj = 16;
const int nSphObj = 18;
const float txRow = 128.;
const float pi = 3.14159;

vec3 RSph (float s)
{
  vec3 r;
  float a;
  a = (2. * pi / float (nSphObj)) * s;
  r = vec3 ((ringRad - 0.5) * cos (a), (ringRad - 0.5) * sin (a), 0.);
  return r;
}

vec3 FcFun (vec3 dr, float rSep, vec3 dv)
{
  vec3 f;
  float vRel, fo, drv;
  const float fOvlap = 500., fricN = 0.1, fricT = 1., fricS = 0.5;
  fo = fOvlap * (1. / rSep - 1.);
  drv = dot (dr, dv) / (rSep * rSep);
  dv -= drv * dr;
  vRel = length (dv);
  fo = max (fo - fricN * drv, 0.);
  f = fo * dr;
  if (vRel > 0.001) f -= min (fricT, fricS * abs (fo) * rSep / vRel) * dv;
  return f;
}

void Step (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm)
{
  mat3 mRot, mRotN;
  vec3 rmN, vmN, wmN, dr, dv, rts, rtsN, rms, vms, fc, am, wam, rMom;
  float farSep, rSep, dt;
  dt = 0.01;
  rm = Loadv4 (4 * mId).xyz;
  vm = Loadv4 (4 * mId + 1).xyz;
  qm = Loadv4 (4 * mId + 2);
  wm = Loadv4 (4 * mId + 3).xyz;
  mRot = QtToRMat (qm);
  farSep = 2. * ringRad + 2.;
  am = vec3 (0.);
  wam = vec3 (0.);
  for (int n = 0; n < nObj; n ++) {
    rmN = Loadv4 (4 * n).xyz;
    if (n != mId && length (rm - rmN) < farSep) {
      vmN = Loadv4 (4 * n + 1).xyz;
      mRotN = QtToRMat (Loadv4 (4 * n + 2));
      wmN = Loadv4 (4 * n + 3).xyz;
      for (int j1 = 0; j1 < nSphObj; j1 ++) {
        rts = mRot * RSph (float (j1));
        rms = rm + rts;
        vms = vm + cross (wm, rts);
        dv = vms - vmN;
        fc = vec3 (0.);
        for (int j2 = 0; j2 < nSphObj; j2 ++) {
          rtsN = mRotN * RSph (float (j2));
          dr = rms - (rmN + rtsN);
          rSep = length (dr);
          if (rSep < 1.) fc += FcFun (dr, rSep, dv - cross (wmN, rtsN));
        }
        am += fc;
        wam += cross (rts, fc);
      }
    }
  }
  rMom = vec3 (0.);
  for (int j = 0; j < nSphObj; j ++) {
    rts = RSph (float (j));
    rMom += dot (rts, rts) - rts * rts + 1./6.;
    rts = mRot * rts;
    dr = rm + rts;
    rSep = abs (dr.y);
    if (rSep < 1.) {
      fc = FcFun (vec3 (0., dr.y, 0.), rSep, vm + cross (wm, rts));
      am += fc;
      wam += cross (rts, fc);
    }
  }
  rMom /= float (nSphObj);
  wam = mRot * (wam * mRot / rMom);
  am.y -=  grav;
  vm += dt * am;
  rm += dt * vm;
  wm += dt * wam;
  qm = normalize (QtMul (RMatToQt (LpStepMat (0.5 * dt * wm)), qm));
}

void Init (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm)
{
  float a, b, s, r;
  b = 2. * Hashff (todCur + float (mId)) - 1.;
  a = 2. * pi * (float (mId) + 1.) / float (nObj);
  s = mod (float (mId), 2.);
  r = 7. - 0.7 * s;
  rm = vec3 (r * cos (a), 11. + r * sin (a), 0.);
  qm = EulToQt (vec3 (0.5 * pi + a, 0.5 * pi * s, 0.5 * pi * s));
  vm = vec3 (- 3. * rm.xz / max (0.1, length (rm.xz)), 0.).xzy;
  wm = vec3 (0.2, 0.2, 3.) * b;
}

void main ()
{
  vec4 stDat, qm;
  vec3 rm, vm, wm;
  vec2 iFrag;
  float tCur, mxStep;
  int mId, pxId, kp;
  bool doInit;
  iFrag = floor (gl_FragCoord.xy);
  pxId = int (iFrag.x + txRow * iFrag.y);
  if (iFrag.x >= txRow || pxId >= 4 * nObj + 1) discard;
  tCur = iTime;
  todCur = iDate.w;
  mId = (pxId < 4 * nObj) ? mId = pxId / 4 : -1;
  doInit = false;
  mxStep = 2000.;
  if (iFrame <= 5) {
    doInit = true;
    ringRad = 2.5;
  } else {
    stDat = Loadv4 (4 * nObj);
    nStep = stDat.x;
    ringRad = stDat.z;
    ++ nStep;
    if (nStep > mxStep) doInit = true;
  }
  if (doInit) nStep = 0.;
  grav = (mod (nStep, mxStep) >= mxStep / 4. && mod (nStep, mxStep / 4.) < 15.) ? -40. : 10.;
  if (mId >= 0) {
    if (doInit) Init (mId, rm, vm, qm, wm);
    else Step (mId, rm, vm, qm, wm);
    kp = 4 * mId;
    if      (pxId == kp + 0) stDat = vec4 (rm, 0.);
    else if (pxId == kp + 1) stDat = vec4 (vm, 0.);
    else if (pxId == kp + 2) stDat = qm;
    else if (pxId == kp + 3) stDat = vec4 (wm, 0.);
  } else stDat = vec4 (nStep, mxStep, ringRad, 0.);
  Savev4 (pxId, stDat, gl_FragColor, gl_FragCoord.xy);
}

vec4 QtMul (vec4 q1, vec4 q2)
{
  return vec4 (
       q1.w * q2.x - q1.z * q2.y + q1.y * q2.z + q1.x * q2.w,
       q1.z * q2.x + q1.w * q2.y - q1.x * q2.z + q1.y * q2.w,
     - q1.y * q2.x + q1.x * q2.y + q1.w * q2.z + q1.z * q2.w,
     - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w);
}

mat3 QtToRMat (vec4 q) 
{
  mat3 m;
  float a1, a2, s;
  q = normalize (q);
  s = q.w * q.w - 0.5;
  m[0][0] = q.x * q.x + s;  m[1][1] = q.y * q.y + s;  m[2][2] = q.z * q.z + s;
  a1 = q.x * q.y;  a2 = q.z * q.w;  m[0][1] = a1 + a2;  m[1][0] = a1 - a2;
  a1 = q.x * q.z;  a2 = q.y * q.w;  m[2][0] = a1 + a2;  m[0][2] = a1 - a2;
  a1 = q.y * q.z;  a2 = q.x * q.w;  m[1][2] = a1 + a2;  m[2][1] = a1 - a2;
  return 2. * m;
}

vec4 RMatToQt (mat3 m)
{
  vec4 q;
  const float tol = 1e-6;
  q.w = 0.5 * sqrt (max (1. + m[0][0] + m[1][1] + m[2][2], 0.));
  if (abs (q.w) > tol) q.xyz =
     vec3 (m[1][2] - m[2][1], m[2][0] - m[0][2], m[0][1] - m[1][0]) / (4. * q.w);
  else {
    q.x = sqrt (max (0.5 * (1. + m[0][0]), 0.));
    if (abs (q.x) > tol) q.yz = vec2 (m[0][1], m[0][2]) / q.x;
    else {
      q.y = sqrt (max (0.5 * (1. + m[1][1]), 0.));
      if (abs (q.y) > tol) q.z = m[1][2] / q.y;
      else q.z = 1.;
    }
  }
  return normalize (q);
}

vec4 EulToQt (vec3 e)
{
  float a1, a2, a3, c1, s1;
  a1 = 0.5 * e.y;  a2 = 0.5 * (e.x - e.z);  a3 = 0.5 * (e.x + e.z);
  s1 = sin (a1);  c1 = cos (a1);
  return normalize (vec4 (s1 * cos (a2), s1 * sin (a2), c1 * sin (a3),
     c1 * cos (a3)));
}

mat3 LpStepMat (vec3 a)
{
  mat3 m1, m2;
  vec3 t, c, s;
  float b1, b2;
  t = 0.25 * a * a;
  c = (1. - t) / (1. + t);
  s = a / (1. + t);
  m1[0][0] = c.y * c.z;  m2[0][0] = c.y * c.z;
  b1 = s.x * s.y * c.z;  b2 = c.x * s.z;
  m1[0][1] = b1 + b2;  m2[1][0] = b1 - b2;
  b1 = c.x * s.y * c.z;  b2 = s.x * s.z;
  m1[0][2] = - b1 + b2;  m2[2][0] = b1 + b2;
  b1 = c.y * s.z;
  m1[1][0] = - b1;  m2[0][1] = b1;  
  b1 = s.x * s.y * s.z;  b2 = c.x * c.z;
  m1[1][1] = - b1 + b2;  m2[1][1] = b1 + b2; 
  b1 = c.x * s.y * s.z;  b2 = s.x * c.z;
  m1[1][2] = b1 + b2;  m2[2][1] = b1 - b2;
  m1[2][0] = s.y;  m2[0][2] = - s.y;
  b1 = s.x * c.y;
  m1[2][1] = - b1;  m2[1][2] = b1;
  b1 = c.x * c.y;
  m1[2][2] = b1;  m2[2][2] = b1;
  return m1 * m2;
}

float Hashff (float p)
{
  const float cHashM = 43758.54;
  return fract (sin (p) * cHashM);
}

#define txBuf iChannel0
#define txSize iChannel0Resolution

vec4 Loadv4 (int idVar)
{
  float fi;
  fi = float (idVar);
  return texture (txBuf, (vec2 (mod (fi, txRow), floor (fi / txRow)) + 0.5) /
     txSize);
}

void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord)
{
  vec2 d;
  float fi;
  fi = float (idVar);
  d = abs (fCoord - vec2 (mod (fi, txRow), floor (fi / txRow)) - 0.5);
  if (max (d.x, d.y) < 0.5) fCol = val;
}
