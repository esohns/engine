#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform bvec2 iKeyboard;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define h 2.

vec2
RK4 (vec2 p)
{
  vec2 r = iResolution.xy;

  vec2 posInit = p;
  for(int i=0; i<10; i++)
    p = posInit - h*texture(iChannel1,  p/r).xy;

  return posInit-p;
}

void
main ()
{
  vec2 C = gl_FragCoord.xy;
  vec2 r = iResolution.xy;
  vec2 uv = ((C-r*0.5)/r.y);

  vec4 buf = texture(iChannel0,(C-RK4(C))/r);
  vec2 v = buf.xy;
  float d = buf.z;

  if(C.x<1.||C.x>r.x-1.)
  {
    v.x = .0;
    v.y *= .5;
  }
  if(r.y-1.<C.y||C.y<1.)
  {
    v.y = .0;
    v.x *= .5;
  }

  vec2 m = (iMouse.xy-r*0.5)/r.y;

  if(length(iMouse.xy) < 10.0)
    m = vec2(pow(abs(sin(iTime*1.)), 2.)-0.5, 0.2*sin(iTime*4.)+0.2);

  vec2 mv = m-vec2(texture(iChannel1,vec2(0., 1.0)).w,texture(iChannel1,vec2(1.0)).w);
  if(iFrame<2)
  {
    mv = vec2(0);
    d = 0.0;
  }
  if(texture(iChannel1,vec2(0., 0.49)).w<0.5)
    mv = vec2(0);
  if(length(uv-m)<0.02 && (iMouse.z > 0.5 || length(iMouse.xy) < 10.0))
  {
    float r = length(uv-m)/0.02;
    r = sqrt(1.0 - r*r);
    if(!iKeyboard.y) // S-key
      d = iKeyboard.x ? d*(1.0-r) : d+r; // E-key
    else
      v += mv*100.0*r;
  }

  float mxy = 0.;
  if(uv.x<0.)
  {
    if(uv.y>0.)
      mxy = m.x;
    else
      mxy = iMouse.z;
  }
  else
    mxy = m.y;

  gl_FragColor = vec4(v,d,mxy);
}
