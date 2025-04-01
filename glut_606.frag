#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define R iResolution.xy
#define M iMouse

void
main ()
{
  vec2 u = (gl_FragCoord.xy-.5*R)/R.y,
       m = (M.xy-.5*R)/R.y;
  float r = M.z==1.?.03:-1.,
        d = smoothstep(100./R.y,0.,length(u-m)-r);
  gl_FragColor = vec4(d+texture(iChannel0,gl_FragCoord.xy/R)*0.97);
}
