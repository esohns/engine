#version 130

uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec4 O;
  vec2 I = gl_FragCoord.xy;

  float t=iTime,
        i = 0.,
        z = 0.,
        d,
        s;
    
  for (O *= i; i++ < 1e2;)
  {
    vec3 p = z * normalize(vec3(I+I,0) - iResolution.xyy),
    v;
    p.z += 9.;
    v = p;
    for (d = 1.; d < 9.; d += d)
      p += .5 * sin(p.yzx * d + t) / d;
    z += d = .2 * (.01 + abs(s = dot(cos(p), sin(p / .7).yzx)) - min(d = 6. - length(v), -d * .1));
    O += (cos(s / .1 + z + t + vec4(2, 4, 5, 0)) + 1.2) / d / z;
  }
  O = tanh(O / 2e3);

  gl_FragColor = O;
}
