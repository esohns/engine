#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 u = gl_FragCoord.xy;

  float d = 0., i = 0., s, n, l, t = iTime*.1;
  vec3 q,p = iResolution.xyx;
  u = (u-p.xy/2.)/p.y;
  for(o*=i; i++<90.;
      d += s = .025 + abs(min(l,s))*.175,
      o += 1. / s)
    for(q = p = vec3(u * d, d),
        s =  .3+p.y,
        q = cos(4.*t+p.zxy*p.yzz),
        l = dot(abs(q-floor(q)-.5), vec3(1)),
        n = 2.;
        n < 32.;
        n += n)
      s += abs(dot(sin(3.*t + p*n ), vec3(.5))) / n;
  
  o = tanh(vec4(3,1,7,1) * o / d / 4e3 / length(u));

  gl_FragColor = o;
}
