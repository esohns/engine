#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;
  
  float i = 0., d, z;
  for(O *= i; i++<1e2; O += (cos(z+vec4(6,1,2,3))+1.)/d)
  {
    vec3 p = z * normalize(vec3(I+I,0)-iResolution.xyy);
    p.z -= iTime;

    for(d = .4; d < 3e1; d += d)
      p += cos(round(p*d)-z*.1).yzx/d;

    z += d = length(sin(p.xy))*.1;
  }

  O = tanh(O/5e3);
  gl_FragColor = O;
}
