#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O = vec4(0.);
  vec2 I = gl_FragCoord.xy;

  float t = iTime,
        i = 0.,
        z,
        d;

  for (O *= i; i++<5e1; O+=.001/d)
  {
    vec3 p = z * normalize(vec3(I+I,0) - iResolution.xyy);
    p.z += 6.;
    for(d=1.; d<9.; d/=.8)
      p += cos(p.yzx*d-vec3(t+t,t,0))/d;
    z += d = .01+.1*length(p.xz);
  }
  O = tanh(O);

  gl_FragColor = O;
}
