//precision mediump float;

uniform vec2 uResolution;
uniform vec2 uPhase;
uniform float uScale;
uniform int uOctaves;
uniform float uFreq;
uniform float uNoiseFactor;
uniform float uSharpness;
uniform float uTime;
uniform int uPattern;

vec3
hash (vec3 p)
{
  p = vec3 (dot (p, vec3 (127.1, 311.7, 74.7)),
            dot (p, vec3 (269.5, 183.3, 246.1)),
            dot (p, vec3 (113.5, 271.9, 124.6)));
  return -1.0 + 2.0 * fract (sin (p) * 43758.5453123);
}

float
noiseVec3 (vec3 p) 
{
  vec3 i = floor (p);
  vec3 f = fract (p);
  vec3 u = f * f * (3.0 - 2.0 * f);

  return mix (mix (mix (dot (hash (i + vec3 (0.0,0.0,0.0)), f - vec3 (0.0,0.0,0.0)),
                        dot (hash (i + vec3 (1.0,0.0,0.0)), f - vec3 (1.0,0.0,0.0)), u.x),
                   mix (dot (hash (i + vec3 (0.0,1.0,0.0)), f - vec3 (0.0,1.0,0.0)),
                        dot (hash (i + vec3 (1.0,1.0,0.0)), f - vec3 (1.0,1.0,0.0)), u.x), u.y),
              mix (mix (dot (hash (i + vec3 (0.0,0.0,1.0)), f - vec3 (0.0,0.0,1.0)),
                        dot (hash (i + vec3 (1.0,0.0,1.0)), f - vec3 (1.0,0.0,1.0)), u.x),
                   mix (dot (hash (i + vec3 (0.0,1.0,1.0)), f - vec3 (0.0,1.0,1.0)),
                        dot (hash (i + vec3 (1.0,1.0,1.0)), f - vec3 (1.0,1.0,1.0) ), u.x), u.y), u.z );
}

float
octaveNoise (vec3 st, int octaves)
{
  float total = 1.0;
  float sum = noiseVec3 (st);
  if (octaves > 1) {sum += 0.5      * noiseVec3 (st * 2.0); total = 1.5; }
  if (octaves > 2) {sum += 0.25     * noiseVec3 (st * 4.0); total = 1.75; }
  if (octaves > 3) {sum += 0.125    * noiseVec3 (st * 8.0); total = 1.875; }
  if (octaves > 4) {sum += 0.0625   * noiseVec3 (st * 16.0); total = 1.9375; }
  if (octaves > 5) {sum += 0.078125 * noiseVec3 (st * 32.0); total = 1.953125; }
  return sum / total;
}

float
qsmoothstep (float x)
{ // quintic smoothstep
  return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
}

void
main ()
{
  vec2 p = (gl_FragCoord.xy * 2.0 - uResolution) / uResolution.y;
  p = p * uScale + uPhase;
  float displacement = octaveNoise (vec3 (p, uTime), uOctaves) * uNoiseFactor;
  float pat = uFreq * length (p - 0.5 * floor (p /0.5 + 0.5));
  if (uPattern == 1)
  {
    vec2 q = p - 0.5 * floor (p /0.5 + 0.5);
    pat = uFreq * (abs (q.x) - abs (q.y));
  }
  else if (uPattern == 2)
    pat = sin (uFreq * p.x) * sin (uFreq * p.y);
  else if (uPattern == 3)
  {
    float t = cos (uFreq * p.x) * cos (uFreq * p.y);
    pat = smoothstep (0.1, 0.0, t * t);
  }
  float gray = cos ((pat + displacement) * 3.1416) * 0.5 + 0.5;
  vec3 color = vec3 (pow (gray, uSharpness));
  gl_FragColor = vec4 (color, 1.0);
}
