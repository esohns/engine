uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 o;
  vec2 C = gl_FragCoord.xy;
  float d, e, i, s, z;
  vec2 r = iResolution.xy;
  vec4 p, O;

  for (mat2 R = mat2 (cos (iTime / 27. + vec4 (0., 11., 33., 0.)));
       p = vec4 (z * normalize (vec3 (C - .5 * r, r.y)), .6), --p.z, p.xw *= R, p.wy *= R, p.zw *= R, o = p, ++i < 77.;
       p = abs (p / s), d = min (length (p.xy) + 3e-4, min (p.z, p.w) + 3e-3), p = e * e + 3. + 3. * cos (13. * dot (o.xy, o.zw) + vec4 (2., 7., 0., 4.)), O += p.w / (7. * e * e * e * d + d) * exp (-z * z * .5) * p, z += .6 * d + 1e-3)
    for (s = d = 1.; ++d < 7.; s *= e)
      p -= 2. * round (p * .5), p *= e = 2. / dot (p, p);
  o = tanh (O / 2e4 - length (C / r - .5) / 5. * vec4 (2., 3., 1., 0.)) / .9;

  gl_FragColor = o;
}
