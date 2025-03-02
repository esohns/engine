#version 130

uniform vec2 iResolution;
uniform sampler2D iChannel0;

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  vec4 data = texture (iChannel0, uv);

  vec3 col = vec3 (0.0);
  if (data.w < 0.0)
    col = data.xyz;
  else
  {
    // decompress velocity vector
    float ss = mod (data.w, 256.0) / 255.0;
    float st = floor (data.w / 256.0) / 255.0;

    // motion blur (linear blur across velocity vectors
    vec2 dir = (-1.0 + 2.0 * vec2 (ss, st)) * 0.25;
    for (int i = 0; i < 32; i++)
    {
      float h = float (i) / 31.0;
      vec2 pos = uv + dir * h;
      col += texture (iChannel0, pos).xyz;
    }
    col /= 32.0;
  }

  // vignetting	
  col *= 0.5 + 0.5 * pow (16.0 * uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y), 0.1);

  gl_FragColor = vec4 (col, 1.0);
}
