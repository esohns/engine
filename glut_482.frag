#version 130
//precision mediump float;

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  float i = 0.0,
        z,
        t = iTime;
  for (gl_FragColor *= i; i < 1.0; i += 0.01)
  {
    vec2 v = iResolution.xy,
         p = (gl_FragCoord.xy + gl_FragCoord.xy - v) / v.y * i;
    p /= 0.2 + sqrt (z = max (1.0 - dot (p, p), 0.0)) * 0.3;
    p.y += fract (ceil (p.x = p.x / 0.9 + t) * 0.5) + t * 0.2;
    v = abs (fract (p) - 0.5);
    gl_FragColor += vec4 (2.0, 3.0, 5.0, 1.0) / 2e3 * z / (abs (max (v.x * 1.5 + v, v + v).y - 1.0) + 0.1 - i * 0.09);
  }

  gl_FragColor = tanh (gl_FragColor * gl_FragColor);
}
