#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i,d,s,t=iTime*1e2;
  vec3 p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for (o*=i; i++<1e2;)
  {
    p = vec3(u * d,d+t);
    p.x += sin(p.z*.03)*8.;
    s = 36. + p.y + sin(p.z*.01) * 32. + abs(p.x*.08)*9.;
    for (float a = .05; a < 4.;
         s += abs(dot(sin(p * a ), vec3(.1))) / a,
         a *= 1.4);
    d += s = .03 + abs(s)*.2;
    o += vec4(1,2,4,0) / s;
  }
  o = tanh(o / 2e3 / dot(u-=.35,u));

  gl_FragColor = o;
}
