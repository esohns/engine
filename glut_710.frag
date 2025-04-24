#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define T(a) texture(iChannel0,p.xz*.1-t*a)

void
main ()
{
  vec4 o;
  vec2 c = gl_FragCoord.xy;

  vec4 p = vec4(c,0.,1.)/iResolution.xyxy-.5, d=p, e;
  float t = iTime+6., x;
  d.y -= .2;
  p.z += t*.3;
  for(float i=1.; i>0.; i-=.02)
  {
    e = sin(p*6.+t);
    x = abs(p.y+e.x*e.z*.1-.75)-(e=T(.01)+T(.02)).x*.08;
    o = .3/length(p.xy+vec2(sin(t),-.4)) - e*i*i;
    if(x<.01)
      break;
    p -= d*x*.5;
  }

  gl_FragColor = o;
}
