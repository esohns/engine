#version 130

uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  //uv.y = -uv.y;
  vec2 mouse = iMouse.xy / iResolution.xy;
  vec2 warp = normalize(iMouse.xy - gl_FragCoord.xy) * pow(distance(iMouse.xy, gl_FragCoord.xy), -2.0) * 50.0;
  //warp.y = -warp.y;
  uv = uv + warp;

  float light = clamp(0.1*distance(iMouse.xy, gl_FragCoord.xy) - 1.5, 0.0, 1.0);

  vec4 color = texture(iChannel0, uv);
  gl_FragColor = color * light;
}
