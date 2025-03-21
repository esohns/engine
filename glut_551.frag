uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform vec2 iChannel0Resolution;

vec4 QtMul (vec4 q1, vec4 q2);
mat3 QtToRMat (vec4 q);
vec4 RMatToQt (mat3 m);
vec4 EulToQt (vec3 e);
vec3 QtToEul (vec4 q);
mat3 LpStepMat (vec3 a);
float SmoothBump (float lo, float hi, float w, float x);
float Hashff (float p);
vec4 Loadv4 (int idVar);
void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord);

#define S_BIG  1
#if S_BIG
const int nBall = 512;
#else
const int nBall = 216;
#endif

vec4 qtVu;
vec3 hbSize;
float nStep;
const float txRow = 128.;
const float pi = 3.14159;

void Step (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm,
   out float rad)
{
  vec4 p, drw;
  vec3 rmN, vmN, wmN, dr, dv, am, wam;
  float fOvlap, fricN, fricT, fricS, fricSW, fDamp, grav, rSep, radN, radAv,
     fc, ft, ms, h, dt;
  fOvlap = 1000.;
  fricN = 5.;
  fricS = 1.;
  fricSW = 5.;
  fricT = 0.5;
  fDamp = 0.01;
  grav = 1.5 * (1. - 2.5 * SmoothBump (0.5, 0.55, 0.01, mod (nStep / 2000., 1.)));
  p = Loadv4 (4 + 4 * mId);
  rm = p.xyz;
  rad = p.w;
  vm = Loadv4 (4 + 4 * mId + 1).xyz;
  qm = Loadv4 (4 + 4 * mId + 2);
  wm = Loadv4 (4 + 4 * mId + 3).xyz;
  ms = rad * rad * rad;
  am = vec3 (0.);
  wam = vec3 (0.);
  for (int n = 0; n < nBall; n ++) {
    p = Loadv4 (4 + 4 * n);
    rmN = p.xyz;
    radN = p.w;
    dr = rm - rmN;
    rSep = length (dr);
    radAv = 0.5 * (rad + radN);
    if (n != mId && rSep < radAv) {
      fc = fOvlap * (radAv / rSep - 1.);
      vmN = Loadv4 (4 + 4 * n + 1).xyz;
      wmN = Loadv4 (4 + 4 * n + 3).xyz;
      dv = vm - vmN;
      h = dot (dr, dv) / (rSep * rSep);
      fc = max (fc - fricN * h, 0.);
      am += fc * dr;
      dv -= h * dr + cross ((rad * wm + radN * wmN) / (rad + radN), dr);
      ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
      am -= ft * dv;
      wam += (ft / rSep) * cross (dr, dv);
    }
  }
  radAv = 0.5 * (rad + 1.);
  drw = vec4 ((hbSize - abs (rm)) * (1. - 2. * step (0., rm)), 0.);
  for (int nf = 0; nf < 3; nf ++) {
    dr = (nf == 1) ? drw.wyw : ((nf == 0) ? drw.xww : drw.wwz);
    rSep = length (dr);
    if (rSep < radAv) {
      fc = fOvlap * (radAv / rSep - 1.);
      dv = vm;
      h = dot (dr, dv) / (rSep * rSep);
      fc = max (fc - fricN * h, 0.);
      am += fc * dr;
      dv -= h * dr + cross (wm, dr);
      ft = min (fricT, fricSW * abs (fc) * rSep / max (0.001, length (dv)));
      am -= ft * dv;
      wam += (ft / rSep) * cross (dr, dv);
    }
  }
  am -= vec3 (0., grav, 0.) * QtToRMat (qtVu) + fDamp * vm;
  dt = 0.01;
  vm += dt * am / ms;
  rm += dt * vm;
  wm += dt * wam / (0.1 * ms * rad);
  qm = normalize (QtMul (RMatToQt (LpStepMat (0.5 * dt * wm)), qm));
}

void Init (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm,
   out float rad)
{
  vec3 e;
  float mIdf, nbEdge;
  nbEdge = floor (pow (float (nBall), 1./3.) + 0.1);
  mIdf = float (mId);
  rm = 0.9 * (floor (vec3 (mod (mIdf, nbEdge),
      mod (mIdf, nbEdge * nbEdge) / nbEdge,
      mIdf / (nbEdge * nbEdge))) - 0.5 * (nbEdge - 1.));
  vm = 2. * normalize (vec3 (Hashff (mIdf), Hashff (mIdf + 0.3),
     Hashff (mIdf + 0.6)) - 0.5);
  e = normalize (vec3 (Hashff (mIdf), Hashff (mIdf + 0.3),
     Hashff (mIdf + 0.6)));
  qm = EulToQt (e);
  wm = 2.5 * e;
  rad = 1. - 0.3 * Hashff (mIdf + 0.1);
}

void OrientVu (inout vec4 qtVu, vec4 mPtr, inout vec4 mPtrP, bool doInit)
{
  vec3 vq1, vq2;
  vec2 dm;
  float mFac;
  if (doInit) {
    qtVu = EulToQt (vec3 (0., 0.5 * pi, 0.));
    mPtrP = vec4 (99., 0., -1., 0.);
  } else {
    if (mPtr.z > 0.) {
      if (mPtrP.x == 99.) mPtrP = mPtr;
      mFac = 1.5;
      dm = - mFac * mPtrP.xy;
      vq1 = vec3 (dm, sqrt (max (1. - dot (dm, dm), 0.)));
      dm = - mFac * mPtr.xy;
      vq2 = vec3 (dm, sqrt (max (1. - dot (dm, dm), 0.)));
      qtVu = normalize (QtMul (vec4 (cross (vq1, vq2), dot (vq1, vq2)), qtVu));
      mPtrP = mPtr;
    } else mPtrP = vec4 (99., 0., -1., 0.);
  }
}

void main ()
{
  vec4 mPtr, mPtrP, stDat, p, qm;
  vec3 rm, vm, wm, euAng;
  vec2 iFrag;
  float tCur, tCurP, rad, szFac;
  int mId, pxId;
  bool doInit;
  iFrag = floor (gl_FragCoord.xy);
  pxId = int (iFrag.x + txRow * iFrag.y);
  if (iFrag.x >= txRow || pxId >= 4 * nBall + 4) discard;
  tCur = iTime;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / iResolution.xy - 0.5;
  if (pxId >= 4) mId = (pxId - 4) / 4;
  else mId = -1;
  doInit = false;
  if (iFrame <= 5) doInit = true;
  if (doInit) {
    nStep = 0.;
    OrientVu (qtVu, mPtr, mPtrP, true);
  } else {
    qtVu = Loadv4 (1);
    mPtrP = Loadv4 (2);
    stDat = Loadv4 (3);
    nStep = stDat.x;
    tCurP = stDat.y;
    ++ nStep;
    euAng = QtToEul (qtVu);
    euAng.z += 0.001 * pi;
    if (mPtrP.z < 0.) euAng.x += 0.00055 * pi;
    qtVu = EulToQt (euAng);
    OrientVu (qtVu, mPtr, mPtrP, false);
  }
  hbSize = vec3 (0.8 * floor (pow (float (nBall), 1./3.)));
  szFac = 1. + 0.3 * sin (2. * pi * 0.0005 * nStep);
  hbSize.z *= szFac;
  hbSize.xy /= sqrt (szFac);
  if (mId >= 0) {
    if (doInit) Init (mId, rm, vm, qm, wm, rad);
    else Step (mId, rm, vm, qm, wm, rad);
  }
  if (pxId == 0) stDat = vec4 (hbSize, 0.);
  else if (pxId == 1) stDat = qtVu;
  else if (pxId == 2) stDat = mPtrP;
  else if (pxId == 3) stDat = vec4 (nStep, tCur, 0., 0.);
  else {
    mId = pxId - (4 + 4 * mId);
    p = (mId <= 1) ? ((mId == 0) ? vec4 (rm, rad) : vec4 (vm, 0.)) :
       ((mId == 2) ? qm : vec4 (wm, 0.));
  }
  Savev4 (pxId, ((pxId >= 4) ? p : stDat), gl_FragColor, gl_FragCoord.xy);
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

vec3 QtToEul (vec4 q)
{
  vec3 e;
  float s, t;
  t = dot (q.xy, q.xy);
  s = 2. * sqrt (max (t * (1. - t), 0.));
  e.y = atan (s, 1. - 2. * t);
  if (s != 0.) e.xz = 
     vec2 (atan (q.x * q.z + q.y * q.w, q.x * q.w - q.y * q.z),
           atan (q.x * q.z - q.y * q.w, q.x * q.w + q.y * q.z));
  else e.xz = vec2 (2. * atan (q.z, q.w), 0.);
  return e;
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

float SmoothBump (float lo, float hi, float w, float x)
{
  return (1. - smoothstep (hi - w, hi + w, x)) * smoothstep (lo - w, lo + w, x);
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
  return textureLod (txBuf, (vec2 (mod (fi, txRow), floor (fi / txRow)) + 0.5) /
     txSize, 0.0);
}

void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord)
{
  vec2 d;
  float fi;
  fi = float (idVar);
  d = abs (fCoord - vec2 (mod (fi, txRow), floor (fi / txRow)) - 0.5);
  if (max (d.x, d.y) < 0.5) fCol = val;
}
