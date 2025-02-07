//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 20.0
#define NO_BOLTS

float objID;
float svObjID;

float hash (float n) { return fract (sin (n) * 43758.5453); }
mat2 rot2 (float a) { float c = cos (a), s = sin (a); return mat2 (c, -s, s, c); }

vec2
path (float t)
{
  //return vec2 (0.0);

  float a = sin (t * 0.22);
  float b = cos (t * 0.28);
  return vec2 (a * 2.0 - b * 0.75, b * 0.85 + a * 0.75);
}

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
{
  //p -= vec3 (path (p.z), 0.0);

  n = max (abs (n), 0.001);
  n /= dot (n, vec3 (1.0));
  vec3 tx = texture (t, p.yz).xyz;
  vec3 ty = texture (t, p.zx).xyz;
  vec3 tz = texture (t, p.xy).xyz;

  return (tx * tx * n.x + ty * ty * n.y + tz * tz * n.z);
}

float
hash31 (vec3 p)
{
  float n = sin (dot (p, vec3 (7.31, 157.47, 113.93)));
  return fract (n); // return fract(vec3(64, 8, 1)*32768.0*n)*2.-1.;

  //p = vec3( dot(p, vec3(127.1, 311.7, 74.7)), dot(p, vec3(269.5, 183.3, 246.1)), dot(p, vec3(113.5, 271.9, 124.6)));
  //return (fract(sin(p)*43758.5453)*2. - 1.);
}

mat3
basis (vec3 n)
{
  float a = 1.0 / (1.0 + n.z);
  float b = -n.x * n.y * a;
  return mat3 (1.0 - n.x * n.x * a, b, n.x, b, 1.0 - n.y * n.y * a, n.y, -n.x, -n.y, n.z);
  //return transpose(mat3(vec3(1. - n.x*n.x*a, b, -n.x), vec3(b, 1. - n.y*n.y*a , -n.y), n));
}

vec2
tor (vec3 p, float rad, float rad2)
{
#ifndef NO_BOLTS
  vec3 q = p;
  q.xy = rot2 (-3.14159 / 12.0) * q.xy;
  //q.xy = rot2 (iTime / 2.0) * q.xy;

  float a = atan (q.y, q.x);

  const float oNum = 6.0; // Six objects in all.
  float ia = floor (a / 6.2831853 * oNum);
  ia = (ia + 0.5) / oNum * 6.2831853;

  q.xy = rot2 (ia) * q.xy;

  q.x -= rad;

  q = abs (q);
  float sh = max (max (q.x * 0.866025 + q.z * 0.5, q.z) - rad2 - 0.0125, q.y - 0.045);
  sh = max (sh, -q.y + 0.01);
#endif

#ifndef NO_BOLTS
  p.xy = vec2 (length (p.xy) - rad, p.z);

  float tor = length (p.xy) - rad2 + cos (a * 180.0) * 0.0002;

  //p.xy = rot2(a)*p.xy;
  //p = abs(p);
  //float tor = max(p.x, p.y) - rad2;// + cos(a*180.) * 0.0002;

  return vec2 (tor, sh);
#else
  float a = atan (p.y, p.x);
  p.xy = vec2 (length (p.xy) - rad, p.z);
  float tor = length (p.xy) - rad2 + cos (a * 180.0) * 0.0002;
  return vec2 (tor, 1e8);
#endif
}

float
simplexTruchet (vec3 p)
{
  vec3 i = floor (p + dot (p, vec3 (1.0 / 3.0)));
  p -= i - dot (i, vec3 (1.0 / 6.0));

  vec3 i1 = step (p.yzx, p),
       i2 = max (i1, 1.0 - i1.zxy);
  i1 = min (i1, 1.0 - i1.zxy);

  vec3 p0 = vec3 (0.0),
       p1 = i1 - 1.0 / 6.0,
       p2 = i2 - 1.0 / 3.0,
       p3 = vec3 (0.5);

  float rnd = hash31 (i * 57.31 + i1 * 41.57 + i2 * 27.93);

  vec3 t0 = p1,
       t1 = p2,
       t2 = p3,
       t3 = p0;
  if (rnd > 0.66)
  {
    t0 = p2, t1 = p3;
    t2 = p0;
    t3 = p1;
  }
  else if (rnd > 0.33)
  {
    t0 = p3, t1 = p0;
    t2 = p1;
    t3 = p2;
  }

  vec4 v;
  vec3 q, bn; 
  float rad = 0.306186218; // Equal to sqrt(6)/8.
  float rad2 = 0.025;

  bn = (t0 - t1) * 1.1547005;
  q = basis (bn) * (p - mix (t0, t1, 0.5));
  v.xy = tor (q, rad, rad2);

  bn = (t2 - t3) * 1.1547005;
  q = basis (bn) * (p - mix (t2, t3, 0.5));
  v.zw = tor (q, rad, rad2);

  v.xy = min (v.xy, v.zw);

  objID = step (v.x, v.y);

  return min (v.x, v.y);
}

float
map (vec3 p)
{
  p.xy -= path (p.z).xy;

  float ns = simplexTruchet (p);

  return ns * 0.9;
}

vec3
bumpMap (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2 (0.001, 0.0);

  mat3 m = mat3 (tex3D (tx, p - e.xyy, n), tex3D (tx, p - e.yxy, n), tex3D (tx, p - e.yyx, n));

  vec3 g = vec3 (0.299, 0.587, 0.114) * m;
  g = (g - dot (tex3D (tx, p, n), vec3 (0.299, 0.587, 0.114))) / e.x;
  g -= n * dot (n, g);

  return normalize (n + g * bf);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, d;
  for (int i = 0; i < 96; i++)
  {
    d = map (ro + rd * t);

    if (abs (d) < 0.001 * (t * 0.125 + 1.0) || t > FAR)
      break;

    t += d;
  }

  return min (t, FAR);
}

float
softShadow (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 24; 

  vec3 rd = lp - ro;

  float shade = 1.0;
  float dist = 0.001 * (t * 0.125 + 1.0);
  float end = max (length (rd), 0.0001);
  rd /= end;

  for (int i = 0; i < maxIterationsShad; i++)
  {
    float h = map (ro + rd * dist);
    shade = min (shade, smoothstep (0.0, 1.0, k * h / dist));
    dist += clamp (h, 0.01, 0.25);

    if (h < 0.0 || dist > end)
      break;
  }

  return min (max (shade, 0.0) + 0.1, 1.0);
}

vec3
getNormal (vec3 p, inout float edge, inout float crv)
{
  vec2 e = vec2 (2.0 / iResolution.y, 0.0);

  float d1 = map (p + e.xyy),
        d2 = map (p - e.xyy);
  float d3 = map (p + e.yxy),
        d4 = map (p - e.yxy);
  float d5 = map (p + e.yyx),
        d6 = map (p - e.yyx);
  float d = map (p) * 2.0;

  edge = abs (d1 + d2 - d) + abs (d3 + d4 - d) + abs (d5 + d6 - d);
  edge = smoothstep (0.0, 1.0, sqrt (edge / e.x * 2.0));

  e = vec2 (0.002, 0.0);
  d1 = map (p + e.xyy), d2 = map (p - e.xyy);
  d3 = map (p + e.yxy), d4 = map (p - e.yxy);
  d5 = map (p + e.yyx), d6 = map (p - e.yyx);

  return normalize (vec3 (d1 - d2, d3 - d4, d5 - d6));
}

float
calculateAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for (int i = 0; i < 5; i++)
  {
    float hr = 0.01 + float (i) * 0.5 / 4.0;
    float dd = map (nor * hr + pos);
    occ += (hr - dd) * sca;
    sca *= 0.7;
  }

  return clamp (1.0 - occ, 0.0, 1.0);
}

vec3
getObjectColor (vec3 p, vec3 n)
{
  vec3 tx = tex3D (iChannel0, p, n);
  tx = smoothstep (0.05, 0.5, tx);

  if (svObjID > 0.5)
    tx *= vec3 (1.25, 1.0, 0.8);
  else
    tx *= vec3 (0.9, 0.6, 0.3);

  return tx;
}

vec3
eMap (vec3 rd, vec3 sn)
{
  vec3 tx = getObjectColor (rd, sn);
  return smoothstep (0.15, 0.75, tx);
}

vec3
doColor (vec3 ro, vec3 rd, vec3 lp, float t)
{
  vec3 sceneCol = vec3 (0.0);

  if (t < FAR)
  {
    vec3 sp = ro + rd*t;

    float edge = 0.0, crv = 1.0;

    vec3 sn = getNormal (sp, edge, crv);
    vec3 svSn = sn;

    sn = bumpMap (iChannel0, sp * 2.0, sn, 0.01);
    svSn = mix (sn, svSn, 0.75);

    float sh = softShadow (sp + sn * 0.00125, lp, 16.0, t);
    float ao = calculateAO (sp, sn);
    sh = (sh + ao * 0.3) * ao;

    vec3 ld = lp - sp;
    float lDist = max (length (ld), 0.001);
    ld /= lDist;

    float atten = 2.0 / (1.0 + lDist * 0.125 + lDist * lDist * 0.25);

    float diff = max (dot (sn, ld), 0.0);
    diff = (pow (diff, 2.0) * 0.66 + pow (diff, 4.0) * 0.34) * 2.0;
    float spec = pow (max (dot (reflect (-ld, sn), -rd), 0.0), 32.0);
    float fres = clamp (1.0 + dot (rd, sn), 0.0, 1.0);

    vec3 objCol = getObjectColor (sp * 2.0, sn);

    sceneCol = objCol * (diff + 0.5 * ao + fres * fres * 0.25) + vec3 (1.0, 0.97, 0.92) * spec * 2.0;

    sceneCol += eMap (reflect (rd, svSn) / 2.0, svSn) * 0.75;

    sceneCol *= 1.0 - edge * 0.9;

    sceneCol *= sh;

    sceneCol *= atten;
  }

  vec3 fogCol = vec3 (0.0);
  sceneCol = mix (sceneCol, fogCol, smoothstep (0.0, 0.75, t / FAR));

  return sceneCol;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * 0.5) / iResolution.y;

  vec3 ro = vec3 (0.0, 0.0, iTime * 0.5);
  vec3 lk = ro + vec3 (0.0, 0.0, 0.25);
  vec3 lp = ro + vec3 (0.0, 1.0, 0.375);

  ro.xy += path (ro.z);
  lk.xy += path (lk.z);
  lp.xy += path (lp.z);

  float FOV = 3.14159 / 2.0;
  vec3 forward = normalize (lk - ro);
  vec3 right = normalize (vec3 (forward.z, 0.0, -forward.x));
  vec3 up = cross (forward, right);

  vec3 rd = normalize (forward + (uv.x * right + uv.y * up) * FOV);
  rd = normalize (vec3 (rd.xy, rd.z - length (rd.xy) * 0.25));

  float t = trace (ro, rd);

  svObjID = objID;

  vec3 sceneColor = doColor (ro, rd, lp, t);

  uv = gl_FragCoord.xy / iResolution.xy;
  sceneColor *= pow (16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y), 0.125) * 0.5 + 0.5;
  //sceneColor = mix (pow (min (vec3 (1.5, 1.0, 1.0) * sceneColor, 1.0), vec3 (1.0, 2.5, 12.0)).zyx, sceneColor,
                  //  pow (16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y), 0.125) * 0.5 + 0.5);

  gl_FragColor = vec4 (sqrt (clamp (sceneColor, 0.0, 1.0)), 1.0);
}
