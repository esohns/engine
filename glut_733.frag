#version 130

uniform vec2 iResolution;
uniform sampler2D iChannel0;

#define A(u) texture(iChannel0,(u)/iResolution.xy)

void
main ()
{
  vec2 u = gl_FragCoord.xy;
  
  vec2 v = u / iResolution.xy;
  vec4 a = A(u);
  vec2 m = + a.xy
           - vec2(0, 1) * .01
           + float(v.x < .05) * vec2(1, 0)
           + float(v.y < .05) * vec2(0, 1)
           - float(v.x > .95) * vec2(1, 0)
           - float(v.y > .95) * vec2(0, 1);
  float s = 0.;
  float z = 4.;
  for (float i = -z; i <= z; ++i)
    for (float j = -z; j <= z; ++j)
    {
      vec2 c = -m + vec2(i, j);
      s += exp(-dot(c, c));
    }
  if (s == 0.)
    s = 1.;
  s = 1. / s;

  gl_FragColor = vec4(m, s, 0);
}
