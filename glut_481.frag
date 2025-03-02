#version 130

uniform vec2 iResolution;
uniform sampler2D iChannel0;

void
main ()
{
  gl_FragColor = vec4 (texture (iChannel0, gl_FragCoord.xy / iResolution.xy).rgb, 1.0);
}
