#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  vec3 p;
  float i = 0.,z = 0.,d = 0.,t = iTime;
  for (O *= i;i++ < 99.;O.rgb += max(sin(z+p), .1) / d / 1e3)
    p = z * normalize( vec3(I+I,tanh(sin(t*.5)*9.)*I.x) - iResolution.xyy),
    p.x += tanh(sin(t))*9.,
    p.z -= t*4.,
    z += d = .02 + abs(dot(cos(t+p.yzx)*.4,cos(p)));
  O = tanh(O);

  gl_FragColor = O;
}
