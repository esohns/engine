#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;
  
  float t = iTime,i = 0.,z = 0.,d = 0.;
  for(O *= i;i++<1e2;
      O += (cos(z+t+vec4(6,1,2,3))+1.)/d)
  {
    vec3 p=z*normalize(vec3(I+I,0)-iResolution.xyy);
    p.z-=t;
    for(d=1.;d<9.;d/=.7)
      p+=cos(p.yzx*d+z*.2)/d;
    z+=d=.02+.1*abs(3.-length(p.xy));
  }
  O = tanh(O/3e3);
  
  gl_FragColor = O;
}
