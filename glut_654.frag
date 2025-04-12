uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const bool autoRotate = true;

#define Vector2      vec2
#define Point2       vec2
#define Point3       vec3
#define Vector3      vec3
#define Color3       vec3
#define Radiance3    vec3
#define Irradiance3  vec3
#define Power3       vec3
#define Biradiance3  vec3

const float pi          = 3.1415926535;
const float degrees     = pi / 180.0;
const float inf         = 1.0 / 1e-10;

float square(float x) { return x * x; }
float pow3(float x) { return x * square(x); }
vec3 square(vec3 x) { return x * x; }
vec3 pow3(vec3 x) { return x * square(x); }
float pow4(float x) { return square(square(x)); }
float pow5(float x) { return x * square(square(x)); }
float pow8(float x) { return square(pow4(x)); }
float infIfNegative(float x) { return (x >= 0.0) ? x : inf; }

struct Ray { Point3 origin; Vector3 direction; };	
struct Material { Color3 color; float metal; float smoothness; };
struct Surfel { Point3 position; Vector3 normal; Material material; };
struct Sphere { Point3 center; float radius; Material material; };
   
bool intersectSphere (Point3 C, float r, Ray R, inout float nearDistance, inout float farDistance) { Point3 P = R.origin; Vector3 w = R.direction; Vector3 v = P - C; float b = 2.0 * dot(w, v); float c = dot(v, v) - square(r); float d = square(b) - 4.0 * c; if (d < 0.0) { return false; } float dsqrt = sqrt(d); float t0 = infIfNegative((-b - dsqrt) * 0.5); float t1 = infIfNegative((-b + dsqrt) * 0.5); nearDistance = min(t0, t1); farDistance  = max(t0, t1); return (nearDistance < inf); }

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }
float noise(float x) { float i = floor(x); float f = fract(x); float u = f * f * (3.0 - 2.0 * f); return mix(hash(i), hash(i + 1.0), u); }
float noise(vec2 x) { vec2 i = floor(x); vec2 f = fract(x); float a = hash(i); float b = hash(i + vec2(1.0, 0.0)); float c = hash(i + vec2(0.0, 1.0)); float d = hash(i + vec2(1.0, 1.0)); vec2 u = f * f * (3.0 - 2.0 * f); return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y; }
float noise(vec3 x) { const vec3 step = vec3(110, 241, 171); vec3 i = floor(x); vec3 f = fract(x); float n = dot(i, step); vec3 u = f * f * (3.0 - 2.0 * f); return mix(mix(mix( hash(n + dot(step, vec3(0, 0, 0))), hash(n + dot(step, vec3(1, 0, 0))), u.x), mix( hash(n + dot(step, vec3(0, 1, 0))), hash(n + dot(step, vec3(1, 1, 0))), u.x), u.y), mix(mix( hash(n + dot(step, vec3(0, 0, 1))), hash(n + dot(step, vec3(1, 0, 1))), u.x), mix( hash(n + dot(step, vec3(0, 1, 1))), hash(n + dot(step, vec3(1, 1, 1))), u.x), u.y), u.z); }

#define DEFINE_FBM(name, OCTAVES) float name(vec3 x) { float v = 0.0; float a = 0.5; vec3 shift = vec3(100); for (int i = 0; i < OCTAVES; ++i) { v += a * noise(x); x = x * 2.0 + shift; a *= 0.5; } return v; }
DEFINE_FBM(fbm3, 3)
DEFINE_FBM(fbm5, 5)
DEFINE_FBM(fbm6, 6)

#define elevationMap iChannel0
#define colorMap iChannel1

const float       verticalFieldOfView = 25.0 * degrees;

const Vector3     w_i             = Vector3(1.0, 1.0, -0.8) / 1.6248076;
const Biradiance3 B_i             = Biradiance3(4);

const Point3      planetCenter    = Point3(0);

const float       planetMaxRadius = 1.0;
const float       maxMountain = 0.13;
const float planetMinRadius = planetMaxRadius - maxMountain;

mat3 planetRotation;

Color3
samplePlanet (Point3 osPosition, out float e, out float k)
{
  Point3 s = normalize(osPosition);    

  Point2 cylCoord = vec2(atan(s.z, -s.x) / pi, s.y * 0.5 + 0.5);

  float sampleElevation  = length(osPosition);//dot(osPosition, s);

  k = (sampleElevation - planetMinRadius) * (1.0 / maxMountain);

  float lod = (iResolution.x > 800.0) ? 1.0 : 2.0;

  e = mix(textureLod(elevationMap, cylCoord, lod).r, textureLod(elevationMap, s.xz, lod).r, abs(s.y));
  e = square((e - 0.2) / 0.8) * 0.5;

  lod += k * 6.0;

  Color3 material = mix(textureLod(colorMap, cylCoord * vec2(2.0, 2.0), lod).rgb,
                        textureLod(colorMap, s.xz, lod).rgb, abs(s.y));

  material = pow3(material);

  float surfaceElevation = mix(planetMinRadius, planetMaxRadius, e);

  return material;
}

float
elevation (Point3 osPoint)
{
  float e, k;
  samplePlanet(osPoint, e, k);
  return e;
}

void
main ()
{
  float yaw   = -((iMouse.x / iResolution.x) * 2.5 - 1.25) + (autoRotate ? -iTime * 0.02 : 0.0);
  float pitch = ((iMouse.y > 0.0 ? iMouse.y : iResolution.y * 0.3) / iResolution.y) * 2.5 - 1.25;
  planetRotation = 
      mat3(cos(yaw), 0, -sin(yaw), 0, 1, 0, sin(yaw), 0, cos(yaw)) *
      mat3(1, 0, 0, 0, cos(pitch), sin(pitch), 0, -sin(pitch), cos(pitch));

  Vector2 invResolution = 1.0 / iResolution.xy;

  Radiance3 L_o;

  Surfel surfel;	

  Ray eyeRay = Ray(Point3(0.0, 0.0, 5.0), normalize(Vector3(gl_FragCoord.xy - iResolution.xy / 2.0, iResolution.y / (-2.0 * tan(verticalFieldOfView / 2.0)))));

  Point3 hitPoint;    
  float minDistanceToPlanet, maxDistanceToPlanet;

  bool hitBounds = intersectSphere(planetCenter, planetMaxRadius, eyeRay, minDistanceToPlanet, maxDistanceToPlanet);

  L_o = vec3(1) * max(0.0, hash(gl_FragCoord.xy * 0.5 + 10.0) - 0.999) / 0.0001 +
  pow5(fbm3(gl_FragCoord.xyy * -0.007 + iTime * 0.1)) * square(texture(iChannel1, gl_FragCoord.xy * invResolution.x).rbr) * max(0.0, hash(gl_FragCoord.xy * 0.1) - 0.99) / 0.0003;

  float gradCoord = (gl_FragCoord.x + gl_FragCoord.y) * (invResolution.x * 0.5);
  L_o += mix(Color3(0.025, 0, 0.02), Color3(0.11, 0.06, 0.0), gradCoord) * 1.5 * (0.15 + smoothstep(0.0, 1.0, 2.5 * abs(gradCoord - 0.4)));

  L_o += Color3(0.6, 0.06, 0.01) * (17.0 * pow8(max(0.0, 1.0 - length((gl_FragCoord.xy - iResolution.xy * 0.5) * invResolution.y))));

  Point2 sunCoord = vec2(-0.5, -0.5) + (gl_FragCoord.xy - iResolution.xy * 0.5) * invResolution.y;
  float sunDist = max(0.0,  1.0 - length(sunCoord));
  L_o += Color3(15, 9, 6) * (pow(sunDist, 12.0) * (1.0 + 0.25 * noise(sin(iTime * 0.1) + iTime * 0.1 + 20.0 * atan(sunCoord.x, -sunCoord.y))));

  if (hitBounds)
  {
    Color3 glow = Color3(0);

    const int NUM_STEPS = 250;

    float dt = (maxDistanceToPlanet - minDistanceToPlanet) / float(NUM_STEPS);
    Color3 material = Color3(0);
    Vector3 wsNormal = Vector3(0);
    Color3 p;
    float coverage = 0.0;
    float e = 1.0, k = 0.0;

    eyeRay.origin = (eyeRay.origin - planetCenter) * planetRotation;
    eyeRay.direction = eyeRay.direction * planetRotation;

    Point3 X;
    for (int i = 0; (i < NUM_STEPS) && (coverage < 1.0); ++i)
    {
      X = eyeRay.origin + eyeRay.direction * (dt * float(i) + minDistanceToPlanet);

      p = samplePlanet(X, e, k);
      if (e > k)
      {
        material = p;
        coverage = 1.0;

        glow += pow(p, p * 2.5 + 7.0) * 3e3;
      }
      else
        glow += pow(p, p + 7.0) * square(square(1.0 - k)) * 25.0;
    }

    Vector3 sphereNormal = normalize(planetRotation * X);

    const float eps = 0.01;
    wsNormal = planetRotation *
      normalize(Vector3(elevation(X + Vector3(eps, 0, 0)), 
                        elevation(X + Vector3(0, eps, 0)), 
                        elevation(X + Vector3(0, 0, eps))) - 
                        e);

    wsNormal = normalize(mix(wsNormal, sphereNormal, 0.95));

    L_o = mix(L_o, material * (max(dot(wsNormal, w_i) + 0.1, 0.0) * B_i + square(max(0.8 - sphereNormal.z, 0.0)) * Color3(2.0, 1.5, 0.5)), coverage);
    L_o += glow;
  }

  gl_FragColor.xyz = sqrt(L_o);
  gl_FragColor.a   = 1.0;
}
