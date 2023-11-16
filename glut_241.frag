#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;
//uniform vec2 u_mouse;
uniform float u_time;

varying vec2 vTexCoord;

float TAU = 2.0*3.14159256;

vec3 palette (float t)
{
  vec3 a = vec3(0.198, 0.438, 0.698);
  vec3 b = vec3(-0.262, 0.208, 0.238);
  vec3 c = vec3(2.238, 2.168, 1.000);
  vec3 d = vec3(-0.052, 0.333, 0.667);

  return a + b * cos (6.28318 * (c * t + d));
}

float sdPentagon (in vec2 p, in float r)
{
  const vec3 k = vec3 (0.809016994,0.587785252,0.726542528);
  p.x = abs (p.x);
  p -= 2.0 * min (dot (vec2 (-k.x, k.y), p), 0.0) * vec2 (-k.x, k.y);
  p -= 2.0 * min (dot (vec2 ( k.x, k.y), p), 0.0) * vec2 ( k.x,k.y);
  p -= vec2 (clamp (p.x, -r * k.z, r * k.z), r);
  return length (p) * sign (p.y);
}

void main()
{
  // copy the vTexCoord
  // vTexCoord is a value that goes from 0.0 - 1.0 depending on the pixels location
  // we can use it to access every pixel on the screen

  vec2 coord = vTexCoord;

  float u = coord.x * 2.0 - 1.0;
  float v = coord.y * 2.0 - 1.0;
  const float scale = 2.0;

  // Make sure pixels are square
  u = u * scale * u_resolution.x / u_resolution.y;
  v = v * scale;

  vec2 uv = vec2 (u, v);

  vec2 uv0 = uv;

  vec3 finalColor = vec3 (0.0);
  for (float i = 0.0; i < 4.0; i++)
  {
    uv = fract (uv * 1.5) - 0.5;

    float d = sdPentagon (uv, 3.0) * exp (-length (uv0));
    //length(uv) * exp(-length(uv0));

    vec3 col = palette (length (uv0) + i * 0.4 + u_time * 0.4);

    d = sin (d * 10.0 - u_time) / 8.0;
    d = abs (d);

    d = pow (0.01 / d, 1.2);

    finalColor += col * d;
  }

  // gl_FragColor is a built in shader variable, and your .frag file must contain it
  // We are setting the vec3 color into a new vec4, with a transparency of 1 (no opacity)
  gl_FragColor = vec4 (finalColor, 1.0);
}
