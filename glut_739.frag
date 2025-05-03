#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

//#define SMOOTH_QUAD
#define ANIMATE_SURFACE
#define BLINKING_LIGHTS 
// Left: 0, Right: 1, Random: 2, Pattern: 3, Slope: 4.
#define DIAGONAL_MODE 4
#define SHOW_HORIZONTAL
#define SHOW_VERTICAL
#define SHOW_DIAGONAL 0
#define SHOW_BALL_JOINS

#define FAR 40.

vec3 vObjID;

mat2
r2 (float th)
{
  vec2 a = sin(vec2(1.5707963, 0) + th);
  return mat2(a, -a.y, a.x);
}

float
hash21 (vec2 p)
{
  float n = dot(p, vec2(13.163, 157.247));
  return fract(sin(n) * 43758.5453);
}

float
sdCapsule (vec3 p, vec3 a, vec3 b)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  pa = abs(pa - ba * h);
  return length(pa);
}

float
hm (vec2 p)
{
#ifdef ANIMATE_SURFACE
  p = p / 4. + iTime / 8.;
#else
  p /= 4.;
#endif

  float n = dot(sin(p.xy * 3.14159 - cos(p.yx * 3.14159) * 3.14159 / 2.), vec2(0.25) + .5) * .66;
  p = p * 1.5;

  p.xy = mat2(.866025, .5, -.5, .866025) * p.xy;

  n += dot(sin(p.xy * 3.14159 - cos(p.yx * 3.14159) * 3.14159 / 2.), vec2(0.25) + .5) * .34;

  return n;
}

#define scale 1.5 
vec3 hVal;
float diag;

float
triTerrain (vec2 p)
{
  vec2 ip = floor(p);
  p -= ip;

  float h00 = hm(ip);
  float h01 = hm(ip + vec2(0, 1));
  float h10 = hm(ip + vec2(1, 0));
  float h11 = hm(ip + 1.);

#ifdef SMOOTH_QUAD
  diag = 0.;
#else 
#if DIAGONAL_MODE == 0
  diag = 0.;
#elif DIAGONAL_MODE == 1
  diag = 1.;
#elif DIAGONAL_MODE == 2
  diag = hash21(ip)<.5? 0. : 1.;
#elif DIAGONAL_MODE == 3
  diag = mod(ip.x + ip.y, 2.)>.5? 0. : 1.;
#else
  float l1 = abs(h00 - h11);
  float l2 = abs(h01 - h10);
  diag = l1 > l2 ? 0. : 1.;
#endif
#endif

  float s;
  if (diag < .5)
  {
    s = step(1., p.x + p.y);

    float hs = s < .5 ? h00 : h11;
    hVal = vec3(hs, h10, h01);
  }
  else
  {
    s = step(p.x, p.y);

    float hs = s < .5 ? h10 : h01;
    hVal = vec3(h00, hs, h11);
  }

#ifdef SMOOTH_QUAD
   return mix(mix(hm(ip), hVal.y, p.x), mix(hVal.z, hm(ip+1.), p.x), p.y);
#else
  if (diag < .5)
  {
    vec3 b = abs(vec3(1.0 - p.x - p.y, p.x - (p.x - p.y + 1.) * s, p.y - (p.y - p.x + 1.) * s));

    return dot(b, hVal);
  }

  vec3 b = abs(vec3(1. - (1. - s) * p.x - p.y * s, (1. - 2. * s) * (p.x - p.y), p.x * s + p.y * (1. - s)));
  return dot(b, hVal);
#endif
}

float tObjID;

float
map (vec3 p)
{
  float ter = triTerrain(p.xz * scale) / scale;

  const float hPert = .4;
  float fl = p.y + (.5 - ter) * hPert;

  hVal = (hVal - .5 * scale) * hPert + .04;

  vec3 q = p * scale;
  q.xz = fract(q.xz);

  const float lw = .04;

  float ln = 1e5;
  float ln2 = 1e5;

  if (diag < .5)
  {
#ifdef SHOW_HORIZONTAL
    ln = min(ln, sdCapsule(q, vec3(0, hVal.x, 0), vec3(1, hVal.y, 0)));
    ln = min(ln, sdCapsule(q, vec3(0, hVal.z, 1), vec3(1, hVal.x, 1)));
#endif
#ifdef SHOW_VERTICAL
    ln = min(ln, sdCapsule(q, vec3(0, hVal.x, 0), vec3(0, hVal.z, 1)));
    ln = min(ln, sdCapsule(q, vec3(1, hVal.y, 0), vec3(1, hVal.x, 1)));
#endif
    ln -= lw;
#if SHOW_DIAGONAL > 0
    ln2 = min(ln2, sdCapsule(q, vec3(0, hVal.z, 1), vec3(1, hVal.y, 0)) - lw);
#endif
  }
  else
  {
#ifdef SHOW_HORIZONTAL
    ln = min(ln, sdCapsule(q, vec3(0, hVal.x, 0), vec3(1, hVal.y, 0)));
    ln = min(ln, sdCapsule(q, vec3(0, hVal.y, 1), vec3(1, hVal.z, 1)));
#endif
#ifdef SHOW_VERTICAL
    ln = min(ln, sdCapsule(q, vec3(0, hVal.x, 0), vec3(0, hVal.y, 1)));
    ln = min(ln, sdCapsule(q, vec3(1, hVal.y, 0), vec3(1, hVal.z, 1)));
#endif
    ln -= lw;
#if SHOW_DIAGONAL > 0
    ln2 = min(ln2, sdCapsule(q, vec3(0, hVal.x, 0), vec3(1, hVal.z, 1)) - lw);
#endif
  }

#ifdef SHOW_BALL_JOINS
  float hgt = (hm(floor(p.xz * scale + .5)) - .5 * scale) * hPert + .04;
  vec2 offXZ = fract(p.xz * scale + .5) - .5;

  float sp = length(vec3(offXZ.x, abs(q.y - hgt), offXZ.y)) - .09; // Ball join size.
#else
  float sp = 1e5;
#endif

  ln /= scale;
  ln2 /= scale;
  sp /= scale;

#ifndef SMOOTH_QUAD
#if SHOW_DIAGONAL == 1
  ln = min(ln, ln2);
#elif SHOW_DIAGONAL == 2
  fl = max(fl, -ln2 + .004);
#endif
#endif

  vObjID = vec3(fl, ln, sp);

  return min(min(fl, ln), sp) * .75;
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.;

  for (int i = 0; i < 96; i++)
  {
    float d = map(ro + rd * t);

    if (abs(d) < .001 * (t * .125 + 1.) || t > FAR)
      break;

    t += d;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p, float t)
{
  const vec2 e = vec2(.002, 0);

  float sgn = 1.;
  float mp[6];
  vec3[3] e6 = vec3[3](e.xyy, e.yxy, e.yyx);
  for (int i = min(iFrame, 0); i < 6; i++)
  {
    mp[i] = map(p + sgn * e6[i / 2]);
    sgn = -sgn;
    if (sgn > 2.)
      break;
  }

  return normalize(vec3(mp[0] - mp[1], mp[2] - mp[3], mp[4] - mp[5]));
}

float
getShad (vec3 ro, vec3 n, vec3 lp)
{
  const float eps = .001;

  float t = 0., sh = 1., dt;

  ro += n * eps * 1.1;

  vec3 ld = (lp - ro);
  float lDist = length(ld);
  ld /= lDist;

  for (int i = 0; i < 16; i++)
  {
    dt = map(ro + ld * t);

    sh = min(sh, 12. * dt / t);

    t += clamp(dt, .01, .5);
    if (dt < 0. || t > lDist)
      break;
  }

  return min(max(sh, 0.) + 0.2, 1.0);
}

float
cAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for (int i = 0; i < 5; i++)
  {
    float hr = 0.01 + float(i) * 0.5 / 4.0;
    float dd = map(nor * hr + pos);
    occ += (hr - dd) * sca;
    sca *= 0.7;
  }
  return clamp(1.0 - occ, 0.0, 1.0);
}

vec3
tex3D (sampler2D channel, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(channel, p.zy).xyz;
  vec3 ty = texture(channel, p.xz).xyz;
  vec3 tz = texture(channel, p.xy).xyz;

  return tx * tx * n.x + ty * ty * n.y + tz * tz * n.z;
}

vec3
texBump (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.002, 0);

  mat3 m = mat3(tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114) * m;
  g = (g - dot(tex3D(tx, p, n), vec3(0.299, 0.587, 0.114))) / e.x;
  g -= n * dot(n, g);

  return normalize(n + g * bf);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(57, 113, 27);
  vec3 ip = floor(p);
  p -= ip;
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p * p * (3. - 2. * p);
  h = mix(fract(sin(h) * 43758.5453), fract(sin(h + s.x) * 43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z);
}

vec3
envMap (vec3 p)
{
  p *= 2.;
  p.xz += iTime * .5;

  float n3D2 = n3D(p * 2.);

  float c = n3D(p) * .57 + n3D2 * .28 + n3D(p * 4.) * .15;
  c = smoothstep(0.5, 1., c);

  p = vec3(c * .8, c * .9, c);

  return mix(p.zxy, p, n3D2 * .34 + .665);
}

vec2
path (float z)
{
  float s = sin(z / 36.) * cos(z / 18.);
  return vec2(s * 16., 0.);
}
 
float
lightBlink (vec2 p)
{
  float rnd = hash21(floor(p));

  return smoothstep(.9, 1., sin(rnd * 6.283 + iTime * 2.) * .5 + .5);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy * .5) / min(iResolution.y, 800.);

  vec3 lk = vec3(0, 0, iTime * 1.5);
  vec3 ro = lk + vec3(0, 2.5, -2);

  vec3 lp = ro + vec3(0, 0, 3);

  lk.xy += path(lk.z);
  ro.xy += path(ro.z);
  lp.xy += path(lp.z);

  float FOV = 3.14159 / 3.;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0, -fwd.x));
  vec3 up = cross(fwd, rgt);

  vec3 rd = normalize(fwd + (uv.x * rgt + uv.y * up) * FOV);
  rd.xy *= r2(path(lk.z).x / 128.);

  float t = trace(ro, rd);
  float svObjID = vObjID.x < vObjID.y ? (vObjID.x < vObjID.z ? 0. : 2.) : (vObjID.y < vObjID.z ? 1. : 2.);

  vec3 sceneCol = vec3(0);
  if (t < FAR)
  {
    float tSize0 = 1.;

    vec3 sp = ro + rd * t;
    vec3 sn = getNormal(sp, t);

    vec3 ld = lp - sp;
    float lDist = max(length(ld), .001);
    ld /= lDist;

    float sh = getShad(sp, sn, lp);
    float ao = cAO(sp, sn);

    float atten = 1.2 / (1. + lDist * lDist * 0.05);

    float diff = max(dot(sn, ld), 0.0);
    diff = pow(diff, 4.) * 1.5;

    float spec = pow(max(dot(reflect(-ld, sn), -rd), 0.0), 8.);

    float fre = pow(clamp(dot(sn, rd) + 1., .0, 1.), 4.);

    vec3 ref = reflect(rd, sn);
    vec3 env = envMap(ref);
    vec3 specCol = vec3(1, .95, .8);

    vec3 texCol;
    if (svObjID < .5)
    {
      texCol = tex3D(iChannel0, sp * tSize0, sn);
      texCol = smoothstep(-.07, .7, texCol);

#ifdef BLINKING_LIGHTS
      texCol = mix(texCol, mix(texCol, texCol.yyz, .35) * 3.5, lightBlink(sp.xz * scale));
#endif

      specCol *= (n3D(sp * 96.) * .66 + n3D(sp * 192.) * .34) * .3 + .35;

      env *= texCol / 1.5;

      texCol = mix(texCol, texCol.xzy, dot(ref, vec3(.15)));
    }
    else
    {
      texCol = tex3D(iChannel0, sp * tSize0 * 2., sn);

      texCol = mix(texCol, texCol.xxx, .75) * .35;

      if (svObjID == 2.)
        env *= vec3(2.2, 1.7, .5);
    }

    sceneCol += (texCol * (diff + ao * .3 + vec3(1, .9, .7) * fre) + env * 1.5 + specCol * spec * 2.);

    sceneCol *= ao * atten * sh;
  }
    
  vec3 bg = mix(vec3(1, .5, .6), vec3(.1, .05, .025), clamp(rd.y + .75, 0., 1.));
  bg = mix(bg, bg.xzy, rd.y * .5 + .5);
  sceneCol = mix(sceneCol, bg, smoothstep(0., .95, t / FAR));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
