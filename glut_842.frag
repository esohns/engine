#version 130

uniform vec2 iResolution;
uniform float iTime;

const vec4 f = vec4(.7, 1., 1.3, 0.);
const vec4 o = vec4(0., 1., 2., 0.);

void
main ()
{
  vec2 p = (gl_FragCoord.xy * 2. - iResolution) / iResolution.y;

  float n = mod(dot(gl_FragCoord.xy, sin(gl_FragCoord.yx)), .1);

  gl_FragColor = tanh(.2 / abs(p.y + .3 * cos(iTime + n + p.x * f + o)));
}
