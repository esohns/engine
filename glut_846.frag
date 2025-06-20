#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main () 
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i,d=.5*fract(dot(u,sin(u))),s,n,t=iTime*42.;
  vec3 q, p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for(o*=i; i++<1e2;
      d += s = .035 + abs(s)*.22,
      o += 1. / s)
    for (p = vec3(u * d, d + t),
         p.xy -= cos(vec2(.2)*p.z)*5.,
         s  = sin(t*.008)*24.+25.  - length(p.xy),
         p.xy *= mat2(cos(t*.004+vec4(0,33,11,0))),
         s = max(s, dot(cos(p), vec3(1))),
         n = .15; n < 2.;
         n += n)
      s -= abs(dot(sin(p*n), vec3(1.))) / n;
  o = tanh(vec4(1,6,2,1)*o / d / 5e1 / length(u));

  gl_FragColor = o;
}
