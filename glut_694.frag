#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const float START_SPEED  = 2.7;
const float MELT_SPEED   = 1.;
const float RESTART_IVAL = 3.;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec2 p = uv;

  float t = mod(iTime, RESTART_IVAL);

  float rt = mod(iTime, 2. * RESTART_IVAL);
  bool texFlip = rt > .0 && rt < RESTART_IVAL;

  float d = START_SPEED * t;
  if (d > 1.)
    d = 1.;

  p.y += d * 0.35 * fract(sin(dot(vec2(p.x, .0), vec2(12.9898, 78.233)))* 43758.5453);

  if(d == 1.)
    p.y += MELT_SPEED * (t - d/START_SPEED);

  if(texFlip)
    gl_FragColor = texture(iChannel0, p);
  else
    gl_FragColor = texture(iChannel1, p);

  if(p.y > 1.)
  {
    if(texFlip)
      gl_FragColor = texture(iChannel1, uv);
    else
      gl_FragColor = texture(iChannel0, uv);
  }
}
