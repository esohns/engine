#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform float iFrameRate;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define SCREW 12.
#define SPEED 1.
#define FOV 3.
#define MOTION_BLUR 1
#define AA 2.
#define MAX_STEPS 80
#define SHADOW_STEPS 200
#define SHADOW_SOFTNESS 20.
#define EPS .0001
#define RENDER_DIST 20.
#define AO_SAMPLES 5.
#define AO_RANGE 10.
#define LIGHT_COLOR vec3(1.,.8,.6)

#define PI 3.14159265359
#define SQRT1_2 0.70710678118
#define saturate(x) clamp(x, 0., 1.)

float time;

float
hash (vec3 uv)
{
  float f = fract(sin(dot(uv, vec3(.09123898, .0231233, .0532234))) * 1e5);
  return f;
}

void
tRotate (inout vec2 p, float angle)
{
  vec2 s = sin(vec2(angle, angle + PI * .5));
	p *= mat2(s.y, -s.x, s);
}

void
tFan (inout vec2 p, float s)
{
  float k = s / PI / 2.;
  tRotate(p, -floor((atan(p.y, p.x)) * k + .5) / k);
}

float
opU (float a, float b)
{
  return min(a, b);
}

float
opI (float a, float b)
{
  return max(a, b);
}

float
opS (float a, float b)
{
  return max(a, -b);
}

float
bolt (vec3 p)
{
  float radius = length(p.xz) - .25;
    
  float thread = (radius - abs(fract(p.y * SCREW - atan(p.z, p.x) / PI / 2.) - .5) / SCREW) * SQRT1_2;
    
  float screw = opS(thread, .5 - abs(p.y + .5));
  float cone = (p.y - radius) * SQRT1_2;
    
  screw = opS(screw, cone + 1. * SQRT1_2);
    
  float hole = opS(thread, cone + .5 * SQRT1_2);
  hole = opU(hole, -cone - .05 * SQRT1_2);
    
  tFan(p.xz, 6.);
  float head = p.x - .5;
    
  head = opI(opI(head, abs(p.y + .25) - .25), (p.y + radius - .22) * SQRT1_2);
  return opS(opU(screw,head), hole);
}

void
tNextBolt (inout vec3 p)
{
  float t = mod(time, 5.);
  const float limit = 1. / SCREW;
  if (t < 1.)
    p.y -= smoothstep(1., 0., t) * 2. + 1. - limit;
  else if (t < 1.5)
    p.y -= 1. - limit;
  else if (t < 4.)
  {
    float progress = smoothstep(4., 1.5, t);
    p.y -= .5 + progress * (.5 - limit);
    tRotate(p.xz, -progress * (SCREW / 2. - 1.) * 2. * PI);
  }
  else if (t < 4.5)
    p.y -= .5;
  else
    p.y += .5 * smoothstep(4.5, 5., t) -.5;
}

float
nextBolt (vec3 p)
{
  tNextBolt(p);
  return bolt(p);
}

void
tPrevBolt (inout vec3 p)
{
  float t = mod(time, 5.);
  p.y += .5 * smoothstep(4.5, 5., t);
}

float
prevBolt (vec3 p)
{
  tPrevBolt(p);
  return bolt(p);
}

float
map (vec3 p)
{
  p.y += .25;
	return opU(nextBolt(p), prevBolt(p));
}

float
trace (vec3 ro, vec3 rd, float maxDist, out float steps)
{
  float total = 0.;
  steps = 0.;

  for (int i = 0; i < MAX_STEPS; ++i)
  {
    ++steps;
    float d = map(ro + rd * total);
    total += d;
    if (d < EPS || maxDist < total)
      break;
  }

  return total;
}

float
softShadow (vec3 ro, vec3 rd, float maxDist)
{
  float total = 0.;
  float s = 1.;

  for (int i = 0; i < SHADOW_STEPS; ++i)
  {
    float d = map(ro + rd * total);
    if (d < EPS)
    {
      s = 0.;
      break;
    }
    if (maxDist < total)
      break;
    s = min(s, SHADOW_SOFTNESS * d / total);
    total += d;
  }

  return s;
}

vec3
getNormal (vec3 p)
{
  vec2 e = vec2(.005, 0);
  return normalize(vec3(
    map(p + e.xyy) - map(p - e.xyy),
    map(p + e.yxy) - map(p - e.yxy),
    map(p + e.yyx) - map(p - e.yyx)
	));
}

float
calculateAO (vec3 p, vec3 n)
{
  float r = 0., w = 1., d;

  for (float i = 1.; i <= AO_SAMPLES; i++)
  {
    d = i / AO_SAMPLES / AO_RANGE;
    r += w * (d - map(p + n * d));
    w *= .5;
  }

  return 1.-saturate(r * AO_RANGE);
}

vec3
_texture (vec3 p)
{
  p.y += .25;
  if (prevBolt(p) <= nextBolt(p)) {
      tPrevBolt(p);
  } else {
      tNextBolt(p);
  }
  p *= 1.;
    
  return pow(texture(iChannel0, p.xy * 2. + p.zz).rgb, vec3(2.2));
}

float
bumpTexture (vec3 p)
{
  return 1. - _texture(p).g;
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  vec2 e = vec2(.0001, 0);
  float ref = bumpTexture(p);                 
  vec3 grad = vec3(bumpTexture(p - e.xyy) - ref,
                   bumpTexture(p - e.yxy) - ref,
                   bumpTexture(p - e.yyx) - ref) / e.x;

  grad -= nor * dot(nor, grad);          

  return normalize(nor + grad * bumpfactor);
}

vec4
frame (vec2 fragCoord)
{
  vec4 fragColor = vec4(0);

  time = iTime * SPEED;

#if MOTION_BLUR
  time = time + hash(vec3(fragCoord, time)) / iFrameRate * SPEED;
#endif

	vec2 uv = fragCoord.xy / iResolution.xy * 2. - 1.;
  uv.x *= iResolution.x / iResolution.y;

	vec2 mouse = iMouse.xy / iResolution.xy * 2. - 1.;
  mouse.x *= iResolution.x / iResolution.y * 4.;
  mouse.y = clamp(mouse.y, -.3, .5);

  vec3 ro =  vec3(0, 0, -2);
  vec3 rd = normalize(vec3(uv, FOV));

  vec3 light = vec3(-.4, .3, -1.);

  vec2 rot = vec2(0);
  if (iMouse.z > 0.)
    rot = -mouse;
  else
    rot = vec2(.6, .05);

  tRotate(rd.yz, rot.y);
  tRotate(rd.xz, rot.x);
  tRotate(ro.yz, rot.y);
  tRotate(ro.xz, rot.x);

  float steps, dist = trace(ro, rd, RENDER_DIST, steps); 

  vec3 p = ro + rd * dist;

  vec3 normal = getNormal(p);
  normal = doBumpMap( p, normal, .005);

  vec3 l = normalize(light - p);

  vec3 shadowStart = p + normal * EPS * 10.;
  float shadowDistance = distance(shadowStart,light);
  float shadow = softShadow(shadowStart, l, shadowDistance);

  float ambient = .02;

  float diffuse = max(0., dot(l, normal) * 2.);

  float specular = pow(max(0., dot(reflect(-l, normal), -rd)), 4.);

  float ao = calculateAO(p, normal) * .5 + .5;

	fragColor.rgb = (ao * _texture(p)) * (ambient * (2. - LIGHT_COLOR) * .5 + (specular + diffuse) * shadow * LIGHT_COLOR);

  vec4 fogColor = vec4(mix(vec3(.0, .025, .05), LIGHT_COLOR * 2., pow(max(0., dot(normalize(light), rd)), 10.)), 1);
  fragColor = mix(fragColor, fogColor, saturate(dist * dist * .01 - .1));

  if (length(p) > 1.) 
    fragColor *= saturate(1. - sqrt(steps / float(MAX_STEPS)));

  fragColor *= smoothstep(2., 0., length(uv));

  return fragColor;
}

void
main ()
{
  gl_FragColor = vec4(0);
  float increment = 1. / AA;
  for (float i = 0.; i < 1.; i += increment)
    for (float j = 0.; j < 1.; j += increment)
      gl_FragColor += frame(gl_FragCoord.xy + vec2(i,j));

  gl_FragColor = pow(gl_FragColor / AA / AA, vec4(1. / 2.2));
}
