#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o = vec4(0.);
  vec2 u = gl_FragCoord.xy;
  float i=0.,a=0.,d=0.,s=0.,t=iTime;
  vec3 p = vec3 (iResolution.xy, 0.);
  u = (u-p.xy/2.)/p.y;
  for(o*=i; i++<1e2;
      d += s = .04 + abs(s)*.1,
      o += 1./s)
    for (p = vec3(u * d, d + t),
         s = 4.+p.y,
         a = .05; a < 2.; a += a)
      p += cos(t+p.yzx)*.03,
      s -= abs(dot(sin(.5*t+p * a * 6.), vec3( .05))) / a;
  u -= vec2(-.5, .4);
  o = tanh(o*o / 3e6 / dot(u,u));
  
  gl_FragColor = o;
}
