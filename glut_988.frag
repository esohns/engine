#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

void
main()
{
  vec4 c;
  vec2 p = gl_FragCoord.xy;

  vec2 k = p - iResolution / 2.;
  float t = iTime;
  vec2 d = vec2(0);
  for(int i = 0; i++ < 50; t += 6.28 * 1.618)
    d += sin((k * .05 + sin(t + vec2(47,23))) * mat2(sin(t * .0007 * vec4(17,14,11,21)))) * mat2(sin(t * .01 * vec4(17,14,11,21)));
  d *= pow(length(d),-.4) * .7;
  vec2 h = (p + d) / iResolution;
  c = texture(iChannel0,h);
  c = mix(c,(sin(sin(((p.x * .3 + vec4(37,21,42,89))) * (p.y * .3 + 10.)) * float(iFrame) * .1) * .5 + .5),iFrame < 5 ? .7 : .01);

  gl_FragColor = c;
}
