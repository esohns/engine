//precision highp float;

#define PI 3.141592653589793
#define TAU 6.283185307179586

uniform vec2 iResolution; // Canvas size (width, height)
uniform float iTime; // Time in seconds
uniform vec3 palette[5]; // Pass 5 colors, even if we don't use them all
uniform float skewFactor; // Skew factor (positive means slanted to the right)
uniform float rotationSpeed; // Speed (and direction) of rotation
uniform float wobblyness; // Speed of the second "wobbly"
uniform float subDivisions; // Number of sub divisions
uniform float spiralFactor; // Shrink factor for concentric circles. Close to 0 means no spiral
uniform float spiralSpeed; // Speed at which concentric circles shrink or grow (depends on sign)
uniform float xOff;
uniform float yOff; // X and Y offsets for center
uniform int numColors; // Number of colors to use from the palette
uniform bool blendColors; // Whether to use solid colors or blend them
uniform bool blendSame; // To use a single color for the blends, giving it a soft gradient effect

vec3
getColor (int index)
{
  if (index < 0) index += (10 * numColors);
  index = int (mod (float (index), float (numColors)));
  if (index == 0) return palette[0];
  else if (index == 1) return palette[1];
  else if (index == 2) return palette[2];
  else if (index == 3) return palette[3];
  else if (index == 4) return palette[4];
  return vec3 (0.0);  // Default case (shouldn't be needed)
}

void
main ()
{
  vec2 fragCoord = gl_FragCoord.xy;
  vec2 p = 2.0 * (0.5 * iResolution.xy - fragCoord.xy) / iResolution.xx;

  p.x += xOff;
  p.y -= yOff;
  p.y += skewFactor * p.x;

  // Move center slightly based on distance to center
  // float dist = length(p);
  // p.x += 5.0 * p.x * dist; //
  // p.y -= 4.1 * p.y * dist; // <--- Not at all what I expected, uncomment for some funky effects

  float angle = atan (p.y, p.x);
  float turn = (angle + PI) / TAU;


  float maxRadius = 2.0 * sqrt ((iResolution.x / 2.0) * (iResolution.x / 2.0) + (iResolution.y / 2.0) * (iResolution.y / 2.0));
  float radius = pow (2.0 * p.x * p.x + 2.0 * p.y * p.y, 0.5);

  float rotation = (rotationSpeed / 10.0) * TAU * iTime;
  float turn_1 = turn + rotation;

  float turn_sub = mod (float (subDivisions) * turn_1, float (subDivisions));
  float sine = wobblyness * (1.0 - sin (10.0 * (pow (radius, 0.4) - 0.4 * iTime)));
  float turn_sine = turn_sub + sine;

  int i_turn = int (mod (float (numColors) * turn_sine, float (numColors)));
  int i_radius = int (2.5 * spiralFactor / pow (radius * 0.5, 0.6) + spiralSpeed * iTime);
  int i_color = int (mod (float (i_turn + i_radius), float (numColors)));

  vec3 colorA = getColor (i_color);
  vec3 colorB = getColor (i_color + 1);
  if (blendSame)
    colorB = getColor (0);
  vec3 color = mix (colorA, colorB, fract (turn_sine + radius));
  if (!blendColors)
    color = colorA;

  color *= pow (radius, 0.35) * 0.9;
  // color *= 2.1 - radius;

  gl_FragColor = vec4 (color, 1.0);
}
