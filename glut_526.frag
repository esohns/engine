uniform vec2 iResolution;
uniform float iTime;

float
plot (float r, float pct)
{
  return  smoothstep (pct - 0.2, pct, r) - smoothstep (pct, pct + 0.2, r);
}

vec3
pal (float t, vec3 a, vec3 b, vec3 c, vec3 d)
{
  return a + b * cos (6.28318 * (c * t + d));
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec3 col = vec3 (1.0);
  vec2 pos = vec2 (0.5) - uv;
  pos.x *= iResolution.x / iResolution.y;
  pos *= cos (iTime) * 1.0 + 1.5;

  float r = length (pos) * 2.0;
  float a = atan (pos.y, pos.x);

  float f = abs (cos (a * 2.5 + iTime * 0.5)) * sin (iTime * 2.0) * 0.698 + cos (iTime) - 4.0;
  float d = f - r;

  col = (vec3 (smoothstep (fract (d), fract (d) - 0.2, 0.16)) - vec3 (smoothstep (fract (d), fract (d) - 1.184, 0.16))) *
         pal (f, vec3 (0.725, 0.475, 0.440), vec3 (0.605, 0.587, 0.007), vec3 (1.0, 1.0, 1.0), vec3 (0.310, 0.410, 0.154));
  float pct = plot (r * 0.272, fract (d * (sin (iTime) * 0.45 + 0.5)));

  col += pct * pal (r, vec3 (0.750, 0.360, 0.352), vec3 (0.450, 0.372, 0.271), vec3 (0.540, 0.442, 0.264), vec3 (0.038, 0.350, 0.107));

  gl_FragColor = vec4 (col, pct * 0.3);
}
