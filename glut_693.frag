#version 130

// glut_693_common.glsl
#define ZOOM .25
#define EMPTY 1024.
#define FTOV .125
#define VTOP (FTOV*2.)
#define FRC1 4.
#define FRC2 1.

vec4
rand (vec2 u)
{
  return fract(sin(vec4(dot(u,vec2(23.123,87.987)),
                        dot(u,vec2(34.234,96.876)),
                        dot(u,vec2(45.345,15.765)),
                        dot(u,vec2(56.456,24.654))))*45678.7654);
}
// glut_693_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main ()
{
  vec4 t = vec4(EMPTY);
  if(iFrame == 0)
  {
    t = rand(gl_FragCoord.xy)*vec4(2.,2.,.02,.02)-vec4(1.,1.,.01,.01);
    if(fract(sin(dot(gl_FragCoord.xy,vec2(12.989,78.233)))*43758.5453)<.9)
      t = vec4(EMPTY);
  }
  vec2 r = 1./iResolution.xy;
  vec2 a = floor(gl_FragCoord.xy*.5)*2.-2.;
  float id = dot(floor(fract(gl_FragCoord.xy*.5)*2.),vec2(1.,2.))+1.;
  float id2 = 0.;
  for(float i = .5; i < 6.; ++i)
    for(float j = .5; j < 6.; ++j)
    {
      vec2 m = vec2(j,i);
      vec4 t2 = texture(iChannel0,(a+m)*r).xyzw;
      if(t2.x == EMPTY)
        continue;
      t2.xy += 2.*(floor(m*.5)-1.);
      if(t2.x < 1. && t2.x >=-1. &&
         t2.y < 1. && t2.y >=-1. )
        ++id2;
      if(abs(id2-id)<.1)
      {
        gl_FragColor = t2;
        return;
      }
    }
  gl_FragColor = t;
}
