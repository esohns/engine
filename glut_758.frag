#version 130

// glut_758_common.glsl
#define RESOLUTION min(vec2(1280, 720), iResolution.xy)
// glut_758_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler3D iChannel0;

#define NUM_STEPS 256

vec3
ACES (vec3 x)
{
  float a = 2.51;
  float b = .03;
  float c = 2.43;
  float d = .59;
  float e = .14;
  return (x * (a * x + b)) / (x * (c * x + d) + e);
}

float
hash (float n)
{
  return fract(sin(n) * 43758.5453123);
}

float
noise (vec3 x)
{
  vec3 p = floor(x + .5);
  vec3 f = fract(x + .5);
  f = f * f * (3. - 2. * f);
  x = p + f - .5;
  
  return textureLod(iChannel0, x / vec3(textureSize(iChannel0, 0)), 0.).r;
}

float
map (vec3 p)
{
  float f = 0.;
    
  vec3 t = iTime * vec3(-.5, -.75 * sign(p.x), .5);
  p *= 3.;
  f += .5 * noise(p + t * .4);
  f += .25 * noise(2. * p + t * .2);
  f += .0625 * noise(8. * p + t * .1);
  f += .03125 * noise(16. * p + t * .05);
  f += .015625 * noise(32. * p + t * .025);
    
  f += cos(p.x * 2.25) * .3;
  f -= .35;

  return -256. * f;
}

float
getLight (float h, float k, vec3 ce, vec3 p)
{
  vec3 lig = ce - p;
  float llig = length(lig);
  lig = normalize(lig);
  float sha = clamp((h - map(p + lig * k)) / 128., 0., 1.);
  float att = 1. / (llig * llig);
  return sha * att;
}

vec3
render (vec3 ro, vec3 rd)
{
  float tmax = 6.;
  float s = tmax / float(NUM_STEPS);
  float t = 0.;
  t += s * hash(gl_FragCoord.x * 8315.9213 / iResolution.x + gl_FragCoord.y * 2942.5192 / iResolution.y);

  vec4 sum = vec4(0, 0, 0, 1);
  for (int i = 0; i < NUM_STEPS; i++)
  {
    vec3 p = ro + rd * t;
    float h = map(p);

    if (h > 0.)
    {
      float occ = exp(-h * .1);

      float k = .08;
      vec3 col = 3. * vec3(.3, .6, 1) * getLight(h, k, ro + vec3(0.75, -1, 1.5), p) * occ + 3. * vec3(1, .2, .1) * getLight(h, k, ro + vec3(-0.5, 1.25, 2.0), p) * occ;

      sum.rgb += h * s * sum.a * col;
      sum.a *= exp(-h * s);
    }

    if (sum.a < .01)
      break;
    t += s;
  }

  return sum.rgb;
}

mat3
setCamera (vec3 ro, vec3 ta)
{
  vec3 w = normalize(ta - ro);
  vec3 u = normalize(cross(w, vec3(-1, 0, 0)));
  vec3 v = cross(u, w);
  return mat3(u, v, w);
}

void
main ()
{
  if (gl_FragCoord.x > RESOLUTION.x || gl_FragCoord.y > RESOLUTION.y)
  {
    gl_FragColor = vec4(0.0);
    return;
  }

  vec2 p = (gl_FragCoord.xy - .5 * RESOLUTION) / RESOLUTION.y;

  vec3 ro = vec3(0, 0, iTime * 0.4);
  vec3 ta = ro + vec3(0, 0, 1);
  mat3 ca = setCamera(ro, ta);
  vec3 rd = ca * normalize(vec3(p, 1.5));

  vec3 col = render(ro, rd);

  col = ACES(col);
  col = pow(col, vec3(.4545));

  vec2 q = gl_FragCoord.xy / RESOLUTION;
  col *= pow(16. * q.x * q.y * (1. - q.x) * (1. - q.y), .1);

  gl_FragColor = vec4(col, 1.0);
}
