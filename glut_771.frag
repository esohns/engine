uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  for(float i=-1.; i<1.; i+=6e-3)
  {
    vec2 r = iResolution.xy,
         p = cos(i*4e2+iTime+vec2(0,11))*sqrt(1.-i*i);
    O += (cos(i+vec4(0,2,4,6))+1.)*(1.-p.y) / dot(p=(I+I-r)/r.y+vec2(p.x,i)/(p.y+2.),p)/3e4;
  }

  gl_FragColor = O;
}
