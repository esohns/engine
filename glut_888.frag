#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 C = gl_FragCoord.xy;

  vec2 R = iResolution;

  mat2 M = mat2(cos(.1 * iTime + vec4(0, 11, 33, 0)));

  vec4 o, P, q;

  for(float i, j, z, d, s, D; ++i < 55.; z += .6 * d + 1E-3)
  {
    q = vec4(normalize(vec3((C + C - R) / R.y, 2)) * z, .5);

    q.z -= 2.;

    q.yw *= M;
    q.wx *= M;
    q.zw *= M;

    P = q;

    for(d = s = j = 1.; ++j < 9.; s *= D = dot(P, P), P = abs(P.ywzx) / D - .1 * vec4(7, 5, 9, 5))
      d = min(d, length(P) * s);

    P = 1. + cos(.7 * vec4(2, 1, 0, 3. * length(q.xy)) + 1. + 8. * length(q));

    o += P.w * P / d;
  }

  O = tanh(o / 3E3);

  gl_FragColor = O;
}
