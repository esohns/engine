#version 300 es
precision mediump float;

out vec4 O;

uniform vec2 resolution;
uniform float time;

#define FC gl_FragCoord.xy
#define R resolution
#define T (10.0 + S (0.0, 20.0, floor (20.0 * fract (0.005 * TT))))
#define TT time
#define S smoothstep
#define N normalize
#define gyr(a) dot (sin (a), cos ((a).yzx))

float rnd (vec3 p)
{
  p = fract (p * vec3 (12.9898, 78.233, 156.345));
  p += dot (p, p + 34.56);
  return fract (p.x * p.y * p.z);
}

float
fbm (vec3 p)
{
  p.x -= 2.4;
  float t = 0.0,
        a = 1.0;
  for (int i = 0; i < 7; i++)
  {
    p.xz += vec2 (T * 12.1 / a, t * 0.5);
    a *= 0.5;
    t += gyr (p / (a + 0.00001)) * a;
  }

  return t;
}

float 
map (vec3 p)
{
  return p.y + 0.8 - S (0.0, 1.75, cos (1.518 * fbm (0.1 + p * 0.2)));
}

void
main ()
{
  vec2 uv = (FC - 0.5 * R) / min (R.x, R.y);
  uv.y *= tanh (sin (2.0 * 0.3141592 * TT - 2.0 + rnd (floor (uv.yyy * 9.0)) * 0.5) * 3.0 + 3.0);
  vec3 col = vec3 (0.0),
         p = vec3 (0.0, 0.0, mod (TT, 10.0)),
        rd = N (vec3 (uv, 1.0));
  for (float i = 0.0; i < 80.0; i++)
  {
    float d = map (p) * mix (0.8, 1.0, rnd (p));
    if (d < 0.01 || d > 1.5)
    {
      col += (80.0 - i) / 80.0;
      break;
    }
    p += rd * d;
  }
  col = max (col, 0.08);

//  gl_FragColor = vec4 (col, 1.0);
  O = vec4 (col, 1.0);
}
