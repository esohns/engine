//precision mediump float;

uniform vec2 dim;
#define BALL_COUNT 75
uniform vec2 balls[BALL_COUNT];
uniform float influences[BALL_COUNT];

varying vec2 vPos;

const float Threshold = 0.2;
const float Multiplicator = 1.0 / 17.0;
const mat4 DitherMatrix = (mat4 (
    1, 13, 4, 16,
    9, 5, 12, 8,
    3, 15, 2, 14,
    11, 7, 10, 6
    ) - 8.) * Multiplicator;

float
GetLuminance (vec4 c)
{
  return (0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b);
}

float
AdjustDither (float val, vec2 coord)
{
  vec2 coordMod = mod (coord, 4.0);
  int xMod = int (coordMod.x);
  int yMod = int (coordMod.y);

  vec4 col;
  if (xMod == 0) col = DitherMatrix[0];
  else if (xMod == 1) col = DitherMatrix[1];
  else if (xMod == 2) col = DitherMatrix[2];
  else if (xMod == 3) col = DitherMatrix[3];

  float adjustment = 0.;
  if (yMod == 0) adjustment = col.x;
  else if (yMod == 1) adjustment = col.y;
  else if (yMod == 2) adjustment = col.z;
  else if (yMod == 3) adjustment = col.w;

  return val + (val * adjustment);
}

void
main ()
{
  float resolution = 2.0;
  vec2 coord = ((vPos + 1.0) / 2.0) * dim;
  vec2 uv = (floor (gl_FragCoord.xy / 2.0) * 2.0) / resolution;

  float amplitude = 0.0;
  float yNorm = (vPos.y + 1.0) / 2.0;
  // yNorm = pow(yNorm, 3.0);
  // Ease in out cubic the value
  yNorm = yNorm < 0.5 ? 4.0 * pow (yNorm, 3.0) : 1.0 - pow (-2.0 * yNorm + 2.0, 3.0) / 2.0;

  for (int i = 0; i < BALL_COUNT; i++)
    amplitude += exp (-1.0 / influences[i] * (pow (coord.x - balls[i].x,2.0) + pow (coord.y - balls[i].y, 2.0)));

  amplitude = mod (1.8 * 255.0 * amplitude, 3500.0) / 100.0;
  vec3 color = mix (vec3 (1.0, 0.0, -0.1), vec3 (-0.1, 0.0, 1.0), yNorm);
  color *= amplitude;

  float luminance = GetLuminance (vec4 (color, 1.0));
  // if (AdjustDither (luminance, gl_FragCoord.xy) > Threshold)
  if (AdjustDither (luminance, uv * resolution) > Threshold)
    gl_FragColor = vec4 (color, 1.0);
  else
    gl_FragColor = vec4 (0.14, 0, 0.03, 1.0); // Or any other color for the 'dark' part

  // gl_FragColor = vec4(color, 1.0);
}
