uniform vec2 iResolution;
uniform float iTime;

#define S smoothstep
#define N 5

vec4
Line (vec2 uv, float speed, float height, vec3 col)
{
  uv.y += S (1.0, 0.0, abs (uv.x)) * sin (iTime * speed + uv.x * height) * 0.2;
  return vec4 (S (0.06 * S (0.2, 0.9, abs (uv.x)), 0.0, abs (uv.y) - 0.004) * col, 1.0) * S (1.0, 0.3, abs (uv.x));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - 0.5 * iResolution.xy) / iResolution.y;
  gl_FragColor = vec4 (0.0);
  for (int i = 0; i <= N; i++)
  {
    float t = float (i) / float (N);
    gl_FragColor += Line(uv, 1.0 + t, 4.0 + t, vec3 (0.2 + t * 0.7, 0.2 + t * 0.4, 0.3));
  }
}
