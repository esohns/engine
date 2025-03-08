#version 130

uniform vec2 iResolution;
uniform float iTime;

const int	nbBall = 8;
const float ballRadius = 0.07;
const float maxLateralSpeed = 1.;
const float Tau = radians(360.);
const float minPhaseSpeed = 2.1;
const float maxPhaseSpeed = 3.2;
const float sinAmplitude = 1.2;
const vec3	drawColor = vec3(0.2,0.3,0.4);

float hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

float sdCircle (vec2 p, float r)
{
  return length(p) - r;
}

float udSegment (vec2 p, vec2 a, vec2 b)
{
  vec2 ba = b-a;
  vec2 pa = p-a;
  float h =clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length(pa-h*ba);
}

float triangleSignal (float x, float f)
{
  f = 1. / f;
  return (abs((f * x - 4. * floor(0.25 * f * x)) - 2.) - 1.) / f;
}

float smoothFilter (float d)
{
  float v = 2. / iResolution.y;
  return smoothstep(v, -v, d);
}

void main ()
{
  vec2	 p = (2. * gl_FragCoord.xy - iResolution.xy) / iResolution.y;
  float ratio = iResolution.x / iResolution.y;
  vec3	 col = vec3(0);

  vec2	 ballArray[nbBall + 1];
  for (int i = 0; i < nbBall; ++i)
  {
    float LateralSpeed = (hash11(float(i) * 1.2544) - 0.5) * 2. * maxLateralSpeed;
    float phaseOffset = hash11(float(i) * 1.4482) * Tau;
    float LateralOffset = hash11(float(i) * 1.9528) * (ratio * 2. - 2. * ballRadius);
    float phaseSpeed = mix(minPhaseSpeed, maxPhaseSpeed, hash11(float(i) * 1.1273));
    ballArray[i] = vec2(	triangleSignal(LateralOffset + LateralSpeed * iTime, ratio - ballRadius),
                        sinAmplitude * (abs(sin(phaseOffset + phaseSpeed * iTime)) - 0.5)
                       );
    float a = smoothFilter(sdCircle( ballArray[i] - p , ballRadius));
    col = mix(col, drawColor, a);
  }

  for (int i = 0; i < nbBall - 1; ++i)
    for (int j = 0; j < nbBall - 1 - i; ++j)
      if (ballArray[j].x > ballArray[j + 1].x)
      {
        vec2 tmp = ballArray[j + 1];
        ballArray[j + 1] = ballArray[j];
        ballArray[j] = tmp;
      }

  ballArray[nbBall] = vec2(ratio, 0);
  int   convexSize = 0;
  vec2  convexHull[nbBall + 2];
  convexHull[convexSize++] = vec2(-ratio, 0);
  int   SmallestIndex = 0;
  // *BUG*: this never terminates on AMD Radeon (on Windows 11)
  for (;;)
  {
    float SmallestDir = 1.;
    bool found = false;
    for(int i = SmallestIndex; i < nbBall + 1; ++i)
    {
      vec2 dir = normalize(ballArray[i] - convexHull[convexSize - 1]);
      if(dir.y < SmallestDir)
      {
        SmallestDir = dir.y;
        SmallestIndex = i;
        found = true;
      }
    }
    if (found)
    {
      convexHull[convexSize++] = ballArray[SmallestIndex];
      continue;
    }

    break;
  }

  for (int i = 0; i < convexSize - 1; ++i)
  {
    float a = smoothFilter(udSegment(p, convexHull[i] - vec2(0, ballRadius), convexHull[i + 1] - vec2(0, ballRadius)) - 0.005);
    col = mix(col, drawColor, a);
  }

  gl_FragColor = vec4(col,1.0);
}
