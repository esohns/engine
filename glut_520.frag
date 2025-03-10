#version 130

uniform float iTime;

void
main ()
{
  vec4 c = mat2x4 (-1,1,1,0,1,1,0,0) * gl_FragCoord.xy * .01;
  for (int n = 0; n++ < 6;)
    c = (c + sin (c.zxyw + iTime * sin (float (n) + vec4 (0,2,4,0)))) * 2.;

  gl_FragColor = c;
}
