//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

//#define PI          3.14159265359
#define PI2         6.28318530718

mat2 rot (float a) { return mat2 (cos (a), sin (a), -sin (a), cos (a)); }
vec3 hue (float t, float f) { return f + f * cos (PI2 * t * (vec3 (1.0, 0.75, 0.75) + vec3 (0.96, 0.57, 0.12))); }
float hash21 (vec2 a) { return fract (sin (dot (a, vec2 (27.69, 32.58))) * 43758.53); }
float box (vec2 p, vec2 b) { vec2 d = abs (p) - b; return length (max (d, 0.0)) + min (max (d.x, d.y), 0.0); }

mat2 r90;

vec2
pattern (vec2 p, float sc)
{
  vec2 uv = p;
  vec2 id = floor (p * sc);
  p = fract (p * sc) - 0.5;

  float rnd = hash21 (id);

  if (rnd > 0.5)
    p *= r90;
  rnd = fract (rnd * 32.54);
  if (rnd > 0.4)
    p *= r90;
  if (rnd > 0.8)
    p *= r90;

  rnd = fract (rnd * 47.13);

  float tk = 0.075;
  float d = box (p - vec2 (0.6, 0.7), vec2 (0.25, 0.75)) - 0.15;
  float l = box (p - vec2 (0.7, 0.5), vec2 (0.75, 0.15)) - 0.15;
  float b = box (p + vec2 (0.0, 0.7), vec2 (0.05, 0.25)) - 0.15;
  float r = box (p + vec2 (0.6, 0.0), vec2 (0.15, 0.05)) - 0.15;
  d = abs (d) - tk;

  if (rnd > 0.92)
  {
    d = box (p - vec2 (-0.6,  0.5), vec2 (0.25, 0.15)) - 0.15;
    l = box (p - vec2 ( 0.6,  0.6), vec2 (0.25)) - 0.15;
    b = box (p + vec2 ( 0.6,  0.6), vec2 (0.25)) - 0.15;
    r = box (p - vec2 ( 0.6, -0.6), vec2 (0.25)) - 0.15;
    d = abs (d) - tk;
  }
  else if (rnd > 0.6)
  {
    d = length (p.x - 0.2) - tk;
    l = box (p - vec2 (-0.6, 0.5), vec2 (0.25, 0.15)) - 0.15;
    b = box (p + vec2 ( 0.6, 0.6), vec2 (0.25)) - 0.15;
    r = box (p - vec2 ( 0.3, 0.0), vec2 (0.25, 0.05)) - 0.15;
  }

  l = abs (l) - tk;
  b = abs (b) - tk;
  r = abs (r) - tk;

  float e = min (d, min (l, min (b, r)));

  if (rnd > 0.6)
  {
    r = max (r, -box (p - vec2 ( 0.2, 0.2), vec2 (tk * 1.3)));
    d = max (d, -box (p + vec2 (-0.2, 0.2), vec2 (tk * 1.3)));
  }
  else
  {
    l = max (l, -box (p - vec2 (0.2, 0.2), vec2 (tk * 1.3)));
  }

  d = min (d, min (l, min (b, r)));

  return vec2 (d, e);
}

void
main ()
{
  vec3 C = vec3 (0.0);
  vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / max (iResolution.x, iResolution.y);
  r90 = rot (1.5707);

  uv *= rot (iTime * 0.095);
  uv = vec2 (log (length (uv)), atan (uv.y, uv.x) * 6.0 / PI2);
  //uv = vec2 (log (length (uv)), atan (uv.y, uv.x)) * 8.0 / PI2;

  float scale = 8.0;
  for (int i = 0; i < 4; i++)
  {
    float ff = (i * 0.05) + 0.2;
    uv.x += iTime * ff;

    float px = fwidth (uv.x * scale);
    vec2 d = pattern (uv, scale);
    vec3 clr = hue (sin (uv.x + (i * 8.0)) * 0.2 + 0.4, (0.5 + i) * 0.15);
    C = mix (C, vec3 (0.001), smoothstep (px, -px, d.y - 0.04));
    C = mix (C, clr, smoothstep (px, -px, d.x));
    scale *= 0.5;
  }

  C = pow (C, vec3 (0.4545));
  gl_FragColor = vec4 (C, 1.0);
}
