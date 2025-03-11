#version 130

uniform float iTime;

void
main ()
{
  vec3 p = mat2x3 (-1.0, vec4 (1.0), 0.0) * gl_FragCoord.xy * 0.01;
  for (int n = 0; n++ < 6;)
    p = (p + sin (p.zxy + iTime)) * 2.0;
  gl_FragColor.xyz = abs (sin (p * 0.01 - iTime));
}
