#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

vec3
texSample (int x, int y, vec2 fragCoord)
{
  vec2 txSizef = vec2(textureSize (iChannel0, 0));
  vec2 uv = fragCoord.xy / iResolution.xy * txSizef;
  uv = (uv + vec2(x, y)) / txSizef;
  return texture(iChannel0, uv).xyz;
}

vec3
sharpenFilter (vec2 fragCoord, float strength)
{
  vec3 f =
  texSample(-1,-1, fragCoord) *  -1. +
  texSample( 0,-1, fragCoord) *  -1. +
  texSample( 1,-1, fragCoord) *  -1. +
  texSample(-1, 0, fragCoord) *  -1. +
  texSample( 0, 0, fragCoord) *   9. +
  texSample( 1, 0, fragCoord) *  -1. +
  texSample(-1, 1, fragCoord) *  -1. +
  texSample( 0, 1, fragCoord) *  -1. +
  texSample( 1, 1, fragCoord) *  -1.  ;
  return mix(texSample( 0, 0, fragCoord), f , strength);    
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec2 uvMouse = iMouse.xy/iResolution.xy;
  if(uv.x > uvMouse.x)
    gl_FragColor = vec4(sharpenFilter(gl_FragCoord.xy, uvMouse.y),1.0);
  else
    gl_FragColor = vec4(texSample( 0, 0, gl_FragCoord.xy),1.0);
  if(uv.x > uvMouse.x - 0.002 && uv.x < uvMouse.x + 0.002)
    gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}
