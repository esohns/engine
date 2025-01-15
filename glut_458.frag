#version 300 es
precision mediump float;

out vec4 O;

uniform float time;
uniform vec2 resolution;
uniform vec2 touch;
uniform int pointerCount;

#define mouse (touch / R)
#define P pointerCount
#define FC gl_FragCoord.xy
#define R resolution
#define T time
#define MN min (R.x, R.y)
#define N normalize
#define S smoothstep
#define SE(v,s) S (s + 1. / MN, s - 1. / MN, v)
#define rot(a) mat2 (cos ((a) - vec4 (0.0, 11.0, 33.0, 0.0)))
#define lum(a) dot (a, vec3 (0.21, 0.71, 0.07))
#define sphoff vec3 (1.0, 0.0, 0.0)

float
rnd (vec2 p)
{
  p = fract (p * vec2 (12.9898, 78.233));
  p += dot (p, p + 34.56);
  return fract (p.x * p.y);
}

float
noise (vec2 p)
{
  vec2 i = floor (p),
       f = fract(p),
       u = f * f * (3.0 - 2.0 * f);
  float a = rnd (i),
        b = rnd (i + vec2 (1.0, 0.0)),
        c = rnd (i + vec2 (0.0, 1.0)),
        d = rnd (i + 1.0);
  return mix (mix (a, b, u.x), mix (c, d, u.x), u.y);
}

float
fbm (vec2 p)
{
  float t = 0.0,
        a = 1.0,
        h = 0.0;
        mat2 m = mat2 (1.5, -0.6, 0.6, 1.4);
  for (int i = 0; i < 5; i++)
  {
    t += a * noise (p * m);
    a *= 0.5;
    p *= 2.0 * m;
    h += a;
  }
  return t / h;
}

float
smin (float a, float b, float k)
{
  float h = clamp (0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
  return mix (b, a, h) - k * h * (1.0 - h);
}

float mat =0.0, glow = 0.0;
float
planet (vec3 p, float g)
{
  vec3 q = abs (p);
  float a = length (p) - 1.0,
        b = max (max (q.x - 2.0, q.y - 0.15), q.z - 2.0),
        c = smin (a, -b, -0.005),
        d = max (b, length (p.xz) - 0.5);
  glow += (0.0125 / (0.0125 + d * d * 80.0)) * g;
  if (g > 0.0)
    mat = d < c ? 1.0 : 0.0;
  return min (d, c);
}


float
box (vec3 p, vec3 s, float r)
{
  p = abs (p) - (s - r);
  return length (max (p, 0.0)) + min (0.0, max (max (p.x, p.y), p.z)) - r;
}

float
map (vec3 p, float g)
{
  float a = planet (p + sphoff, g),
        b = box (p + vec3 (1.0, 1.5, 0.0), vec3 (1.5, 0.5, 1.0), 0.005);
  if (g > 0.0)
    mat = a < b ? mat : 2.0;
  float c = min (a, b),
        room = -box (p - vec3 (0.0, 0.0, -12.0), vec3 (4.0, 10.0, 14.0), 0.01);
  if (g > 0.0)
    mat = c < room ? mat : 3.0;
  return min (room, c);
}

vec3
norm (vec3 p)
{
  float h = 1e-3;
  vec2 k = vec2 (-1.0, 1.0);
  return N (k.xyy * map (p + k.xyy * h, 0.0) +
            k.yxy * map (p + k.yxy * h, 0.0) +
            k.yyx * map (p + k.yyx * h, 0.0) +
            k.xxx * map (p + k.xxx * h, 0.0));
}

float
shadow (vec3 p, vec3 lp)
{
  float shd = 1.0,
        maxd = length (lp - p);
  vec3 l = normalize (lp - p);
  for (float i=1e-3; i < maxd;)
  {
    float d = map (p + l * i, 0.0);
    if (d < 1e-3)
    {
      shd = 0.0;
      break;
    }
    shd = min (shd, 128.0 * d / i);
    i += d;
  }
  return shd;
}

vec2
sm (vec3 p)
{
  return 0.5 + vec2 (atan (p.x, p.z), atan (p.y, length (p.xz))) / 6.28318;
}

float
pattern (vec3 p)
{
  p.xz *= rot (2.0);
  vec2 uv = sm (p.zyx * 0.2);
  return S (1.0, 0.0, abs (p.y)) * noise (0.5 - uv + fbm (uv * 20.0));
}

float
wall (vec2 p)
{
  p.y += 1.75;
  p *= vec2 (1.0, 2.0) * 0.2;
  p.x += step (1.0, mod (p.y, 2.0)) * 0.5;
  vec2 id = floor (p);
  p = fract (p);
  return SE (1.0, max (abs (p.x + 0.005), abs (p.y + 0.01)));
}

float
calcAO (vec3 p, vec3 n)
{
  float occ = 0.0,
        sca = 1.0;
  for (float i=.0; i<5.; i++)
  {
    float h = 0.01 + i * 0.09,
          d = map (p + h * n, 0.0);
    occ += (h - d) * sca;
    sca *= 0.55;
    if (occ > 0.35)
      break;
  }
  return clamp (1.0 - 3.0 * occ, 0.0, 1.0) * (0.5 + 0.5 * n.y);
}

void
cam (inout vec3 p)
{
  if (P > 0)
  {
    p.yz *= rot (sin (-mouse.y * 6.3 + 3.14));
    p.xz *= rot (sin (3.14 - mouse.x * 6.3) * 1.5);
  }
  else
  {
    p.yz *= rot (0.42 - sin (T * 0.125) * 0.42);
    p.xz *= rot (0.5 * S (1.0, -1.0, -sin (T * 0.2)));
  }
}

void
main ()
{
  vec2 uv = (FC - 0.5 * R) / MN;
  vec3 col = vec3 (0),
         p = vec3 (0.0, -0.2, -mix (4.0, 8.0, P > 0 ? 0.0 : S (0.0, 1.0, 1.0 - T * 0.025)) + (P > 0 ? 0.0 : exp (S (1.0, -1.0, cos (T * 0.125))))),
        rd = N (vec3 (uv + vec2 (R.x > R.y ? 0.2 : -0.3, 0.0), 1.2));
  cam (p); cam (rd);
  float dd = 0.0; const float maxd = 18.0;
  for (int i = 0; i < 400; i++)
  {
    float d = map (p, 1.0);
    if (abs (d) < 1e-3 || dd > maxd)
      break;
    p += rd * d;
    dd += d;
  }
  if (dd < maxd)
  {
    vec3 n = norm (p);
    float ao = calcAO (p, n);
    if (mat == 0.0)
    {
      vec3 q = p;
      q += sphoff;
      q.xz *= rot (T * 0.125);
      q -= sphoff;
      vec2 tx = sm (norm (q));
      tx *= 0.3;
      float t = T * 0.0;
      col += max (0.08, S (1.7, 0.0, (p.y)) * fbm (0.5 + tx + fbm (t * 0.5 - tx * rot (1.5707) * 12.0 + fbm (t + tx * rot (0.42) * 22.0)))) * vec3 (0.1, 0.3, 0.9);
      col = mix (col, vec3 (1.0), lum (col));
      col = tanh (col * col);
      col = sqrt (col) * ao;
    }
    else if (mat == 1.0)
    {
      col += 1.0;
    }
    else if (mat == 2.0)
    {
      col += fbm (p.yz + fbm (p.yz * 4.0)) * abs (n.x);
      col += fbm (p.xy + fbm (p.xy * 4.0)) * abs (n.z);
      col += fbm (p.xz + fbm (p.xz * 4.0)) * abs (n.y);
      col /= 1.0 + exp (-col);
      col = sqrt (vec3 (lum (col)));
    }
    else if (mat == 3.0)
    {
      col += mix (0.0, 0.5, fbm (p.yz + fbm (p.yz * 4.0))) * abs (n.x);
      col += mix (0.0, 0.2, fbm (p.xy + fbm (p.xy * 4.0))) * abs (n.z);
      col = tanh (col * col);
      col = sqrt (col);
      col = mix (col, vec3 (mix (0.4, 0.5, noise (190.0 * p.xy))), wall (p.xy) * abs (n.z));
      col = mix (col, vec3 (mix (0.4, 0.5, noise (190.0 * p.zy))), wall (p.zy) * abs (n.x));
    }
    if (mat > 1.0)
    {
      float shd = shadow (p + n * 3e-3, vec3 (3.3, 2.7, -2.95));
      col = mix (vec3 (0), col, max (shd, 0.2));
      col *= ao;
    }
    col += 0.4 * pattern (N (reflect (rd, n))) * ao;
    if (mat == 0.0 && abs (n.y) < 1.0)
    {
      vec3 q = p;
      q += sphoff;
      q.xz *= rot (T * 0.125);
      col -= 0.0075 * (sin (386.0 * atan (q.z, q.x))) * S (0.4, 0.7, length (q.xz));
    }
    if (mat == 0.0 || mat >= 2.0)
    {
      col += 0.5 * pow (clamp (dot (reflect (rd, n), N (vec3 (1.0, 2.7, -1.0))), 0.0, 1.0), 9.0);
    }
  }
  col = tanh (col * col * col);
  col = pow (col, vec3 (0.4545));
  col *= 0.8;
  vec2 vp = FC / R * 2.0 - 1.0;
  vp *= 0.95;
  vp *= vp * vp * vp * vp;
  float v = pow (dot (vp, vp), 0.8);
  v -= sqrt (glow);
  v = max (0.0, v);
  col = mix (col, vec3 (0.0), v);
  col += vec3 (0.3, 0.4, 0.8) * sqrt (glow);
  O = vec4 (col, 1.0);
}
