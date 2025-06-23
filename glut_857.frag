#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  float t = iTime,
        i = 0.,
        z,
        d;
  for (O *= i; i++ < 1e2;
       O += vec4(z / 7., 2, 3, 1) / d)
  {
    vec3 p = z * normalize(vec3(I+I,0) - iResolution.xyy);
    p.xy *= mat2(cos((z + t) * .1 + vec4(0, 33, 11, 0)));
    p.z -= 5. * t;

    for (d = 1.; d < 9.; d /= .7)
      p += cos(p.yzx * d + t) / d;

    z += d = .02 + abs(2. - dot(cos(p), sin(p.yzx * .6))) / 8.;
  }

  O = tanh(O * O / 1e7);

  gl_FragColor = O;
}
