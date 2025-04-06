uniform vec2 iResolution;
uniform float iTime;

#define INNER_RADIUS 0.75
#define OUTER_RADIUS 0.9
#define SHEET_THICKNESS 0.012
#define NOISINESS 10.0

#define INNER_COLOR vec4(0.0, 30.0, 30.0, 1.0)
#define OUTER_COLOR vec4(20.0, 20.0, 30.0, 1.0)

#define NUM_STEPS 20
#define TIME_SCALE 5.0

float
trapezium (float x)
{
	return min(1.0, max(0.0, 1.0 - abs(-mod(x, 1.0) * 3.0 + 1.0)) * 2.0);
}

vec3
colFromHue (float hue)
{
	return vec3(trapezium(hue - 1.0/3.0), trapezium(hue), trapezium(hue + 1.0/3.0));
}

float
cnoise3 (float pos)
{
	return (cos(pos / 2.0) * 0.2 + 1.0);
}

float
cnoise2 (float pos)
{
	return (sin(pos * cnoise3(pos) / 2.0) * 0.2 + 1.0);
}

float
cnoise (vec4 pos)
{
  float x = pos.x * cnoise2(pos.y) + pos.w * 0.87123 + 82.52;
  float y = pos.y * cnoise2(pos.z) + pos.w * 0.78725 + 12.76;
  float z = pos.z * cnoise2(pos.x) + pos.w * 0.68201 + 42.03;
  return (sin(x) + sin(y) + sin(z)) / 3.0;
}

vec4
merge_colours (vec4 apply_this, vec4 on_top_of_this)
{
  return on_top_of_this * (1.0 - apply_this.a) + apply_this * apply_this.a;
}

vec4
getdensity (vec3 pos)
{
  float time = iTime * TIME_SCALE;

  vec3 samplePos = normalize(pos);

  vec4 inner_color = vec4(colFromHue(cnoise(vec4(samplePos / 5.0, time / 15.0))) * 25.0, 1.0);
  vec4 outer_color = merge_colours(vec4(25.0,25.0,25.0,0.5), inner_color);

  float sample_ = (cnoise(vec4(samplePos * NOISINESS, time)) + 1.0) / 2.0;

  sample_ = clamp(sample_, 0.0, 1.0);

  float innerIncBorder = INNER_RADIUS + SHEET_THICKNESS;
  float outerIncBorder = OUTER_RADIUS - SHEET_THICKNESS;

  float radius = innerIncBorder + (outerIncBorder - innerIncBorder) * sample_;

  float dist = distance(pos, vec3(0.0, 0.0, 0.0));

  float density = exp(-pow(dist - radius, 2.0) * 05000.0);

  return (inner_color + (outer_color - inner_color) * (radius - innerIncBorder) / (outerIncBorder - innerIncBorder)) * density;
}

vec4
raymarch (vec3 start, vec3 end)
{
  vec4 retn = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 delta = end - start;
  float stepDistance = length(delta) / float(NUM_STEPS);

  vec4 densityPrevious = getdensity(start);
  for (int i = 1; i < NUM_STEPS; i++) 
  {
    vec3 samplePos = start + delta * float(i) / float(NUM_STEPS);
    vec4 density = getdensity(samplePos);
    vec4 densityIntegrated = (density + densityPrevious) / 2.0;
    retn += densityIntegrated;

    densityPrevious = density;
  }

  return retn * stepDistance;
}

vec4
raymarch_ball (vec2 coord)
{
  float d = distance(coord, vec2(0.0, 0.0));
  if (d > OUTER_RADIUS)
  	return vec4(0.0, 0.0, 0.0, 0.0);
  float dOuterNormalized = d / OUTER_RADIUS;
  float outerStartZ = -sqrt(1.0 - dOuterNormalized*dOuterNormalized) * OUTER_RADIUS;
  float outerEndZ = -outerStartZ;
  if (d > INNER_RADIUS)
  {
    vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, 0));
    vec4 backPart = raymarch(vec3(coord, 0), vec3(coord, outerEndZ));
    return frontPart + backPart;
  }

  float dInnerNormalized = d / INNER_RADIUS;
  float innerStartZ = -sqrt(1.0 - dInnerNormalized*dInnerNormalized) * INNER_RADIUS;
  float innerEndZ = -innerStartZ;

  vec4 frontPart = raymarch(vec3(coord, outerStartZ), vec3(coord, innerStartZ));
  vec4 backPart = raymarch(vec3(coord, innerEndZ), vec3(coord, outerEndZ));
  vec4 final = frontPart + backPart;
  return final;
}

void
main ()
{
	vec2 uv = (gl_FragCoord.xy / min(iResolution.x, iResolution.y)) * 2.0 - vec2(iResolution.x / iResolution.y, 1.0);
  gl_FragColor = merge_colours(raymarch_ball(uv), vec4(0.0, 0.0, 0.0, 1.0));
}
