#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float i, s, n, t=iTime*.5, d = .2*fract(dot(u,sin(u)));
  vec3 p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for (o*=i; i++<128.;
       d += s = .008 + abs(3.-length(p.yx))*.15,
       o += (1.+cos(d+vec4(4,2,1,0))) / s)
    for (p = vec3(u * d, d+t),
         p += cos(t+p.yzx*1.8)*1.4,
         n = .3; n < 2.; n += n)
      p += cos(p.yzx*.9)*.6;

  o = tanh(o  / 3e3);

  gl_FragColor = o;
}
