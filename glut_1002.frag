uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;
  
  vec2 v = iResolution,
       p = (I + I - v) / v.y / .25;

  float i = 0., f;
  for (O *= i; i++ < 9.; O += (cos (i + vec4 (0., 1., 2., 3.)) + 1.) / 6. / length (v))
    for (v = p, f = 0.; f++ < 9.; v += sin (v.yx * f + i + iTime) / f);

  O = tanh (O * O);
  gl_FragColor = O;
}
