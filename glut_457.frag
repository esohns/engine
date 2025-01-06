#version 300 es
precision mediump float;

out vec4 O;

uniform vec2 resolution;
uniform float time;
uniform vec2 touch;
uniform int pointerCount;

#define FC gl_FragCoord.xy
#define R resolution
#define P pointerCount
#define mouse (touch / R)
#define T mod (time + 40.0, 180.0)
#define TICK (fract (T * 0.025))
#define MN min (R.x, R.y)
#define N normalize
#define hue(a) (0.5 + 0.5 * sin (3.3 * a * vec3 (0.0, 1.0, 2.0)))
#define lum(a) vec3 (dot (a, vec3 (0.21, 0.71, 0.07)))
#define rot(a) mat2 (cos ((a) - vec4 (0.0, 11.0, 33.0, 0.0)))
#define box(p,s,r) length (max (abs (p) - (s), 0.0)) + min (0.0, max (max (abs ((p).x) - (s).x, abs ((p).y) - (s).y), abs ((p).z) - (s).z)) - r
#define Q(p) fract (p * vec2 (12.9898, 12.233))
#define Y(p) (p + dot (Q (p), Q (p) + 34.56))
#define rnd(p) fract (Y (p).x * Y (p).y)

vec4
map (vec3 p)
{
  vec3  q = p, st = p;
  const float n = 3.0;
  p.xz = (fract (p.xz / n) - 0.5) * n;
  vec2 id = (floor (q.xz / n) - 0.5) * n;
  float h = max (0.2, rnd (id) * 2.5),
        w = max (0.5, 1.0 - rnd (id));
        q = mod (q * 10.0, 1.125);
  float bld = box ((p - vec3 (0.0, h, 0.0)) + 0.05, vec3 (w, h, w), 0.0125),
        wnd = box (q, vec3 (1.0), 0.005);
  bld = max (bld, -(TICK < 0.5 ? max (wnd, -(bld + 0.05)) : min (wnd, bld - 0.025)));
  vec4 a = vec4 (bld, 1.0, id),
       b = vec4 (st.y + 0.66, 0, vec2 (0.0));
  return a.x < b.x ? a : b;
}

void
cam (inout vec3 p)
{
  if (P > 0)
  {
    p.yz *= rot (-mouse.y * 3.1415 + 1.5707);
    p.xz *= rot (3.1415 - mouse.x * 6.2832);
  }
  else
  {
    if (TICK > 0.5)
    {
      p.yz *= rot (-0.45);
      p.xz *= rot (2.7 + sin (T * 0.05));
    }
    else
    {
      p.yz *= rot ((sin (T * 0.1) * 0.5 - 0.5) * 0.95);
      p.xz *= rot (3.1415 + cos (T * 0.1) * 0.5);
    }
  }
}

void
main ()
{
  vec2 uv = (FC - 0.5 * R) / MN;
  vec3 col = vec3 (0.0),
         p = vec3 (0.0),
        rd = N (vec3 (uv, 0.8));
  cam (p); cam (rd);
  p.z += T * 3.0;
  const float steps = 60.0,
               maxd = 30.0;
  float i       = 0.0,
        dd      = 0.0,
        mat     = -1.0,
        diffuse = mix (0.3, 0.5, rnd (uv * 10.0 + vec2 (T, 1.0)));
  vec2 id = vec2 (0.0);
  for (; i < steps; i++)
  {
    vec4 d = map (p);
    d.x *= diffuse;
    if (d.x < 1e-2)
    {
      mat = d.y;
      id = d.zw;
      break;
    }
    if (d.x > maxd)
      break;
    p += rd * d.x;
    dd += d.x;
  }
  if (dd < maxd)
  {
    if (mat == 1.0)
    {
      col += (steps - i) / steps;
      col *= TICK < 0.5 ? 1.618 * lum (max (hue (id.x + id.y + rnd (id)), 0.4)) : vec3 (1.2);
    }
    else if (mat == 0.0)
    {
      col += vec3 (1.0, 0.7, 0.025) * step (max (abs (p.x * 12.0 + 10.0) - 0.05, mod (p.z, 2.0)), 1.0);
      col += 0.12;
    }
  }
  col = max (col, 0.08);
  O = vec4 (col, 1.0);
}
