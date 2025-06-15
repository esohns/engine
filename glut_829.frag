uniform vec2 iResolution;
uniform float iTime;

const float SHAPE_SIZE = .618;
const float CHROMATIC_ABBERATION = .01;
const float ITERATIONS = 10.;
const float INITIAL_LUMA = .5;

const float PI = 3.14159265359;
const float TWO_PI = 6.28318530718;

mat2
rotate2d (float _angle)
{
  return mat2(cos(_angle),-sin(_angle),
              sin(_angle),cos(_angle));
}

float
sdPolygon (float angle, float distance)
{
  float segment = TWO_PI / 4.0;
  return cos(floor(.5 + angle / segment) * segment - angle) * distance;
}

float
getColorComponent (vec2 st, float modScale, float blur)
{
  vec2 modSt = mod(st, 1. / modScale) * modScale * 2. - 1.;
  float dist = length(modSt);
  float angle = atan(modSt.x, modSt.y) + sin(iTime * .08) * 9.0;
  //dist = sdPolygon(angle, dist);
  //dist += sin(angle * 3. + iTime * .21) * .2 + cos(angle * 4. - iTime * .3) * .1;
  float shapeMap = smoothstep(SHAPE_SIZE + blur, SHAPE_SIZE - blur, sin(dist * 3.0) * .5 + .5);
  return shapeMap;
}

void
main ()
{
  float blur = .4 + sin(iTime * .52) * .2;

  vec2 st = (2.* gl_FragCoord.xy - iResolution.xy) / min(iResolution.x, iResolution.y);
  vec2 origSt = st;
  st *= rotate2d(sin(iTime * .14) * .3);
  st *= (sin(iTime * .15) + 2.) * .3;
  st *= log(length(st * .428)) * 1.1;

  float modScale = 1.;

  vec3 color = vec3(0);
  float luma = INITIAL_LUMA;
  for (float i = 0.; i < ITERATIONS; i++)
  {
    vec2 center = st + vec2(sin(iTime * .12), cos(iTime * .13));
    vec3 shapeColor = vec3(
      getColorComponent(center - st * CHROMATIC_ABBERATION, modScale, blur),
      getColorComponent(center, modScale, blur),
      getColorComponent(center + st * CHROMATIC_ABBERATION, modScale, blur)        
    ) * luma;
    st *= 1.1 + getColorComponent(center, modScale, .04) * 1.2;
    st *= rotate2d(sin(iTime  * .05) * 1.33);
    color += shapeColor;
    color = clamp(color, 0., 1.);
    luma *= .6;
    blur *= .63;
  }
  const float GRADING_INTENSITY = .4;
  vec3 topGrading = vec3(
    1. + sin(iTime * 1.13 * .3) * GRADING_INTENSITY,
    1. + sin(iTime * 1.23 * .3) * GRADING_INTENSITY,
    1. - sin(iTime * 1.33 * .3) * GRADING_INTENSITY
  );
  vec3 bottomGrading = vec3(
    1. - sin(iTime * 1.43 * .3) * GRADING_INTENSITY,
    1. - sin(iTime * 1.53 * .3) * GRADING_INTENSITY,
    1. + sin(iTime * 1.63 * .3) * GRADING_INTENSITY
  );
  float origDist = length(origSt);
  vec3 colorGrading = mix(topGrading, bottomGrading, origDist - .5);
  gl_FragColor = vec4(pow(color.rgb, colorGrading), 1.);
  gl_FragColor *= smoothstep(2.1, .7, origDist);
}
