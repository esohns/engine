uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

const float PI        = 3.14159265359;
const float TAU       = PI * 2.0;

#define AA 2.0

vec2
complexMul (vec2 A, vec2 B)
{
  return vec2((A.x * B.x) - (A.y * B.y), (A.x * B.y) + (A.y * B.x));
}

vec3
hsv2rgb (vec3 c)
{
  vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

struct POI
{
  vec2    center;
  float range;
  float maxIter;
};

float
mandelEscapeIters (vec2 C, float maxIter, vec2 ocOff, out float cycleLength1, out float cycleLength2)
{
  vec2 Z = C;
  vec2 orbitCenter1 = 0.3 * vec2(cos(iTime * 1.00), sin(iTime * 1.00));
  vec2 orbitCenter2 = orbitCenter1 / 0.3 * 0.2;
  orbitCenter1 += ocOff;
  orbitCenter2 += ocOff;
  cycleLength1 = 0.0;
  cycleLength2 = 0.0;
  for (float n = 0.0; n < maxIter; ++n)
  {
    Z    = complexMul(Z, Z) + C;
    if (cycleLength1 == 0.0 && abs(1.0 - length(Z - orbitCenter1)) < 0.015)
      cycleLength1 = n;
    if (cycleLength2 == 0.0 && abs(0.2 - length(Z - orbitCenter2)) < 0.01)
      cycleLength2 = n;
    if (dot(Z, Z) > 4.0)
      return n;
  }
  return maxIter;
}

void
main ()
{
  gl_FragColor.a  = 1.0;
  float smallWay = min(iResolution.x, iResolution.y);
  vec2 uv = (gl_FragCoord.xy * 2.0 - iResolution.xy)/smallWay;
  float t = iTime * TAU / 5.0;

  vec2 ocOff = vec2(0.0);
  if (any(greaterThan(iMouse.xy, vec2(50.0))))
    ocOff = (iMouse.xy * 2.0 - iResolution.xy)/smallWay;

  const POI poi1 = POI(vec2(-.7105, 0.2466), 0.04, 90.0);
  const POI poi2 = POI(vec2(-.600, 0.0000), 1.200, 70.0);
  const POI poi  = poi2;

  float rng = poi.range;

  float cycleLength1;
  float cycleLength2;

  vec3 col = vec3(0.0);

  for( float m = 0.0; m < AA; ++m)
    for( float n = 0.0; n < AA; ++n)
    {
      vec2  C   = (uv + vec2(m, n) / (AA * smallWay)) * rng + poi.center;
      float escapeIters = mandelEscapeIters(C, poi.maxIter, ocOff, cycleLength1, cycleLength2);
      float f   =  escapeIters / poi.maxIter;
      if (escapeIters == poi.maxIter)
        f = 0.0;
      f = pow(f, 0.6);
      f *= 0.82;

      vec3 rgb  = vec3(f * 0.2, f * 0.6, f * 1.0);
      if (cycleLength1 > 0.0)
      {
        const float spectrumSamples = 20.0;
        rgb += vec3(cos(cycleLength1 / spectrumSamples * TAU) * 0.2 + 0.3);
      }
      if (cycleLength2 > 0.0)
      {
        const float spectrumSamples = 30.0;
        rgb += hsv2rgb(vec3(fract(cycleLength2/ spectrumSamples), 0.9, 0.8));
      }

      col += rgb;
    }
  col /= (AA * AA);

  gl_FragColor.rgb = col;
}
