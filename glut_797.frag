#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define DECAY

#define PI 3.14159
#define TAU 6.28318

float pal_scale = 3.5;
float pal_offset = .6;

float noise_freq = 20.;
float noise_amt = .3;

#define pal(t,a,b,c,d) (a+(b)*cos(2.*PI*((c)*(t)+d)))
#define kick(t,b) (floor(t) + fract(t) + sin(fract(t)*PI)*b)

float
noise (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f * f * (3. - 2. * f);
  vec2 uv = (p.xy + vec2(37., 17.) * p.z) + f.xy;
  vec2 rg = textureLod(iChannel0, (uv + .5) / 512., 0.).yx;
  return -1. + 2.4 * mix(rg.x, rg.y, f.z);
}

float
fbm (vec3 p)
{
  return noise(p * .06125) * .5 + noise(p * .125) * .25 + noise(p * .25) * .125;
}

vec3
fold (vec3 p)
{
  vec3 nc = vec3(-.5, -.809017, .309017);
  for (int i = 0; i < 5; i++)
  {
    p.xy = abs(p.xy);
    p -= 2.*min(0., dot(p, nc))*nc;
  }
  return p - vec3(0, 0, 1.275);
}

float
sdf_shell (vec3 p, float scale)
{
  vec3 fp = fold(p * scale);
  float dist = dot(fp, normalize(sign(fp))) - .1 - sin(fp.y*.2)*2. - sin(fp.y*.7)*1.;
  dist += min(fp.x*1., sin(fp.y*.3));

  fp = fold(fp) - vec3(.1, 0, -.2);
  fp = fold(fp) - vec3(.4, 2, -2.3);
  fp = fold(fp) - vec3(.8, 1, .1);
  fp = fold(fp) - vec3(0, .3, 0);
  dist += sin(fp.y*.1)*.1;
  dist *= 1.;

  return dist / scale;
}

float
sdf_core (vec3 p, float scale)
{
  vec3 fp = fold(p * scale);
  float dist = dot(fp, normalize(sign(fp))) - .4 - sin(fp.y*.8)*1.8 - sin(fp.y*.7)*1.;
  dist += min(fp.x*8., sin(fp.y*.1));

  fp = fold(fp) - vec3(-.45, 6.7, -.1);
  fp = fold(fp) - vec3(-.3, 8.7, -0.9);
  fp = fold(fp) - vec3(0, .02, 0);
  dist += sin(fp.y*.2)*8.;
  dist *= .4;

  return dist / scale;
}

float
de (vec3 p)
{
  float s = 17.;
  p = mod(p - s * .5, s) - s * .5;

  float dist;
  dist = sdf_shell(p, .15 * 1.7);
  dist = min(dist, sdf_core(p, .4 * 1.7)) * .7;

#ifdef DECAY
  return dist + fbm(p*30.) * (.6+.45*sin(4.+1.*kick(iTime*.1, .1)));
#else
  return dist + fbm(p*noise_freq) * noise_amt;
#endif
}

vec3
camera (vec3 ro, vec3 ta, vec2 uv)
{
  vec3 fwd = normalize(ta - ro);
  vec3 left = normalize(cross(vec3(0, 1, 0), fwd));
  vec3 up = normalize(cross(fwd, left));
  return normalize(fwd + uv.x * left + up * uv.y);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy - .5;
  uv.x *= iResolution.x / iResolution.y;

  vec3 ray_ori = vec3(-10.*cos(kick(iTime*2., .1)), 16.*cos(kick(iTime*1., .1)), -10.*sin(kick(iTime, .1)));
  vec3 target = vec3(0);
  vec3 ray_dir = camera(ray_ori, target, uv);
  vec3 pos = ray_ori;

  float ldensity = 0.;

  vec3 bg = vec3(.16, .12, .22);
  vec4 sum = vec4(bg, 0.);

  float tmax = 45.;
  float tdist = 0., dist = 0.;

  for (float i = 0.; (i < 1.); i += 1. / 64.)
  {
    if (dist < tdist * .001 || tdist > tmax || sum.a > .95)
      break;

    dist = de(pos) * .5;

    const float h = .05;
    ldensity = (h - dist) * step(dist, h);

    vec4 col;
    col.rgb = pal(ldensity * pal_scale + pal_offset,
                  vec3(.7, .4, .2 + sin(iTime*1.5)*.1),
                  vec3(.9, .8, .4),
                  vec3(.4, .5, .6),
                  vec3(.8, .5, .3));
    col.a = ldensity;

    col.rgb *= col.a;
    sum += (1. - sum.a) * col;

    sum.a += .01;

    dist = max(dist, .03);

    pos += dist * ray_dir;
    tdist += dist;
  }

  sum *= 1. / exp(ldensity * .4) * 1.25;

  gl_FragColor = vec4(sum.rgb, 1.);
}
