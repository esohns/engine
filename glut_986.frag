#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

void
main ()
{
// gl_FragColor = texture(iChannel0, (floor( (gl_FragCoord.xy*.98 + iResolution*.01 + (gl_FragCoord.xy-iResolution/2.).yx*vec2(-.03,.03)) )+.5) / iResolution);
  gl_FragColor = texture(iChannel0, (gl_FragCoord.xy * .98 + iResolution * .01 + (gl_FragCoord.xy - iResolution / 2.).yx * vec2(-.03, .03)) / iResolution);

  float t = iTime * .5;
  vec4 col = vec4(sin(t * vec3(13, 11, 17)) * .5 + .5, 1);
  float idx = .0 + 1.0 * smoothstep(6., 20., length(gl_FragCoord.xy - sin(vec2(11, 13) * t) * 60. - iResolution / 2.));

  gl_FragColor = mix(col, gl_FragColor, idx);
}
