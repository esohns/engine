uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform vec2 iChannel0Resolution;

vec4 QtMul (vec4 q1, vec4 q2);
vec4 RMatToQt (mat3 m);
vec4 EulToQt (vec3 e);
mat3 LpStepMat (vec3 a);
vec2 Rot2D (vec2 q, float a);
float Hashff (float p);
vec4 Loadv4 (int idVar);
void Savev4 (int idVar, vec4 val, inout vec4 fCol, vec2 fCoord);

vec3 hbSize, hbPos;
ivec3 nbEdge;
float nStep, fOvlap, fricN, fricS, fricT, dt;
int nFrame, nBall, txOff;
const float txRow = 128.;
const float pi = 3.1415927;

#define VAR_ZERO min (nFrame, 0)

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
      ft = min (fricT, fricS * abs (fc) * rSep / max (1e-3, length (dv)));
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
  rm -= hbPos;
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
      ft = min (fricT, fricS * abs (fc) * rSep / max (1e-3, length (dv)));
      am -= ft * dv;
      wam += (ft / rSep) * cross (dr, dv);
    }
  }
}

void Step (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam)
{
  vec4 p;
  vec3 am, wam;
  float fDamp, grav, ms;
  fOvlap = 1000.;
  fricN = 10.;
  fricT = 10.;
  fricS = 1.;
  fDamp = 0.005;
  grav = 5.;
  p = Loadv4 (txOff + 4 * mId);
  rm = p.xyz;
  diam = p.w;
  p = Loadv4 (txOff + 4 * mId + 1);
  vm = p.xyz;
  qm = Loadv4 (txOff + 4 * mId + 2);
  wm = Loadv4 (txOff + 4 * mId + 3).xyz;
  ms = diam * diam * diam;
  am = vec3 (0.);
  wam = vec3 (0.);
  PairInt (mId, rm, vm, wm, diam, am, wam);
  BdyInt (rm, vm, wm, diam, am, wam);
  am.y -= grav;
  am -= fDamp * vm;
  wam -= fDamp * wm;
  vm += dt * am / ms;
  rm += dt * vm;
  wm += dt * wam / (0.2 * ms * diam);
  qm = normalize (QtMul (RMatToQt (LpStepMat (0.5 * dt * wm)), qm));
}

void Init (int mId, out vec3 rm, out vec3 vm, out vec4 qm, out vec3 wm, out float diam)
{
  vec3 e;
  vec2 w;
  float mIdf;
  e = vec3 (nbEdge);
  mIdf = float (mId);
  rm = 0.95 * (floor (vec3 (mod (mIdf, e.x), mod (mIdf, e.x * e.y) / e.x,
     mIdf / (e.x * e.y))) - 0.5 * (e - 1.));
  vm = 2. * normalize (vec3 (Hashff (mIdf + 0.1), Hashff (mIdf + 0.4), Hashff (mIdf + 0.7)) - 0.5);
  e = normalize (vec3 (Hashff (mIdf + 0.2), Hashff (mIdf + 0.5), Hashff (mIdf + 0.8)));
  qm = EulToQt (e);
  wm = 2.5 * e;
  diam = 1. - 0.2 * Hashff (mIdf + 0.1);
}

void main ()
{
  vec4 stDat, p, qm;
  vec3 rm, vm, wm;
  vec2 iFrag, canvas;
  float asp, diam, amp, freq;
  int mId, pxId, nbMax;
  bool doInit;
  iFrag = floor (gl_FragCoord.xy);
  pxId = int (iFrag.x + txRow * iFrag.y);
  nbEdge = ivec3 (64, 4, 2);
  txOff = 2;
  nbMax = nbEdge.x * nbEdge.y * nbEdge.z;
  if (iFrag.x >= txRow || pxId >= 4 * nbMax + txOff) discard;
  nFrame = iFrame;
  canvas = iResolution.xy;
  if (pxId >= txOff) mId = (pxId - txOff) / 4;
  else mId = -1;
  asp = canvas.x / canvas.y;
  doInit = false;
  if (nFrame <= 1) doInit = true;
  if (doInit) {
    hbSize = 0.7 * vec3 (nbEdge) + 0.5;
    hbSize.y *= 4.;
    hbSize.z *= 0.9;
    hbPos = vec3 (0.);
    nStep = 0.;
    nBall = nbMax;
  } else {
    stDat = Loadv4 (0);
    hbSize = stDat.xyz;
    nStep = stDat.w;
    ++ nStep;
    stDat = Loadv4 (1);
    hbPos = stDat.xyz;
    nBall = int (stDat.w);
  }
  dt = 0.01;
  freq = 0.5;
  amp = 2.;
  hbPos.y = amp * sin (2. * pi * freq * nStep * dt);
  if (mId >= 0) {
    if (doInit) Init (mId, rm, vm, qm, wm, diam);
    else Step (mId, rm, vm, qm, wm, diam);
  }
  if      (pxId == 0) stDat = vec4 (hbSize, nStep);
  else if (pxId == 1) stDat = vec4 (hbPos, float (nBall));
  else if (pxId >= txOff) {
    mId = pxId - (txOff + 4 * mId);
    p = (mId <= 1) ? ((mId == 0) ? vec4 (rm, diam) : vec4 (vm, 0.)) :
       ((mId == 2) ? qm : vec4 (wm, 0.));
  }
  Savev4 (pxId, ((pxId >= txOff) ? p : stDat), gl_FragColor, gl_FragCoord.xy);
}

vec4 QtMul (vec4 q1, vec4 q2)
{
  vec2 e = vec2 (1., -1.);
  return vec4 (dot (e.xyxx * q1.wzyx, q2), dot (e.xxyx * q1.zwxy, q2),
     dot (e.yxxx * q1.yxwz, q2), dot (e.yyyx * q1.xyzw, q2));
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
  m1[0][0] = c.y * c.z;  m2[0][0] = m1[0][0];
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

const float cHashM = 43758.54;

float Hashff (float p)
{
  return fract (sin (mod (p, 2. * pi)) * cHashM);
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
