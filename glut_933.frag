#version 130

// glut_933_common.glsl
#define M void main () { vec4 o; vec2 i = gl_FragCoord.xy; o=
#define f vec2(1.1)
#define S 2.5 + fract(iTime/3.5)
#define G 0.8*cos(8.*i.x/iResolution.x)
#define T(d,l) texture(iChannel0, (i+d)/iResolution, l)
// glut_933_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

M
  step(.01, T(f, 0.) - T(f, S+G));
  o.r = (o.a > .5) ? T(f, 0.).r : -.5;
  if (T(f, S+G).r < .01)
    o.r += .1;
  
  gl_FragColor = o;
}