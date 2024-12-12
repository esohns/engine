//precision highp float;

uniform vec2 u_resolution;
uniform float u_time;
uniform vec2 u_mouse;

const float PI = 3.14159;
const float TAU = PI * 2.;
const float QTR_PI = PI / 4.;

float
stroke (float x, float s, float w)
{
  float d = step (s, x + w / 2.) - step (s, x - w / 2.);
  return clamp (d, 0., 1.);
}

float
circleSDF (vec2 st) {
  return length (st - 0.5) * 2.;
}

float
fill (float x, float size)
{
  return 1. - step (size, x);
}

float
rectSDF (vec2 st, vec2 s)
{
  st = st * 2. - 1.;
  return max (abs (st.x / s.x), abs (st.y / s.y));
}

float
crossSDF (vec2 st, float s)
{
  vec2 size = vec2 (0.25, s);
  return min (rectSDF (st, size.xy), rectSDF (st, size.yx));
}

float
flip (float v, float pct)
{
  return mix (v, 1. - v, pct);
}

float
vesicaSDF (vec2 st, float w)
{
  vec2 offset = vec2 (w * .5, 0.);
  return max (circleSDF (st - offset), circleSDF (st + offset));
}

float
triSDF (vec2 st)
{
  st = (2. * st - 1.) * 2.;
  return max (abs (st.x) * 0.866025 + st.y * .5, -st.y * .5);
}

float
rhombSDF (vec2 st)
{
  return max (triSDF (st), triSDF (vec2 (st.x, 1. - st.y)));
}

vec2
rotate (vec2 st, float a)
{
  st = mat2 (cos (a), -sin (a), sin (a), cos (a)) * (st - .5);
  return st + .5;
}

float
polySDF (vec2 st, int V)
{
  st = st * 2. - 1.;
  float a = atan (st.x, st.y) + PI;
  float r = length (st);
  float v = TAU / float (V);
  return cos (floor (.5 + a / v) * v - a) * r;
}

float
hexSDF (vec2 st)
{
  st = abs (st * 2. - 1.);
  return max (abs (st.y), st.x * 0.866025 + st.y * .5);
}

float
starSDF (vec2 st, int V, float s)
{
  st = st * 4. - 2.;
  float a = atan (st.y, st.x) / TAU;
  float seg = a * float (V);
  a = ((floor (seg) + 0.5) / float (V) + mix (s, -s, step (.5, fract (seg)))) * TAU;
  return abs (dot (vec2 (cos (a), sin (a)), st));
}

float
raysSDF (vec2 st, float N)
{
  st -= .5;
  return fract (atan (st.y, st.x) / TAU * N);
}

float
heartSDF (vec2 st)
{
  st -= vec2 (.5, .8);
  float r = length (st) * 5.;
  st = normalize (st);
  return r - ((st.y * pow (abs (st.x), 0.67)) / (st.y + 1.5) - 2. * st.y + 1.26);
}

float
bridge (float c, float d, float s, float w)
{
  c *= 1. - stroke (d, s, w * 2.);
  return c + stroke (d, s, w);
}

float
spiralSDF (vec2 st, float t)
{
  st -= .5;
  float r = dot (st, st);
  float a = atan (st.y, st.x);
  return abs (sin (fract (log (r) * t + a * 0.159)));
}

vec2
scale (vec2 st, vec2 s)
{
  return (st - .5) * s + .5;
}

float
flowerSDF (vec2 st, float N)
{
  st = st * 2. - 1.;
  float r = length (st) * 2.;
  float a = atan (st.y, st.x);
  float v = N * .5;
  return 1. - (abs (cos (a * v)) * .5 + .5) / r;
}

void
coswarp (inout vec3 trip, float warpsScale)
{
  trip.xyz += warpsScale * .1 * cos (3. * trip.yzx + (u_time * .25));
  trip.xyz += warpsScale * .05 * cos (11. * trip.yzx + (u_time * .25));
  trip.xyz += warpsScale * .025 * cos (17. * trip.yzx + (u_time * .25));
}

vec4
permute (vec4 x)
{
  return mod (((x * 34.0) + 1.0) * x, 289.0);
}

vec2
fade (vec2 t)
{
  return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

float
cnoise (vec2 P)
{
  vec4 Pi = floor (P.xyxy) + vec4 (0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract (P.xyxy) - vec4 (0.0, 0.0, 1.0, 1.0);
  Pi = mod (Pi, 289.0); // To avoid truncation effects in permutation
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;
  vec4 i = permute (permute (ix) + iy);
  vec4 gx = 2.0 * fract (i * 0.0243902439) - 1.0; // 1/41 = 0.024...
  vec4 gy = abs (gx) - 0.5;
  vec4 tx = floor (gx + 0.5);
  gx = gx - tx;
  vec2 g00 = vec2 (gx.x, gy.x);
  vec2 g10 = vec2 (gx.y, gy.y);
  vec2 g01 = vec2 (gx.z, gy.z);
  vec2 g11 = vec2 (gx.w, gy.w);
  vec4 norm =
    1.79284291400159 - 0.85373472095314 * vec4 (dot (g00, g00), dot (g01, g01), dot (g10, g10), dot (g11, g11));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;
  float n00 = dot (g00, vec2 (fx.x, fy.x));
  float n10 = dot (g10, vec2 (fx.y, fy.y));
  float n01 = dot (g01, vec2 (fx.z, fy.z));
  float n11 = dot (g11, vec2 (fx.w, fy.w));
  vec2 fade_xy = fade (Pf.xy);
  vec2 n_x = mix (vec2 (n00, n01), vec2 (n10, n11), fade_xy.x);
  float n_xy = mix (n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  vec2 uv2 = uv;

  float t = (u_time * .2) + length (uv - .5);

  vec3 color2 = vec3 (uv.x, uv2.y, 1.);

  coswarp (color2, 1.);
  coswarp (color2, u_mouse.x *.0015);
  coswarp (color2, u_mouse.y *.0015);

  vec3 color = vec3 (1.);
  color = mix (color, 1. - color , step (cnoise (color2.rg * 2.), .001));

  if (color == vec3 (0.))
  {
    color = mix (color, vec3 (step (sin (t), .5), 0., 0.) , step (cnoise (color2.gb * 14.), .01));
  }
  if (color == vec3 (0.))
  {
    color = mix (color, vec3 (0., step (cos (t), .5), 0.) + uv.x, step (cnoise (color2.br * 20.), .01));
  }
  if (color == vec3 (0.))
  {
    color = mix (color, vec3 (0., 0., step (sin (t + uv.x), .5)) + uv.y, step (cnoise (color2.gr * 26.), .01));
  }

  if (color != vec3 (1.) && color != vec3 (0.))
  {
    coswarp (color, 3.);
  }

  gl_FragColor = vec4 (color, 1.0); 
}
