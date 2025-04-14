#version 130

// glut_665_common.glsl
#define SAMPLES 64.0
#define TINT vec4(1.5, 1.2, 1, 1)

vec2 hash2 (vec2 p) { return normalize(fract(cos(p*mat2(95,74,86,83))*3742.0)-0.5); }

vec4
fibonacci_blur (sampler2D tex, vec2 uv, vec2 texel, float radius)
{
  vec4 blur = vec4(0.0);
  float total = 0.0;

  float scale = radius/sqrt(SAMPLES);
  vec2 point = hash2(uv)*scale;

  float rad = 1.0;
  mat2 ang = mat2(-0.7373688, -0.6754904, 0.6754904,  -0.7373688);

  for(float i = 0.0; i<SAMPLES; i++)
  {
    point *= ang;
    rad += 1.0/rad;

    vec2 coord = uv + point*(rad-1.0)*texel;
    float weight = 1.0/(1.0+i);
    vec4 samp = texture(tex, coord);

    blur += samp * weight;
    total += weight;
  }
  blur /= total;

  return blur;
}
// glut_665_common.glsl

uniform vec2 iResolution;
uniform float iTime;

#define MAX 100.
#define EPS 4e-4

float hash (vec2 p) { return fract(sin(p.x*75.3 + p.y*94.2)*4952.); }

float
value (vec2 p)
{
  vec2 f = floor(p);
  vec2 s = p-f;
  s *= s * (3.0 - 2.0 * s);
  vec2 o = vec2(0, 1);

  return mix(mix(hash(f+o.xx),hash(f+o.yx),s.x),
             mix(hash(f+o.xy),hash(f+o.yy),s.x),s.y);
}

float
dist (vec3 p)
{
  vec2 n = p.xz*0.6+1.0;
  mat2 m = mat2(0.6754904, 0.7373688, -0.7373688, 0.6754904)*2.0;
  float weight = 0.3;
  float water = 0.0;
  float speed = 0.3;
  for(int i = 0; i<10; i++)
  {
    water += smoothstep(0.1, 0.9, value(n+speed*iTime)) * weight;
    n *= m;
    speed *= 1.3;
    weight *= 0.45;
  }

  return (water+0.5-p.y);
}

vec3
normal (vec3 p)
{
  vec2 e = vec2(4,-4)*EPS;
  return normalize(dist(p+e.yxx)*e.yxx+dist(p+e.xyx)*e.xyx+
                   dist(p+e.xxy)*e.xxy+dist(p+e.yyy)*e.yyy);
}

void
main ()
{
  vec3 ray = normalize(vec3(gl_FragCoord.xy*2.0 - iResolution.xy, iResolution.x));
  ray.yz *= mat2(cos(0.5+vec4(0,11,33,0)));
  vec3 pos = vec3(iTime*0.2,0,0);
  vec4 mar = vec4(pos,0);

  for(int i = 0; i<50; i++)
  {
    float stp = dist(mar.xyz);
    mar += vec4(ray, 1) * stp;

    if (stp<EPS || mar.w>MAX)
      break;
  }
  vec3 nor = normal(mar.xyz);
  vec3 sun = normalize(vec3(0,-1,9));
  vec3 ref = refract(ray, nor, 1.333);
  float spec = exp(dot(ref, sun) * 9.0 - 9.0);
  float fog = max(1.0 - mar.w/MAX, 0.0);

  gl_FragColor = vec4(sqrt(spec) * fog);
}
