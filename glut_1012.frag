uniform vec2 iResolution;
uniform float iTime;

#define N normalize

void
main ()
{
  vec4 o;
  vec2 C = gl_FragCoord.xy;
  vec3 p, Z, I = vec3 (iResolution.xy, 0), S;
  vec2 A = vec2 (.4,.3), s;

  C -= .5 * I.xy;
  s = sin (A * iTime);
  S = N (cross (vec3 (0., 1., 0.) + vec3 (A * A * s, 0.), Z = N (vec3 (A * cos (A * iTime), 1.))));
  I = N (C.y * cross (S, Z) + I.y * Z - C.x * S);
  S = vec3 (s, iTime);

  vec4 O;
  for (float i, d, z; ++i < 77.; O += o.w / d * o)
    p = z * I + S
  , p.xy -= s = sin (A * p.z)
  , Z = N (vec3 (A * cos (A * p.z), -1.))
  , p -= dot (p.xy, Z.xy) * .5 * Z
  , p.xy *= mat2 (cos (vec4 (0., 11., 33., 0.) - (A * A * s).x))
  , o = .9 + sin (2. * abs (p.x) + vec4 (8., 3., 4., 2.))
  , p -= round (p)
  , z += d = 1e-3+.7 * abs (.53 - sqrt (length (p * p)))
  ;

  o = tanh (O / 2E4);

  gl_FragColor = o;
}
