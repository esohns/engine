#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define R iResolution.xy
#define A(U) texture(iChannel0, (U)/R)

void
main ()
{
  vec4 Q;
  vec2 U = gl_FragCoord.xy;

  for (int i = 0; i < 5; i++) {
      vec4 a = A(U);
      U -= a.x*a.zw;
  }
  Q = A(U);
  vec4 
      n = A(U+vec2(0,1)),
      e = A(U+vec2(1,0)),
      s = A(U-vec2(0,1)),
      w = A(U-vec2(1,0)),
      a = A(U+vec2(1,1)),
      b = A(U+vec2(1,-1)),
      c = A(U-vec2(1,1)),
      d = A(U-vec2(1,-1)),
      dQ = 0.125*(n+e+s+w+a+b+c+d)-Q;
  Q = A(U);
  Q += vec4(0.5,1,1,1)*dQ;
  float x = .1*Q.y*Q.x*(1.-Q.x);
  Q.x = Q.x+x-0.00+(e.z*e.x-w.z*w.x+n.w*n.x-s.w*s.x);
  Q.y = Q.y-x+0.04*Q.y*(1.-Q.y);
  Q.xy = max(Q.xy,0.);
  Q.zw = Q.zw - .025*vec2(e.y-w.y,n.y-s.y);
  if (U.x < 1. || U.y < 1. || R.x-U.x<1. || R.y-U.y<1.)Q.x*=0.;
  if (iMouse.z>0.&&length(iMouse.xy-U)<40.)Q*=0.;
  if (iFrame < 1) Q = vec4(exp(-.1*length(U-0.5*R)),1,0,0);
  
  gl_FragColor = Q;    
}