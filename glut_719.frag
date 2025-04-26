uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

//#define NON_REALTIME_HQ_RENDER
const float frameToRenderHQ = 11.0; // Time in seconds of frame to render
const float antialiasingSamples = 16.0; // 16x antialiasing - too much might make the shader compiler angry.

#define MANUAL_CAMERA

float localTime = 0.0;
float seed = 1.0;

float
v31 (vec3 a)
{
  return a.x + a.y * 37.0 + a.z * 521.0;
}

float
v21 (vec2 a)
{
  return a.x + a.y * 37.0;
}

float
Hash11 (float a)
{
  return fract(sin(a) * 10403.9);
}

float
Hash21 (vec2 uv)
{
  float f = uv.x + uv.y * 37.0;
  return fract(sin(f) * 104003.9);
}

vec2
Hash22 (vec2 uv)
{
  float f = uv.x + uv.y * 37.0;
  return fract(cos(f) * vec2(10003.579, 37049.7));
}

vec2
Hash12 (float f)
{
  return fract(cos(f) * vec2(10003.579, 37049.7));
}

float
Hash1d (float u)
{
  return fract(sin(u) * 143.9); // scale this down to kill the jitters
}

float
Hash2d (vec2 uv)
{
  float f = uv.x + uv.y * 37.0;
  return fract(sin(f) * 104003.9);
}

float
Hash3d (vec3 uv)
{
  float f = uv.x + uv.y * 37.0 + uv.z * 521.0;
  return fract(sin(f) * 110003.9);
}

const float PI = 3.14159265;

vec3
saturate (vec3 a)
{
  return clamp(a, 0.0, 1.0);
}

vec2
saturate (vec2 a)
{
  return clamp(a, 0.0, 1.0);
}

float
saturate (float a)
{
  return clamp(a, 0.0, 1.0);
}

float
smin (float a, float b, float k)
{
  float h = clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
  return mix(b, a, h) - k * h * (1.0 - h);
}

vec2
matMin (vec2 a, vec2 b)
{
  return (a.x < b.x) ? a : b;
}

float
sdBox (vec3 p, vec3 radius)
{
  vec3 dist = abs(p) - radius;
  return min(max(dist.x, max(dist.y, dist.z)), 0.0) + length(max(dist, 0.0));
}

float
cyl (vec2 p, float r)
{
  return length(p) - r;
}

const float TAU = 2.0 * PI;

float glow = 0.0;

vec2
DistanceToObject (vec3 p)
{
  float time = localTime * 2.0;
  float cylRadBig = 1.0;
  float cylRadSmall = 0.05;
  float freq = 4.0;
  float braidRad = 0.15;
  float angle = atan(p.z, p.x);
  float cylDist = length(p.xz) - cylRadBig;
  vec3 cylWarp = vec3(cylDist, p.y, angle);
  float amp = sin(angle + time) * 0.5 + 0.5;

  float theta = angle * freq;
  vec2 wave1 = vec2(sin(theta), cos(theta)) * braidRad;
  wave1 *= amp;
  float d = sdBox(vec3(cylWarp.xy + wave1, 0.0), vec3(cylRadSmall));
  float final = d;

  theta = angle * freq + TAU / 3.0;
  vec2 wave2 = vec2(sin(theta), cos(theta)) * braidRad;
  wave2 *= amp;
  d = sdBox(vec3(cylWarp.xy + wave2, 0.0), vec3(cylRadSmall));
  final = smin(final, d, 0.1);

  theta = angle * freq - TAU / 3.0;
  vec2 wave3 = vec2(sin(theta), cos(theta)) * braidRad;
  wave3 *= amp;
  d = sdBox(vec3(cylWarp.xy + wave3, 0.0), vec3(cylRadSmall));
  final = smin(final, d, 0.1);

  vec2 matd = vec2(final, fract((angle + time) / TAU + 0.5));
  float sliver = cyl(cylWarp.xy, 0.03);
  glow += 1.0 / (sliver * sliver * sliver * sliver + 1.0);
  matd = matMin(matd, vec2(sliver, -1.0));

  return matd;
}

vec3
RayTrace (vec2 fragCoord)
{
  glow = 0.0;

  vec3 camPos, camUp, camLookat;

  vec2 uv = fragCoord.xy / iResolution.xy * 2.0 - 1.0;
  float zoom = 2.2;
  uv /= zoom;

  camUp = vec3(0, 1, 0);
  camLookat = vec3(0);

  float mx = iMouse.x / iResolution.x * PI * 2.0;
  float my = -iMouse.y / iResolution.y * 10.0;
#ifndef MANUAL_CAMERA
  camPos = vec3(0.0);
  camPos.y = sin(localTime*0.125)*3.0;
  camPos.z = cos(localTime*0.125)*3.0;
  camUp.y = camPos.z;
  camUp.z = -camPos.y;
  camUp = normalize(camUp);
#else
  camPos = vec3(cos(my) * cos(mx), sin(my), cos(my) * sin(mx)) * 3.0;
#endif

  vec3 camVec = normalize(camLookat - camPos);
  vec3 sideNorm = normalize(cross(camUp, camVec));
  vec3 upNorm = cross(camVec, sideNorm);
  vec3 worldFacing = (camPos + camVec);
  vec3 worldPix = worldFacing + uv.x * sideNorm * (iResolution.x / iResolution.y) + uv.y * upNorm;
  vec3 rayVec = normalize(worldPix - camPos);

  vec2 distMat = vec2(1.0, 0.0);
  float t = 0.0 + Hash2d(uv) * 1.6;
  const float maxDepth = 6.0;
  vec3 pos = vec3(0, 0, 0);
  const float smallVal = 0.000625;
  float marchCount = 0.0;
  for (int i = 0; i < 80; i++)
  {
    pos = camPos + rayVec * t;

    distMat = DistanceToObject(pos);

    t += distMat.x * 0.8;
    if ((t > maxDepth) || (abs(distMat.x) < smallVal))
      break;

    float cyc = (-sin(distMat.y * TAU)) * 0.5 + 0.7;
    marchCount += cyc / (distMat.x * distMat.x + 1.0);
  }

  float glowSave = glow;
  float marchSave = marchCount;
  marchCount = 0.0;
  glow = 0.0;

  vec3 finalColor = vec3(0.09, 0.15, 0.35);

  if (t <= maxDepth)
  {
    vec3 smallVec = vec3(smallVal, 0, 0);
    vec3 normalU = vec3(distMat.x - DistanceToObject(pos - smallVec.xyy).x,
                        distMat.x - DistanceToObject(pos - smallVec.yxy).x,
                        distMat.x - DistanceToObject(pos - smallVec.yyx).x);

    vec3 texColor = vec3(0.0, 0.0, 0.1);
    if (distMat.y < 0.0)
      texColor = vec3(0.6, 0.3, 0.1) * 110.0;

    finalColor = texColor;
  }
  finalColor += vec3(0.3, 0.5, 0.9) * glowSave * 0.00625;
  finalColor += vec3(1.0, 0.5, 0.3) * marchSave * 0.05;

  finalColor *= vec3(1.0) * saturate(1.0 - length(uv / 2.5));

  return vec3(saturate(finalColor));
}

#ifdef NON_REALTIME_HQ_RENDER
const float blockRate = 20.0;
void
BlockRender (vec2 fragCoord)
{
  const float blockSize = 64.0;
  float frame = floor(iTime * blockRate);
  vec2 blockRes = floor(iResolution.xy / blockSize) + vec2(1.0);
  float blockX = fract(frame / blockRes.x) * blockRes.x;
  float blockY = fract(floor(frame / blockRes.x) / blockRes.y) * blockRes.y;
  if ((fragCoord.x - blockX * blockSize >= blockSize) ||
      (fragCoord.x - (blockX - 1.0) * blockSize < blockSize) ||
      (fragCoord.y - blockY * blockSize >= blockSize) ||
      (fragCoord.y - (blockY - 1.0) * blockSize < blockSize))
    discard;
}
#endif

void
main ()
{
  vec2 fragCoord = gl_FragCoord.xy;

#ifdef NON_REALTIME_HQ_RENDER
  BlockRender(fragCoord);
#endif

  vec3 finalColor = vec3(0.0);
#ifdef NON_REALTIME_HQ_RENDER
  for (float i = 0.0; i < antialiasingSamples; i++)
  {
    const float motionBlurLengthInSeconds = 1.0 / 60.0;
    localTime = frameToRenderHQ;
    localTime += Hash11(v21(fragCoord + seed)) * motionBlurLengthInSeconds;
    vec2 jittered = fragCoord.xy + vec2(
      Hash21(fragCoord + seed),
      Hash21(fragCoord*7.234567 + seed)
    );
    if (antialiasingSamples == 1.0) jittered = fragCoord;
    finalColor += RayTrace(jittered);
    seed *= 1.01234567;
  }
  finalColor /= antialiasingSamples;
#else
  localTime = iTime;
  finalColor = RayTrace(fragCoord);
#endif

  gl_FragColor = vec4(sqrt(clamp(finalColor, 0.0, 1.0)), 1.0);
}
