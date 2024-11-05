#version 300 es

precision mediump float;

uniform vec2 canvasSize;
uniform float time;

in vec2 vTexCoord;
out vec4 fragColor;

vec3
palette (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos (6.28318 * (c * t + d));
}

float
hash12 (vec2 p)
{
  vec3 p3 = fract (vec3 (p.xyx) * .1031);
  p3 += dot (p3, p3.yzx + 33.33);
  return fract ((p3.x + p3.y) * p3.z);
}

//

float
sdCircle (vec2 p, float r)
{
  return length (p) - r;
}

float
sdBox (vec2 p, vec2 b)
{
  vec2 d = abs (p) - b;
  return length (max (d, 0.0)) + min (max (d.x, d.y), 0.0);
}

float
sdRoundedBox (vec2 p, vec2 b, vec4 r)
{
  r.xy = (p.x > 0.0) ? r.xy : r.zw;
  r.x = (p.y > 0.0) ? r.x : r.y;
  vec2 q = abs (p) - b + r.x;
  return min (max (q.x, q.y), 0.0) + length (max (q, 0.0)) - r.x;
}

float
sdSegment (vec2 p, vec2 a, vec2 b)
{
  vec2 pa = p - a, ba = b - a;
  float h = clamp (dot (pa, ba) / dot (ba, ba), 0.0, 1.0);
  return length (pa - ba * h);
}

float
sdHexagram (vec2 p, float r)
{
  const vec4 k = vec4 (-0.5, 0.8660254038, 0.5773502692, 1.7320508076);
  p = abs (p);
  p -= 2.0 * min (dot (k.xy, p), 0.0) * k.xy;
  p -= 2.0 * min (dot (k.yx, p), 0.0) * k.yx;
  p -= vec2 (clamp (p.x, r * k.z, r * k.w), r);
  return length (p) * sign (p.y);
}

//

vec3
dust (vec2 uv, vec3 oriChannel, vec3 dustChannel, float wei)
{
  oriChannel += dustChannel*wei;
  return oriChannel-wei/2.;
}

float
Hexagon (vec2 uv)
{
  uv = abs (uv);
  return max (uv.x, dot (uv, normalize (vec2 (1., sqrt (3.)))));
}

vec4
HexCoord (vec2 uv)
{
  vec2 r = vec2(1., sqrt(3.));
  vec2 h = r*.5;
  vec2 a = mod(uv, r)-h;
  vec2 b = mod(uv-h, r)-h;
  vec2 gv = dot(a, a) < dot(b, b) ? a : b;
  return vec4(gv, uv-gv);
}

float
ring (vec2 uv, vec2 pos, float size)
{
  return abs (sdCircle (uv - pos, size));
}

float
hexRing (vec2 uv, vec2 pos, float size)
{
  return abs (Hexagon (uv - pos) - size);
}

float
starRing (vec2 uv, vec2 pos, float size)
{
  return abs (sdHexagram (uv - pos, size));
}

vec2
rotate (vec2 uv, float angle)
{
  float s = sin(angle);
  float c = cos(angle);
  mat2 rot = mat2(c, -s, s, c);
  return rot*uv;
}

void
main ()
{
  vec2 uv = vTexCoord;
  uv -= .5;
  uv *= 2.;
  if (canvasSize.x > canvasSize.y)
    uv.x *= canvasSize.x / canvasSize.y;
  else
    uv.y *= canvasSize.y / canvasSize.x;

  uv *= 3.5; // scale
  vec2 huv = HexCoord (uv).xy;

  float ts = .5 + .5 * sin (time * .6);
  float tc = .5 + .5 * cos (time * .5) *.4 + .1;

  float s = .5 / sqrt (3.);
  float c = 1.;
  c = min (c, ring (huv, vec2 (  .5,     s ), ts * .5 + .2));
  c = min (c, ring (huv, vec2 ( -.5,     s ), ts * .5 + .2));
  c = min (c, ring (huv, vec2 (  .5,    -s ), ts * .5 + .2));
  c = min (c, ring (huv, vec2 ( -.5,    -s ), ts * .5 + .2));
  c = min (c, ring (huv, vec2 (  0.,  2.*s ), ts * .5 + .2));
  c = min (c, ring (huv, vec2 (  0., -2.*s ), ts * .5 + .2));
  c = min (c, hexRing (huv, vec2 (  .5,     s), tc));
  c = min (c, hexRing (huv, vec2 ( -.5,     s), tc));
  c = min (c, hexRing (huv, vec2 (  .5,    -s), tc));
  c = min (c, hexRing (huv, vec2 ( -.5,    -s), tc));
  c = min (c, hexRing (huv, vec2 (  0.,  2.*s), tc));
  c = min (c, hexRing (huv, vec2 (  0., -2.*s), tc));
  c = min (c, starRing (rotate (huv, sin (time)), vec2 (0.), ts * .2));
  c = clamp (.0125 / abs (c) * (dot (uv, uv) + .5), 0., 1.);

  vec3 col = palette (c, vec3 (.7), vec3 (1.), vec3 (1.), vec3 (.0, .1, .2));
  col *= col;
  col = dust (uv, col, vec3 (hash12 (uv * 314159.26)), .25);

  fragColor = vec4 (col, 1.);
}
