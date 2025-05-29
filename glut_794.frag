#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  float t = iTime,
        i,
        z,
        d;

  for (O *= i; i++ < 50.; O += (sin(z / 3. + vec4(7., 2., 3., 0.)) + 1.1) / d )
  {
    vec3 p = z * normalize(vec3(I+I,0)-iResolution.xyy);
    p.z += 5. + cos(t);
    p.xz *= mat2(cos(p.y * .5 + vec4(0, 33, 11, 0))) / max(p.y * .1 + 1., .1);
    for (d = 2.; d < 15.; d /= .6)
      p += cos((p.yzx - vec3(t/.1, t, d) ) * d ) / d;
    z += d = .01 + abs(length(p.xz) + p.y * .3 - .5) / 7.;
  }
  O = tanh(O / 1e3);

  gl_FragColor = O;
}
