#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;

  float value = texture(iChannel1, uv).r * 100. * (sin(iTime)*0.5+0.5);

  gl_FragColor = textureGrad(iChannel0, uv + vec2(iTime*0.1,0), dFdx(vec2(value)), vec2(0));
}
