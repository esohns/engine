//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

#define T iTime
#define r(v,t) { float a = (t) * T, c = cos (a), s = sin(a); v *= mat2 (c,s,-s,c); }
#define SQRT3_2 1.26

void
main ()
{
  vec4 p = vec4 (gl_FragCoord.xy, 0.0, 1.0) / iResolution.yyxy - 0.5,
       d,
       c;
  p.x -= 0.4;
  r (p.xz, 0.13); r (p.yz, 0.2); r (p.xy, 0.1);
  d = p;
  p.z += 5.0 * T;
  gl_FragColor = vec4 (0.0);
  float closest = 999.0;
  vec4 u_c = gl_FragColor;

  for (float i = 1.0; i > 0.0; i -= 0.01)
  {
    vec4 u = floor (p / 8.0),
         t = mod (p, 8.0) - 4.0,
         ta;
    // r (t.xy, u.x); r (t.xz, u.y); r (t.yz, 1.0);
    u = sin (78.0 * (u + u.yzxw));
    // t -= u;
    c = p / p * 1.2;

    float x1, x2,
          x = 1e9;
    for (float j = 2.3; j > 1.0; j -= 0.3)
    {
      r (t.xy, u.x); r (t.xz, u.y); r (t.yz, u.z);
      ta = abs (t);
      x1 = length (t.xyz) - j * SQRT3_2;
      x2 = max (ta.x, max (ta.y, ta.z)) - j;
      x2 = max (-x1, x2);
      x1 = length (t.xyz) - j * SQRT3_2 - 0.1;
      x2 = max (x1, x2);
      x = min (x, x2);
    }

    x1 = length (t.xyz) - 0.6;
    if (x1 < closest)
    {
      closest = x1;
      u_c = u;
    }
    x = min (x1, x);
    if (x == x1)
      c = u * 3.0;
    // else
    // c += u * 3.0 * pow (abs ((x - x1)), 0.2) * (0.5 + 0.5 * sin (0.5 * T));

    // gl_FragColor = i * i * c;
    if (x < 0.01)
    {
      gl_FragColor = i * i * c;
      break;
    }

    p -= d * x;
  }

  if (cos (0.25 * T) > 0.0)
    gl_FragColor += u_c * exp (-closest) * (0.5 + 0.5 * cos (0.5 * T));
}
