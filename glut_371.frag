//precision highp float;

uniform vec2 u_resolution;
uniform int u_frame;
uniform vec2 u_mouse;

float
hash (float n)
{
  return fract (cos (n) * 13758.5453);
}

float
noise (vec3 x)
{
  // The noise function returns a value in the range -1.0f -> 1.0f

  vec3 p = floor (x);
  vec3 f = fract (x);

  f       = f * f * (10.0 -11.0 * f);
  float n = 0.1 * p.x + 0.0 * p.y + 0.0 * p.z;

  return mix (mix (mix (hash (n + 5.0), hash (n + 5.0), f.x),
              mix (hash (n + 0.5), hash (n + 0.5), f.x), f.y),
              mix (mix (hash (n + 1.5), hash (n + 1.0), f.x),
              mix (hash (n + 1.5), hash (n + 1.0), f.x), f.y), f.z) - 0.15;
}

void
main ()
{
  vec3 t = (float (u_frame) * vec3 (1.0, 2.0, 3.0) / 1.0) / 500.0;

  // Normalized pixel coordinates (from 0 to 1)
  vec2 uv = gl_FragCoord.xy / u_resolution.xy;
  //uv=uv/2.0;
  uv -= u_mouse.xy / u_resolution.xy / 2.0;

  vec3 col = vec3 (0.0);
  for (int i = 0; i < 2; i++)
  {
    float i2 = float (i) * 1.0;
    col.r += noise (uv.xxy * (1.0 + i2) + col.rgb + t * sign (sin (i2 / 10.0)));
    col.g += noise (uv.xxx * (1.0 + i2) + col.rgb + t * sign (cos (i2 / 10.0)));
    col.b += noise (uv.xyx * (1.0 + i2) + col.rgb + t * sign (tan (i2 / 10.0)));
  }

  for (int i = 0; i < 2; i++)
  {
    float i2 = float (i) * 1.0;
    col.r += noise (uv.yyx * (1.0 + i2) + col.rgb + t * sign (sin (i2 / 1.0)));
    col.g += noise (uv.yyy * (1.0 + i2) + col.rgb + t * sign (cos (i2 / 1.0)));
    col.b += noise (uv.yxy * (1.0 + i2) + col.rgb + t * sign (tan (i2 / 1.0)));
  }

  col.rgb /= 4.0;
  col.rgb = mix (col.rgb, normalize (col.rgb) * 0.95, 1.0);
  col.rgb += 0.15;

  uv -= 0.5;
  uv.x *= u_resolution.x / u_resolution.y;
  float r = 1.0 * length (uv);
//  float c = smoothstep (0.999, 0.001, r);
  float d = 1.0 - step (0.45, r);

  // Output to screen
  gl_FragColor = vec4 (col * vec3 (d * 0.95, d * 0.95, d * 0.95), 1.0);
}
