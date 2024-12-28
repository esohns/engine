//precision mediump float;

uniform float time;
uniform vec2 resolution;

#define FC gl_FragCoord.xy
#define R resolution
#define T (2.8+time)
#define S smoothstep
#define N normalize
#define MN min(R.x,R.y)
#define TAU radians(360.0)
#define hue(a) (0.6 + 0.6 * cos (TAU * a - vec3 (0, 83, 21)))
#define gold vec3 (255.0, 233.0, 152.0) / 255.0
#define bron vec3 (87.0, 55.0, 13.0) / 255.0
#define rot(a) mat2 (cos (a - vec4 (0.0, 11.0, 33.0, 0.0)))

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
  vec2 i = floor (p), f = fract (p), u = f * f * (3.0 -2.0 * f);
  float a = rnd (i),
        b = rnd (i + vec2 (1.0, 0.0)),
        c = rnd (i + vec2 (0.0, 1.0)),
        d = rnd (i + 1.0);
  return mix (mix (a, b, u.x), mix (c, d, u.x), u.y);
}

float
fbm (vec2 p)
{
  float t =0.0, a = 1.0;
  mat2 m = mat2 (1.6, -0.6, 0.2, 1.6);
  for (int i = 0; i < 5; i++)
  {
    t += a * noise (p);
    p *= m * 2.0;
    a *= 0.5;
  }

  return t;
}

vec3
pattern (vec2 uv)
{
  vec3 col = vec3 (0.0);
  vec2 p = uv;
  float d = 1.0, t = -T * 0.0025;
  for (int i = 0; i < 3; i++)
  {
    p = vec2 (cos (0.125 * sin (uv.x)), sin (uv.y) * 0.25);
    float k = p.x + p.y;
    d = fbm (8.0 * (uv - p) * k * d);
    uv *= 0.5;
    col += hue (d) / 3.0;
  }
  col = mix (col, vec3 (0.5), 0.35);
  col = mix (col, col * col * col * col, col);
  col = S (0.0, 1.0, col);
  col = pow (col, vec3 (0.4545));
  return col;
}

float
cLength (vec2 p, float k)
{
  p = abs (p);
  return pow (pow (p.x, k) + pow (p.y, k), 1.0 / k);
}

float
spiral (vec2 p, float t)
{
  float r = dot (p, p),
        a = atan (p.y, p.x) / TAU;
  return sin (fract (0.05 / (r + 0.01) * t + a));
}

float
circle (vec2 p, float r, float w, float b, float k)
{
  float d = cLength (p, k);
  return S (b, -b, abs (d - r) - w);
}

void
main ()
{
  vec2 uv = (FC - 0.5 * R) / MN;
  float px = 1.0 / MN,
        bs = MN * 0.05,
        alpha = sin (T),
        rhm = S (1.0, 0.0, 0.5 + 0.5 * sin (T)),
        ch = 0.8 + rhm * 1.2,
        fig = circle (uv, 0.25, 0.125, px, ch),
        shadow = circle (uv - vec2 (-0.03, 0.0) * rot (0.5 - 1.0 * alpha), 0.255 - (0.025 - rhm * 0.025), 0.1, px * (40.0 + rhm * 20.0), ch),
        spiral = spiral (uv * rot (10.0 * (0.5 + 0.5 * cos (T))), 0.118 + rhm),
        anima = sin (exp (-clamp (log (spiral), -3.0, 0.0)));
  vec2 p = fract (uv * 2.0);
  float g = mod (p.x + p.y, 2.0),
        n = g * pow (noise (uv * 3.4 - vec2 (0.3, -0.05)), 2.0) * anima;
  vec3 c = mix (gold, bron, n),
       col = mix (pattern (FC / MN), mix (gold, bron, noise (uv * 2.0) * (anima * (0.5 * dot (uv, uv)))), 0.5);
  c /= 1.0 + exp (-c * c);
  col /= 1.0 + exp (-col * col * col);
  col = mix (col, vec3 (0.0), min (shadow, 0.65));
  col = mix (col, c, fig);
  if (FC.x > R.x-bs || FC.x < bs || FC.y > R.y-bs || FC.y < bs)
  {
    float r = rnd (uv);
    col = mix (col, bron, mix (0.6, 0.7, noise (r * uv * 84.0)));
    col = mix (gold, col, mix (0.8, 1.0, noise (fract (r * 12.34) * uv * 4.0)));
    col /= 1.0 + exp (-col);
    col = sqrt (col);
  }

  gl_FragColor = vec4 (col, 1.0);
}
