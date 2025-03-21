uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
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

#define S_BIG  1
#if S_BIG
const int nBall = 512;
#else
const int nBall = 216;
#endif

vec4 qtVu;
float nStep, cylRad, cylLen, angV, fOvlap, fricN, fricS, fricT, delT;
const float txRow = 128.;
const float pi = 3.14159;

#define VAR_ZERO min (iFrame, 0)

void PairInt (int mId, vec3 rm, vec3 vm, vec3 wm, float diam, inout vec3 am, inout vec3 wam)
{
  vec4 p;
  vec3 rmN, vmN, wmN, dr, dv;
  float diamN, rSep, diamAv, h, fc, ft;
  for (int n = VAR_ZERO; n < nBall; n ++) {
    p = Loadv4 (4 + 4 * n);
    rmN = p.xyz;
    diamN = p.w;
    dr = rm - rmN;
    rSep = length (dr);
    diamAv = 0.5 * (diam + diamN);
    if (n != mId && rSep < diamAv) {
      fc = fOvlap * (diamAv / rSep - 1.);
      vmN = Loadv4 (4 + 4 * n + 1).xyz;
      wmN = Loadv4 (4 + 4 * n + 3).xyz;
      dv = vm - vmN;
      h = dot (dr, dv) / (rSep * rSep);
      fc = max (fc - fricN * h, 0.);
      am += fc * dr;
      dv -= h * dr + cross ((diam * wm + diamN * wmN) / (diam + diamN), dr);
      ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
      am -= ft * dv;
      wam += (ft / rSep) * cross (dr, dv);
    }
  }
}

void BdyInt (vec3 rm, vec3 vm, vec3 wm, float diam, inout vec3 am, inout vec3 wam)
{
  vec3 dr, dv;
  float rSep, diamAv, h, fc, ft;
  diamAv = 0.5 * (diam + 1.);
  rSep = cylRad - length (rm.xz);
  if (rSep < diamAv) {
    dr = - rSep * normalize (vec3 (rm.x, 0., rm.z));
    fc = fOvlap * (diamAv / rSep - 1.);
    dv = vm - angV * cylRad * normalize (vec3 (rm.z, 0., - rm.x));
    h = dot (dr, dv) / (rSep * rSep);
    fc = max (fc - fricN * h, 0.);
    am += fc * dr;
    dv -= h * dr + cross (wm, dr);
    ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
    am -= ft * dv;
    wam += (ft / rSep) * cross (dr, dv);
  }
  dr.y = abs (rm.y) - cylLen;
  rSep = abs (dr.y);
  if (rSep < diamAv) {
    dr.y *= sign (rm.y);
    dr.xz = vec2 (0.);
    fc = fOvlap * (diamAv / rSep - 1.);
    dv = vm - angV * vec3 (- rm.z, 0., rm.x);
    h = dot (dr, dv) / (rSep * rSep);
    fc -= fricN * h;
    am += fc * dr;
  }
}

void Step (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam)
{
  vec4 p;
  vec3 am, wam;
  float fDamp, grav, ms;
  fOvlap = 1000.;
  fricN = 5.;
  fricT = 10.;
  fricS = 0.5;
  fDamp = 0.001;
  grav = 2.;
  p = Loadv4 (4 + 4 * mId);
  rm = p.xyz;
  diam = p.w;
  vm = Loadv4 (4 + 4 * mId + 1).xyz;
  qm = Loadv4 (4 + 4 * mId + 2);
  wm = Loadv4 (4 + 4 * mId + 3).xyz;
  ms = diam * diam * diam;
  am = vec3 (0.);
  wam = vec3 (0.);
  PairInt (mId, rm, vm, wm, diam, am, wam);
  BdyInt (rm, vm, wm, diam, am, wam);
  am -= vec3 (0., grav, 0.) * QtToRMat (qtVu) + fDamp * vm;
  wam -= fDamp * wm;
  vm += delT * am / ms;
  rm += delT * vm;
  wm += delT * wam / (0.2 * ms * diam);
  qm = normalize (QtMul (RMatToQt (LpStepMat (0.5 * delT * wm)), qm));
}

void Init (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam)
{
  vec3 e;
  float mIdf, nbEdge;
  nbEdge = floor (pow (float (nBall), 1./3.) + 0.1);
  mIdf = float (mId);
  rm = 0.9 * (floor (vec3 (mod (mIdf, nbEdge), mod (mIdf, nbEdge * nbEdge) / nbEdge,
      mIdf / (nbEdge * nbEdge))) - 0.5 * (nbEdge - 1.));
  vm = 2. * normalize (vec3 (Hashff (mIdf), Hashff (mIdf + 0.3), Hashff (mIdf + 0.6)) - 0.5);
  e = normalize (vec3 (Hashff (mIdf), Hashff (mIdf + 0.3), Hashff (mIdf + 0.6)));
  qm = EulToQt (e);
  wm = 2.5 * e;
  diam = 1. - 0.3 * Hashff (mIdf + 0.1);
}

void OrientVu (inout vec4 qtVu, vec4 mPtr, inout vec4 mPtrP, bool doInit)
{
  vec3 vq1, vq2;
  vec2 dm;
  float mFac;
  if (doInit) {
    qtVu = EulToQt (vec3 (0., 0.5 * pi, 0.15 * pi));
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
  vec4 mPtr, mPtrP, wgBx, stDat, p, qm;
  vec3 rm, vm, wm;
  vec2 iFrag, canvas, ust;
  float asp, diam, cylRot, angVMax, slVal;
  int mId, pxId, wgSel, wgReg;
  bool doInit;
  iFrag = floor (gl_FragCoord.xy);
  pxId = int (iFrag.x + txRow * iFrag.y);
  if (iFrag.x >= txRow || pxId >= 4 * nBall + 4) discard;
  canvas = iResolution.xy;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / iResolution.xy - 0.5;
  if (pxId >= 4) mId = (pxId - 4) / 4;
  else mId = -1;
  wgReg = -2;
  doInit = false;
  if (iFrame <= 5) doInit = true;
  asp = canvas.x / canvas.y;
  delT = 0.005;
  angVMax = 3.;
  if (doInit) {
    cylRad = floor (pow (float (nBall), 1./3.));
    cylLen = 0.5 * cylRad;
    nStep = 0.;
    cylRot = 0.;
    wgSel = -1;
    slVal = 0.25;
    angV = slVal * angVMax;
    OrientVu (qtVu, mPtr, mPtrP, true);
  } else {
    stDat = Loadv4 (0);
    cylRad = stDat.x;
    cylLen = stDat.y;
    angV = stDat.z;
    cylRot = stDat.w;
    qtVu = Loadv4 (1);
    mPtrP = Loadv4 (2);
    stDat = Loadv4 (3);
    nStep = stDat.x;
    slVal = stDat.y;
    wgSel = int (stDat.z);
    ++ nStep;
    cylRot = mod (cylRot + delT * angV, 2. * pi);
  }
  if (mPtr.z > 0.) {
    wgBx = vec4 (0.42 * asp, 0., 0.015 * asp, 0.2);
    ust = abs (mPtr.xy * vec2 (asp, 1.) - wgBx.xy) - wgBx.zw;
    if (max (ust.x, ust.y) < 0.) wgReg = 0;
    if (mPtrP.z <= 0.) wgSel = wgReg;
  } else {
    wgSel = -1;
    wgReg = -2;
  }
  if (wgSel < 0) {
    OrientVu (qtVu, mPtr, mPtrP, false);
  } else {
    if (wgSel == 0) {
      slVal = clamp (0.5 + 0.5 * (mPtr.y - wgBx.y) / wgBx.w, 0., 1.);
      angV = slVal * angVMax;
    }
    mPtrP = mPtr;
  }
  if (mId >= 0) {
    if (doInit) Init (mId, rm, vm, qm, wm, diam);
    else Step (mId, rm, vm, qm, wm, diam);
  }
  if (pxId == 0) stDat = vec4 (cylRad, cylLen, angV, cylRot);
  else if (pxId == 1) stDat = qtVu;
  else if (pxId == 2) stDat = mPtrP;
  else if (pxId == 3) stDat = vec4 (nStep, slVal, float (wgSel), 0.);
  else {
    mId = pxId - (4 + 4 * mId);
    p = (mId <= 1) ? ((mId == 0) ? vec4 (rm, diam) : vec4 (vm, 0.)) :
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
  return texture (txBuf, (vec2 (mod (fi, txRow), floor (fi / txRow)) + 0.5) / txSize);
}

void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord)
{
  vec2 d;
  float fi;
  fi = float (idVar);
  d = abs (fCoord - vec2 (mod (fi, txRow), floor (fi / txRow)) - 0.5);
  if (max (d.x, d.y) < 0.5) fCol = val;
}
