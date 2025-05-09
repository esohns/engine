#version 130

uniform vec2 iResolution;
//uniform float iTime;
uniform vec4 iDate;
uniform sampler2D iChannel0;

void
main ()
{
  vec4 f = vec4(.8, .2, .5, 1);
  vec2 g = gl_FragCoord.xy;

  vec2 s = iResolution.xy;
  vec4 h = texture(iChannel0, g / s);
  g = (g + g - s) / s.y * 1.3;
  vec2 k = vec2(1.6, 0.) + mod(iDate.w, 6.28),
       a = g - sin(k),
       b = g - sin(2.09 + k),
       c = g - sin(4.18 + k);
//  f = (0.02 / dot(a, a) + 0.02 / dot(b, b) + 0.02 / dot(c, c)) * 0.04 + h * 0.96 + step(h, f) * 0.01;
  f = (0.02 / dot(a, a) + 0.02 / dot(b, b) + 0.02 / dot(c, c)) * 0.1 + h * 0.9 + step(h, f) * 0.01;

  gl_FragColor = f;
}

/*void
main ()
{
  vec4 f;
  vec2 g = gl_FragCoord.xy;

  vec2 s = iResolution.xy;

  vec2 uv = (2. * g - s) / s.y;

  float t = iTime;

  vec2 d0 = vec2(cos(t), sin(t)) * 0.7;
  vec2 d1 = vec2(cos(t + 6.28318 / 3.), sin(t + 6.28318 / 3.)) * 0.7;
  vec2 d2 = vec2(cos(t + 6.28318 / 3. * 2.), sin(t + 6.28318 / 3. * 2.)) * 0.7;

  float m0 = 0.02 / dot(uv - d0, uv - d0);
  float m1 = 0.02 / dot(uv - d1, uv - d1);
  float m2 = 0.02 / dot(uv - d2, uv - d2);

  vec4 col = vec4(vec3(m0 + m1 + m2), 1);

  vec4 bufA = texture(iChannel0, g / s);

//  f = col * 0.04 + bufA * 0.96;
  f = col * 0.1 + bufA * 0.9;
  f += smoothstep(bufA, bufA + 0.01, vec4(.8, .2, .5, 1)) * 0.01;

  gl_FragColor = f;
}*/
