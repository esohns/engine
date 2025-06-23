#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main () 
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float
  i = 0.,
  d,
  s,
  n,
  t = iTime*.3;
  vec3 p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for(o*=i; i++<1e2;)
  {
    p = vec3(u * d, d + t*4.);
    p += cos(p.z+t+p.yzx*.5)*.5;
    s = 5.-length(p.xy);
    for (n = .06; n < 2.;
         p.xy *= mat2(cos(t*.1+vec4(0,33,11,0))),
         s -= abs(dot(sin(p.z+t+p * n * 20.), vec3( .05))) / n,
         n += n);
    d += s = .02 + abs(s)*.1;
    o += 1. / s;
  }

  o = tanh(o / d / 9e2 / length(u));

  gl_FragColor = o;
}
