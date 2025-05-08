uniform vec2 iResolution;
uniform float iTime;

float
v (vec2 uv, float d, float o)
{
  return 1.0 - smoothstep(0.0, d, distance(uv.x, 0.5 + sin(o + uv.y * 3.0) * 0.3));
}

vec4
b (vec2 uv, float o)
{
  float d = 0.05 + abs(sin(o * 0.2)) * 0.25 * distance(uv.y + 0.5, 0.0);
  return vec4(v(uv + vec2(d * 0.25, 0.0), d, o), 0.0, 0.0, 1.0) +
         vec4(0.0, v(uv - vec2(0.015, 0.005), d, o), 0.0, 1.0) +
         vec4(0.0, 0.0, v(uv - vec2(d * 0.5, 0.015), d, o), 1.0);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.y;
  gl_FragColor = b (uv, iTime) * 0.5 +
                 b (uv, iTime * 2.0) * 0.5 +
                 b (uv + vec2(0.3, 0.0), iTime * 3.3) * 0.5;
}
