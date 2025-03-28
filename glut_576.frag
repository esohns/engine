uniform vec2 iResolution;
uniform float iTime;

void
main ()
{
  vec2 uv = (2.0 * gl_FragCoord.xy - iResolution.xy) / min(iResolution.x, iResolution.y);

  for (float i = 2.0; i < 13.0; i++)
  {
    uv.x += 0.4 / i * cos(i * 2.0 * uv.y + iTime) * cos(i * 1.5 * uv.y + iTime);
    uv.y += 0.4 / i * cos(i * 2.0 * uv.x + iTime);
  }

  vec3 col = cos(iTime / 4.0 - uv.xyx);
  col = step(0.0, col);
  col.b = col.g;

  float alpha = 0.0;
  if (col.g > 0.0 || col.r > 0.0) alpha = 0.6;

  gl_FragColor = vec4(col, alpha);
}
