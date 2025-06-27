#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i = 0., d = 0., s, n, t = iTime;
  vec3 p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for (o*=i; i++<1e2;)
  {
    p = vec3(u * d, d + t*4.);
    p += cos(p.z+t+p.yzx*.5)*.6;
    s = 4.+sin(t*.7)*4.-length(p.xy);
    p.xy *= mat2(cos(t+vec4(0,33,11,0)));
    for (n = 1.6; n < 32.; n += n)
      s -= abs(dot(sin( p.z + t + p*n ), vec3(1.12))) / n;
    d += s = .01 + abs(s)*.1;
    o += 1. / s;
  }
  o = tanh(vec4(5,2,1,1) * o * o / d / 2e6);

  gl_FragColor = o;
}
