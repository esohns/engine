#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;
  
  float i = 0.,d = 0.,s,t=iTime;
  vec3  p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for(o*=i; i++<1e2;)
  {
    p = vec3(u* d,d+t);
    for (s = .15; s < 1.;
         p += cos(t+p.yzx*.6)*sin(p.z*.1)*.2,
         p.y += sin(t+p.x)*.03,
         p += abs(dot(sin(p * s * 24.), p-p+.01)) / s,
         s *= 1.5);
      d += s = .03 + abs(2.+p.y)*.3;
    o += vec4(1,2,4,0) / s;
  }

  o = tanh(o / 7e3 / dot(u-=.35,u));

  gl_FragColor = o;
}
