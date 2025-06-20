#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i = 0., d, s, s1, s2, n, t=sin(iTime*.4)+iTime*.4;
  vec3 p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for(o*=i;
      i++<128.;
      d += s = .001 + abs(max(s1,s2))*.1, o += 1. / s)
    for (p = vec3(u * d, d+t),
         s1 = 1.-length(p.xy),
         s2 = 2.-length(p.xy),
         p.xy -= cos(t+vec2(.15,.19)*p.z)*8.,
         n = .3;
         n < 4.;
         n += n )
      p += cos(t+p.yzx),
      s1 += abs(dot(sin(t+p*n*1.), vec3(.9))) / n,
      s2 += abs(dot(sin(t+p*n*2.), vec3(.6))) / n;
  o = tanh(vec4(4, 2, s1 > s2?1:9,1)*o*o  / 7e8);

  gl_FragColor = o;
}
