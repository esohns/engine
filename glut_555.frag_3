#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iDate;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform vec2 iChannel0Resolution;

vec4 QtMul (vec4 q1, vec4 q2);
mat3 QtToRMat (vec4 q);
vec4 RMatToQt (mat3 m);
vec4 EulToQt (vec3 e);
mat3 LpStepMat (vec3 a);
vec2 Rot2D (vec2 q, float a);
float Hashff (float p);
vec4 Loadv4 (int idVar);
void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord);

const ivec3 nbEdge = ivec3 (20, 2, 16);
const int nBall = nbEdge.x * nbEdge.y * nbEdge.z;

vec3 hbSize, gVec;
float nStep, fOvlap, fricN, fricS, fricT, delT, nBin, rSeed;
const float txRow = 128.;
const int txOff = 4;
const float pi = 3.14159;

#define VAR_ZERO min (iFrame, 0)

void PairInt (int mId, vec3 rm, vec3 vm, vec3 wm, float diam, inout vec3 am, inout vec3 wam)
{
  vec4 p;
  vec3 rmN, vmN, wmN, dr, dv;
  float diamN, rSep, diamAv, h, fc, ft;
  for (int n = VAR_ZERO; n < nBall; n ++) {
    p = Loadv4 (txOff + 4 * n);
    rmN = p.xyz;
    diamN = p.w;
    dr = rm - rmN;
    rSep = length (dr);
    diamAv = 0.5 * (diam + diamN);
    if (n != mId && rSep < diamAv) {
      fc = fOvlap * (diamAv / rSep - 1.);
      vmN = Loadv4 (txOff + 4 * n + 1).xyz;
      wmN = Loadv4 (txOff + 4 * n + 3).xyz;
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
  vec4 drw;
  vec3 dr, dv;
  float rSep, diamAv, h, fc, ft;
  diamAv = 0.5 * (diam + 1.);
  drw = vec4 ((hbSize - abs (rm)) * (1. - 2. * step (0., rm)), 0.);
  for (int nf = VAR_ZERO; nf < 3; nf ++) {
    dr = (nf == 1) ? drw.wyw : ((nf == 0) ? drw.xww : drw.wwz);
    rSep = length (dr);
    if (rSep < diamAv) {
      fc = fOvlap * (diamAv / rSep - 1.);
      dv = vm;
      h = dot (dr, dv) / (rSep * rSep);
      fc = max (fc - fricN * h, 0.);
      am += fc * dr;
      dv -= h * dr + cross (wm, dr);
      ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
      am -= ft * dv;
      wam += (ft / rSep) * cross (dr, dv);
    }
  }
}

void ObsInt (vec3 rm, vec3 vm, vec3 wm, float diam, inout vec3 am, inout vec3 wam)
{
  vec3 dr, dv;
  vec2 cEdge;
  float cGap, hGap, hHt, vHt, rSep, diamAv, h, fc, ft, fHoriz;
  fHoriz = 0.5;
  cGap = 2.5;
  hGap = 0.2;
  hHt = 0.5;
  cEdge = vec2 (3.5, 4.5);
  vHt = 8.;
  diamAv = 0.5 * (diam + 1.);
  dr.y = 0.;
  dr.xz = rm.xz;
  dr.z += 0.25 * cGap * sign (mod (floor (dr.x / cGap + 0.5), 2.) - 0.5);
  if (abs (dr.x) < cEdge.x * cGap && abs (dr.z) < cEdge.y * cGap)
     dr.xz = mod (dr.xz + 0.5 * cGap, cGap) - 0.5 * cGap;
  rSep = length (dr);
  if (rSep < diamAv) {
    fc = fOvlap * (diamAv / rSep - 1.);
    dv = vm;
    h = dot (dr, dv) / (rSep * rSep);
    fc = max (fc - fricN * h, 0.);
    am += fc * dr;
    dv -= h * dr + cross (wm, dr);
    ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
    am -= ft * dv;
    wam += (ft / rSep) * cross (dr, dv);
  }
  dr.xz = rm.xz;
  if (mod (nBin, 2.) != 0.) dr.z += hbSize.z / nBin;
  dr.z = mod (dr.z + hbSize.z / nBin, 2. * hbSize.z / nBin) - hbSize.z / nBin;
  dr.x = max (dr.x + hbSize.x - vHt, 0.);
  rSep = length (dr);
  if (rSep < diamAv) {
    fc = fOvlap * (diamAv / rSep - 1.);
    dv = vm;
    h = dot (dr, dv) / (rSep * rSep);
    fc = max (fc - fricN * h, 0.);
    am += fc * dr;
    dv -= h * dr + cross (wm, dr);
    ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
    am -= ft * dv;
    wam += (ft / rSep) * cross (dr, dv);
  }
  dr.xz = rm.xz;
  dr.x -= hHt * hbSize.x;
  if (dr.x > 0.) am.z -= fHoriz * sign (rm.z);
  else am.z += 0.5 * fHoriz * sign (rm.z);
  dr.z = min (abs (dr.z) - hGap * hbSize.z, 0.) * sign (dr.z);
  rSep = length (dr);
  if (rSep < diamAv) {
    fc = fOvlap * (diamAv / rSep - 1.);
    dv = vm;
    h = dot (dr, dv) / (rSep * rSep);
    fc = max (fc - fricN * h, 0.);
    am += fc * dr;
    dv -= h * dr + cross (wm, dr);
    ft = min (fricT, fricS * abs (fc) * rSep / max (0.001, length (dv)));
    am -= ft * dv;
    wam += (ft / rSep) * cross (dr, dv);
  }
}

void Step (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam, out float bCol)
{
  vec4 p;
  vec3 am, wam;
  float fDamp, grav, ms;
  fOvlap = 1000.;
  fricN = 10.;
  fricT = 10.;
  fricS = 0.5;
  fDamp = 0.001;
  grav = 5.;
  p = Loadv4 (txOff + 4 * mId);
  rm = p.xyz;
  diam = p.w;
  p = Loadv4 (txOff + 4 * mId + 1);
  vm = p.xyz;
  bCol = p.w;
  qm = Loadv4 (txOff + 4 * mId + 2);
  wm = Loadv4 (txOff + 4 * mId + 3).xyz;
  ms = diam * diam * diam;
  am = vec3 (0.);
  wam = vec3 (0.);
  PairInt (mId, rm, vm, wm, diam, am, wam);
  BdyInt (rm, vm, wm, diam, am, wam);
  ObsInt (rm, vm, wm, diam, am, wam);
  am -= grav * gVec + fDamp * vm;
  wam -= fDamp * wm;
  vm += delT * am / ms;
  rm += delT * vm;
  wm += delT * wam / (0.2 * ms * diam);
  qm = normalize (QtMul (RMatToQt (LpStepMat (0.5 * delT * wm)), qm));
}

void Init (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam, out float bCol)
{
  vec3 e;
  float mIdf, ha;
  mIdf = float (mId);
  e = vec3 (0.5 * hbSize.x, hbSize.y, 2. * hbSize.z);
  rm = 0.8 * (floor (vec3 (mod (mIdf, e.x), mod (mIdf, e.x * e.y) / e.x,
     mIdf / (e.x * e.y))) - 0.5 * (e - 1.));
  rm.x += hbSize.x - 4.5;
  ha = mIdf + rSeed;
  vm = 0.05 * normalize (vec3 (Hashff (ha), Hashff (ha + 0.3), Hashff (ha + 0.6)) - 0.5);
  e = normalize (vec3 (Hashff (ha), Hashff (ha + 0.3), Hashff (ha + 0.6)));
  qm = EulToQt (e);
  wm = 2.5 * e;
  diam = 1. - 0.3 * Hashff (ha + 0.1);
  bCol = floor (nBin * Hashff (ha + 1.1));
}

void main ()
{
  vec4 mPtr, stDat, p, qm;
  vec3 rm, vm, wm, blkAng;
  vec2 iFrag, canvas;
  float el, az, diam, bCol, bc, vMax, nCyc, todCur, todInit;
  int mId, pxId;
  bool doInit;
  iFrag = floor (gl_FragCoord.xy);
  pxId = int (iFrag.x + txRow * iFrag.y);
  if (iFrag.x >= txRow || pxId >= 4 * nBall + txOff) discard;
  canvas = iResolution.xy;
  todCur = iDate.w;
  mPtr = iMouse;
  mPtr.xy = mPtr.xy / iResolution.xy - 0.5;
  if (pxId >= txOff) mId = (pxId - txOff) / 4;
  else mId = -1;
  doInit = false;
  if (iFrame <= 5) doInit = true;
  delT = 0.005;
  if (doInit) {
    hbSize = vec3 (nbEdge);
    nStep = 0.;
    nCyc = 0.;
    el = 0.;
    az = 0.;
    todInit = todCur;
    nBin = 4. + floor (3. * Hashff (mod (todInit, 1.)));
    blkAng = vec3 (0., -0.5 * pi, 1.5 * pi);
  } else {
    stDat = Loadv4 (0);
    hbSize = stDat.xyz;
    stDat = Loadv4 (1);
    blkAng = stDat.xyz;
    nCyc = stDat.w;
    stDat = Loadv4 (2);
    nBin = stDat.w;
    stDat = Loadv4 (3);
    el = stDat.x;
    az = stDat.y;
    todInit = stDat.z;
    nStep = stDat.w;
    ++ nStep;
  }
  if (mPtr.z > 0.) {
    az = 2. * pi * mPtr.x;
    el = clamp (0.5 * pi * mPtr.y, -0.2 * pi, 0.);
  }
  gVec = vec3 (0., Rot2D (vec2 (1., 0.), blkAng.x));
  gVec.xy = Rot2D (gVec.xy, blkAng.z);
  rSeed = 11.1 * mod (todInit, 1.);
  if (mId >= 0) {
    if (doInit) Init (mId, rm, vm, qm, wm, diam, bCol);
    else Step (mId, rm, vm, qm, wm, diam, bCol);
  }
  if (nStep > 1000.) {
    vMax = 0.;
    for (int n = VAR_ZERO; n < nBall; n ++) vMax = max (vMax, length (Loadv4 (txOff + 4 * n + 1).xyz));
    if (vMax < 0.5) {
      nStep = 0.;
      ++ nCyc;
      if (mod (nCyc, 2.) > 0.) bc = floor (nBin * (0.5 * rm.z / hbSize.z + 0.5));
      else todInit = todCur;
      rSeed = 11.1 * mod (todInit, 1.);
      Init (mId, rm, vm, qm, wm, diam, bCol);
      if (mod (nCyc, 2.) > 0.) bCol = bc;
      else nBin = 4. + floor (3. * Hashff (mod (todInit, 1.)));
    }
  }
  if (pxId == 0) stDat = vec4 (hbSize, 0.);
  else if (pxId == 1) stDat = vec4 (blkAng, nCyc);
  else if (pxId == 2) stDat = vec4 (mPtr.xyz, nBin);
  else if (pxId == 3) stDat = vec4 (el, az, todInit, nStep);
  else {
    mId = pxId - (txOff + 4 * mId);
    p = (mId <= 1) ? ((mId == 0) ? vec4 (rm, diam) : vec4 (vm, bCol)) :
       ((mId == 2) ? qm : vec4 (wm, 0.));
  }
  Savev4 (pxId, ((pxId >= txOff) ? p : stDat), gl_FragColor, gl_FragCoord.xy);
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

vec2 Rot2D (vec2 q, float a)
{
  vec2 cs;
  cs = sin (a + vec2 (0.5 * pi, 0.));
  return vec2 (dot (q, vec2 (cs.x, - cs.y)), dot (q.yx, cs));
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
