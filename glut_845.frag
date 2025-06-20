#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O = vec4(0.);
  vec2 I = gl_FragCoord.xy;

  float i = 0.,
        z = fract(dot(I,sin(I))),
        d;
  for(O *= i; i++<1e2; O+=(sin(z+vec4(6,2,4,0))+1.5)/d)
  {
    vec3 p = z * normalize(vec3(I+I,0) - iResolution.xyy);
    p.z += 6.;
    for(d=1.; d<9.; d/=.8)
      p += cos(p.yzx*d-iTime)/d;
    z += d = .002+abs(length(p)-.5)/4e1;
  }
  O = tanh(O/7e3);

  gl_FragColor = O;
}
