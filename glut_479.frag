#version 130
//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

#define FAR 20.

// Color scheme: 0, 1 or 2 -> Gold and aluminium, black and chrome, pink and chrome
#define COLOR_SCHEME 0

mat2 rot2 (float a) { float c = cos (a), s = sin (a); return mat2 (c, s, -s, c); }

vec3 
rotObj (vec3 p)
{
  p.yz *= rot2 (iTime * 0.2 / 2.0);
  p.xz *= rot2 (iTime * 0.5 / 2.0);
  return p;
}

float
noise3D (vec3 p)
{
  const vec3 s = vec3 (113.0, 157.0, 1.0);
  vec3 ip = floor (p);
  vec4 h = vec4 (0., s.yz, s.y + s.z) + dot (ip, s);

  p -= ip;

  p = p * p * (3.0 - 2.0 * p);

  h = mix (fract (sin (mod (h, 6.2831589)) * 43758.5453),
           fract (sin (mod (h + s.x, 6.2831589)) * 43758.5453),
           p.x);
  h.xy = mix (h.xz, h.yw, p.y);

  return mix (h.x, h.y, p.z);
}

float fbm (vec3 p) { return 0.5333 * noise3D (p) + 0.2667 * noise3D (p * 2.02) + 0.1333 * noise3D (p * 4.03) + 0.0667 * noise3D (p * 8.03); }

const float PHI = (1.0 + sqrt (5.0)) / 2.0;
const float A = PHI / sqrt (1.0 + PHI * PHI);
const float B = 1.0 / sqrt (1.0 + PHI * PHI);
const float J = (PHI - 1.0) / 2.0;
const float K = PHI / 2.0;
const mat3 R0 = mat3 (0.5, -K, J, K, J, -0.5, J, 0.5, K);
const mat3 R1 = mat3 (K, J, -0.5, J, 0.5, K, 0.5, -K, J);
const mat3 R2 = mat3 (-J, -0.5, K, 0.5, -K, -J, K, J, 0.5);
const mat3 R4 = mat3 (0.587785252292, -K, 0.0, -0.425325404176, -J, 0.850650808352, 0.688190960236, 0.5, 0.525731112119);
const vec3 v0 = vec3 (0.0, A, B);
const vec3 v1 = vec3 (B, 0.0, A);
const vec3 v2 = vec3 (-B, 0.0, A);
const vec3 mid = (vec3 (0.0, A, B) + vec3 (0.0, 0.0, A) * 2.0) / 3.0;

#define a(n) (1.0 / (1.0 + n.z))
#define b(n) (-n.x * n.y * a (n))
#define basis(n) mat3 (1.0 - n.x * n.x * a (n), b (n), n.x, b (n), 1.0 - n.y * n.y * a (n), n.y, -n.x, -n.y, n.z)

const vec3 hexN = normalize (cross ((mid - v0), (mid - v1)));
const vec3 n1 = normalize (mix (v0, v2, 0.333));
const vec3 n2 = normalize (mix (v0, v1, 0.333));
const mat3 basisHex = basis (hexN);
const mat3 basisHexSm1 = basis (n1);
const mat3 basisHexSm2 = basis (n2);

vec3
opIcosahedronWithPolarity (vec3 p)
{
  vec3 pol = sign (p);
  p = R0 * abs (p);
  pol *= sign (p);
  p = R1 * abs (p);
  pol *= sign (p);
  p = R2 * abs (p);
  pol *= sign (p);
  vec3 ret = abs (p);
  return ret * vec3 (pol.x * pol.y * pol.z, 1.0, 1.0);
}

vec4 objID;
float spokes, sph;

float
dist (vec3 p, float r1, float r2)
{
  //p.xy *= rot2(mod(iTime/1.5, 3.*6.2831/15.) + 6.2831/2.);
  p.xy *= rot2 (mod (iTime /1.5, 6.2831 / 5.0) + 3.14159265);

  float a = atan (p.y, abs (p.x));

  r1 *= (0.9 + p.z);

  vec3 q = p; 
  float ia = floor (a / 6.2831 * 15.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 15.0) * q.xy;
  q.x += r1;// - 0.03 / 3.0;
  q = abs (q);
  float spike = mix (max (q.x - 0.05, q.y - 0.02), length (q.xy * vec2 (0.7, 1.0)) - 0.025, 0.5);
  float d2 = max (spike, q.z - r2);

  q = p; 
  ia = floor (a / 6.2831 * 5.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 5.0) * q.xy;
  sph = max (max (abs (q.x), abs (q.y)) - 0.0225, abs (q.z - 0.07) - 0.06);
  //sph = max (length (q.xy) - 0.0225, abs (q.z - 0.07) - 0.06);
  q = abs (q + vec3 (r1 / 2.0, 0.0, -0.07));
  //spokes = max (max (q.x - r1 / 2.0 + 0.02, q.y - 0.03), q.z - 0.01);
  spokes = max (q.x - r1 / 2.0 + 0.02, max (max (q.y - 0.05, q.z - 0.015), (q.y + q.z) * 0.7071 - 0.025));

  p = abs (p);
  float d = length (p.xy);
  float di = abs (d - r1 + 0.1 / 2.0) - 0.05 / 2.0;
  d = abs (d - r1 + 0.075 / 2.0) - 0.075 / 2.0;
  d = min (max (d, p.z - r2), max (di, p.z - r2 - 0.01));

  return min (d, d2);
}

float
dist2 (vec3 p, float r1, float r2)
{
  //p.xy *= rot2 (mod (iTime / 1.8, 3.0 * 6.2831 / 18.0) + 3.0 * 6.2831 / 18.0);
  p.xy *= rot2 (mod (iTime / 1.8, 6.2831 / 6.0) + 6.2831 / 6.0);

  float a = atan (p.y, abs (p.x));

  p.z = -p.z;

  r1 *= (0.9 + p.z);

  vec3 q = p; 

  float ia = floor (a / 6.2831 * 18.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 18.0) * q.xy;
  q.x += r1;// - 0.03 / 3.0;
  q = abs (q);
  float spike = mix (max (q.x - 0.05, q.y - 0.02), length (q.xy * vec2 (0.7, 1.0)) - 0.025, 0.5);
  float d2 = max (spike, q.z - r2);

  q = p;
  ia = floor (a / 6.2831 * 6.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 6.0) * q.xy;
  sph = max (max (abs (q.x), abs (q.y)) - 0.0275, abs (q.z - 0.07) - 0.06);
  //sph = max (length (q.xy) - 0.0275, abs (q.z - 0.07) - 0.06);
  q = abs (q + vec3 (r1 / 2.0, 0.0, -0.07));
  //spokes = max (max (q.x - r1 / 2.0 + 0.02, q.y - 0.03), q.z - 0.01);
  spokes = max (q.x - r1 / 2.0 + 0.02, max (max (q.y - 0.05, q.z - 0.015), (q.y + q.z) * 0.7071 - 0.03));

  p = abs (p);
  float d = length (p.xy);
  float di = abs (d - r1 + 0.1 / 2.0) - 0.05 / 2.0;
  d = abs (d - r1 + 0.075 / 2.0) - 0.075 / 2.0;
  d = min (max (d, p.z - r2), max (di, p.z - r2 - 0.0115));

  return min (d, d2);
}

float
dist3 (vec3 p, float r1, float r2)
{
  float dir = p.x < 0.0 ? -1.0 : 1.0;

  p.x = abs (p.x);
  vec3 q2 = p;
  p.xy *= rot2 (mod (iTime / 1.2 * dir + 3.14159 / 12.0, 6.2831 / 12.0) + 5.0 * 6.2831 / 12.0); // + 5.0 * 3.14159 / 12.0

  float a = atan (p.y, abs (p.x));

  r1 *= (0.9 + p.z);

  vec3 q = p;

  float ia = floor (a / 6.2831 * 12.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 12.0) * q.xy;
  q.x += r1;// - 0.03 / 3.0;
  q.xy = abs (q.xy);
  float spike = mix (max (q.x - 0.05, q.y - 0.02), length (q.xy * vec2 (0.7, 1.0)) - 0.025, 0.5);
  float d2 = max (spike, abs (q.z) - r2);

  q = q2;
  q.xy *= rot2 (mod (iTime /1.2 * dir, 6.2831 / 6.0) + 2.0 * 6.2831 / 6.0);
  a = atan (q.y, abs (q.x));
  ia = floor (a / 6.2831 * 6.0) + 0.5;
  q.xy = rot2 (ia * 6.2831 / 6.0) * q.xy;
  sph = max (max (abs (q.x), abs (q.y)) - 0.02, abs (q.z - 0.07) - 0.06);
  //sph = max (length (q.xy) - 0.02, abs (q.z - 0.07) - 0.06);
  q = abs (q + vec3 (r1 / 2.0, 0.0, -0.07));
  spokes = max (q.x - r1 / 2.0 + 0.02, max (max (q.y - 0.04, q.z - 0.015), (q.y + q.z) * 0.7071 - 0.02));

  p = abs (p);
  float d = length (p.xy);
  float di = abs (d - r1 + 0.1 / 2.0) - 0.05 / 2.0;
  d = abs (d - r1 + 0.075 / 2.0) - 0.075 / 2.0;
  d = min (max (d, p.z - r2), max (di, p.z - r2 - 0.007));

  return min (d, d2);
}

float
map (vec3 p)
{
  float pln = -p.z + 6.0;

  p = rotObj (p);
  //p.yz *= rot2 (-3.14159 / 6.0);

  vec3 oP = p;

  float d = 1e5, d2 = 1e5, d3 = 1e5;

  vec3 hexFace = opIcosahedronWithPolarity (p);

  vec3 pentFace = R4 * hexFace;

  vec3 p1 = (pentFace - vec3 (0.0, 0.0, 1.0));
  d3 = min (d3, dist (p1, 0.185, 0.1));
  d = min (d, spokes);
  d3 = min (d3, sph);

  p1 = basisHex * (hexFace - mid * 1.2425);
  d3 = min (d3, dist2 (p1, 0.25, 0.1));
  d = min (d, spokes);
  d3 = min (d3, sph);

  p1 = basisHexSm1 * (hexFace - mix (v0, v2, 0.333) * 1.1547);
  d2 = min (d2, dist3 (p1, 0.16, 0.1));
  d = min (d, spokes);
  d3 = min (d3, sph);
  p1 = basisHexSm2 * (hexFace - mix (v0, v1, 0.333) * 1.1547);
  d2 = min (d2, dist3 (p1, 0.16, 0.1));
  d = min (d, spokes);
  d3 = min (d3, sph);

  float mainSph = length (oP);
  d = max (d, mainSph - 1.0825);
  d2 = max (d2, mainSph - 1.116);
  d3 = max (d3, mainSph - 1.118);

  objID = vec4 (d, d2, d3, pln);

  return min (min (d, d2), min (d3, pln));
}

vec3
calcNormal (vec3 p, inout float edge, inout float crv, float t)
{
  float eps = 3.0 / mix (450.0, min (850.0, iResolution.y), 0.35);

  float d = map (p);

  vec3 e = vec3 (eps, 0.0, 0.0);

  vec3 da = vec3 (-2.0 * d);
  for (int i = min (iFrame, 0); i < 3; i++)
  {
    for (int j = min (iFrame, 0); j < 2; j++)
      da[i] += map (p + e * float (1 - 2 * j));
    e = e.zxy;
  }
  da = abs (da);

  edge = da.x + da.y + da.z;
  edge = smoothstep (0.0, 1.0, sqrt (edge / e.x * 2.0));

  vec3 n = vec3 (0.0);
  for (int i = min (iFrame, 0); i < 4; i++)
  {
    vec3 e = 0.57735 * (2.0 * vec3 ((((i + 3) >> 1) & 1), ((i >> 1) & 1), (i & 1)) - 1.0);
    n += e * map (p + 0.001 * e);
  }

  return normalize (n);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, d;

  for (int i = min (iFrame, 0); i < 64; i++)
  {
    d = map (ro + rd * t);
    if (abs (d) < 0.001 * (1.0 + t * 0.05) || t > FAR)
      break;
    t += d;
  }

  return min (t, FAR);
}

float
calcAO (vec3 p, vec3 n)
{
  float sca = 4.0,
        occ = 0.0;

  for (int i = 1; i < 6; i++)
  {
    float hr = float (i) * 0.125 / 5.0;
    float dd = map (p + hr * n);
    occ += (hr - dd) * sca;
    sca *= 0.75;
  }

  return clamp (1.0 - occ, 0.0, 1.0);
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int maxIterationsShad = 32; 

  ro += n * 0.0015;
  vec3 rd = lp - ro;

  float shade = 1.0;
  float t = 0.0;
  float end = max (length (rd), 0.0001);
  rd /= end;

  for (int i = min (iFrame, 0); i < maxIterationsShad; i++)
  {
    float d = map (ro + rd * t);

    shade = min (shade, k * d / t);
    t += clamp (d, 0.01, 0.25);

    if (d < 0.0 || t > end)
      break;
  }

  return max (shade, 0.0);
}

vec3
envMap (vec3 p)
{
  p *= 3.0;

  float n3D2 = noise3D (p * 3.0);

  float c = noise3D (p) * 0.57 + noise3D (p * 2.0) * 0.28 + noise3D (p * 4.0) * 0.15;
  c = smoothstep (0.25, 1.0, c);

  p = vec3 (c, c * c, c * c * c);

  return mix (p, p.zyx, n3D2 * 0.5 + 0.5);
}

void
main ()
{
  vec2 p = (gl_FragCoord.xy - iResolution.xy * 0.5) / min (850.0, iResolution.y);

  vec3 rd = normalize (vec3 (p, 1.0));
  vec3 ro = vec3 (0.0, 0.0, -2.75);

  vec3 lp = ro + vec3 (-1.0, 2.0, -1.0);

  rd.xy *= rot2 (sin (iTime / 8.0) * 0.2);
  rd.xz *= rot2 (sin (iTime / 4.0) * 0.1);

  float t = trace (ro, rd);

  float svObjID = objID.x < objID.y && objID.x < objID.z && objID.x < objID.w ? 0.0
                                                                              : objID.y < objID.z && objID.y < objID.w ? 1.0
                                                                                                                       : objID.z < objID.w ? 2.0
                                                                                                                                           : 3.0;

  vec3 sceneCol = vec3 (0.0);

  if (t < FAR)
  {
    vec3 pos = ro + rd * t;
    float edge = 0.0,
           crv = 1.0;
    vec3 nor = calcNormal (pos, edge, crv, t);

    vec3 li = lp - pos;
    float lDist = max (length (li), 0.001);
    li /= lDist;

    float atten = 1.0 / (1.0 + lDist * 0.05 + lDist * lDist * 0.025);

    float shd = softShadow (pos, lp, nor, 8.0);
    float ao = calcAO (pos, nor);

    float diff = max (dot (li, nor), 0.0);
    float spec = pow (max (dot (reflect (-li, nor), -rd), 0.0), 16.0);
    float od = diff;
    diff = pow (diff, 4.0) * 2.0;

    float Schlick = pow (1.0 - max (dot (rd, normalize (rd + li)), 0.0), 5.0);
    float fre2 = mix (0.5, 1.0, Schlick);

#if COLOR_SCHEME == 0
    vec3 col = vec3 (0.6);
#elif COLOR_SCHEME == 1
    vec3 col = vec3 (0.1);
#else
    vec3 col = vec3 (0.9, 0.2, 0.4);
#endif

    if (svObjID == 1.0)
    {
#if COLOR_SCHEME == 0
      col = vec3 (1.0, 0.5, 0.2) * 0.7;
#else
      col = vec3 (0.6);
#endif
    }
    else if (svObjID == 2.0)
    {
#if COLOR_SCHEME == 0
      col = vec3 (1.0, 0.65, 0.3) * 0.7;
#else 
      col = vec3 (0.6);
#endif
    }
    else if(svObjID == 3.0)
    {
#if COLOR_SCHEME == 0
      col = vec3 (1.0, 0.7, 0.4) * 0.045;
#elif COLOR_SCHEME == 1
      col = vec3 (0.7) * 0.045;
#else
      col = vec3 (0.6, 0.7, 1.0) * 0.045;
#endif
    }

    float txSz = 1.0;
    vec3 txPos = pos;
    if (svObjID == 3.0)
      txSz /= 4.0;
    else
      txPos = rotObj (txPos);
    col *= fbm (txPos * 64.0 * txSz) * 0.75 + 0.5;

    sceneCol = col * (diff * shd + 0.25);

    if (svObjID == 3.0)
      sceneCol += col * vec3 (1.0, 0.6, 0.2).zyx * spec * shd * 0.25;
    else
      sceneCol += col * vec3 (0.5, 0.75, 1.0) * spec * shd * 2.0;

    float envF = 4.0;
    if (svObjID == 0.0)
      envF = 8.0;
    sceneCol += sceneCol * envMap (reflect (rd, nor)) * envF;

    sceneCol *= 1.0 - edge * 0.8;
    //col = col * 0.7 + edge * 0.3;

    sceneCol *= atten * ao;
  }

  gl_FragColor = vec4 (sqrt (clamp (sceneCol, 0.0, 1.0)), 1.0);
}
