#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i = 0., d = 0., s, t = iTime * 2.;
  vec3 q, p = iResolution.xyx;
  u = (u - p.xy / 2.) / p.y;
  for (o *= i; i++ < 1e2;)
  {
    q = p = vec3(u * d, d + t);
    for (s = .03; s < 2.;
         p += abs(dot(sin(p * s * 4.), vec3(.035))) / s,
         s += s);
    d += s = .04 + .6 * abs(min(.2 - q - cos(p.x) * .2, 2.5 + p).y);
    o += 1. / s;
  }
  o = tanh(vec4(4, 2, 1, 1) * o / 4e3 / length(u - .1));

  gl_FragColor = o;
}
