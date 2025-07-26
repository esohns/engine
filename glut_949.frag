#version 130

// glut_949_common.glsl
#define PI 3.14159265359
#define HALFPI 1.57079632679

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec4(.1031, .1030, .0973, .1099)

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE3);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec4
hash42 (vec2 p)
{
  vec4 p4 = fract(vec4(p.xyxy) * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

mat2
rot (float a) 
{
  vec2 s = sin(vec2(a, a + HALFPI));
  return mat2(s.y,s.x,-s.x,s.y);
}

vec2
CalculateUv (vec2 coord, float time)
{
  vec2 uv = coord;
  uv *= rot(time*0.1);
  uv += sin(vec2(time*0.2, time*0.3 + HALFPI)) * 0.35;

  vec4 disto = sin(uv.xxyy * vec4(8.1, 7.8, 7.7, 8.3) + vec4(0.3, -0.4, 0.25, -0.3) * time) * vec4(0.01, 0.015, 0.007, 0.012);
  uv.x += disto.z + disto.w;
  uv.y += disto.x + disto.y;
  return uv;
}
// glut_949_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

float
star (vec2 uv, vec2 s, vec2 offset)
{
  uv += offset;
  uv *= 2.0;
  float l = length(uv);
  l = sqrt(l);
  vec2 v = smoothstep(s, vec2(0.0), vec2(l));

  return v.x + v.y*0.1;
}

vec4
starField (vec2 uv)
{
  vec2 fracuv = fract(uv);
  vec2 flooruv = floor(uv);
  vec4 r = hash42(flooruv);
  vec4 color = mix(vec4(0.5, 1.0, 0.25, 1.0), vec4(0.25, 0.5, 1.0, 1.0), dot(r.xy, r.zw)) * 4.0 * dot(r.xz, r.yw);

  float t = iTime*2.0;
  vec2 o = sin(vec2(t, t + HALFPI) * r.yx) * r.zw * 0.75;

  return color * star((fracuv - 0.5) * 2.0, vec2(0.4, 0.75) * (0.5 + 0.5*r.xy), o);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution;
  uv -= 0.5;
  uv.x *= iResolution.x/iResolution.y;

  uv = CalculateUv(uv, iTime);

  vec4 res = vec4(0.0);
  float t = iTime * -0.075;
  const float itter = 15.0;
  float tex = 0.0;
  vec2 disto = vec2(0.0);

  for(float f = 0.0; f < itter; f++)
  {
    vec2 r = hash21(f);
    float t2 = fract(t + f / itter);
    float size = mix(30.0, 0.001, t2);
    vec2 fade = smoothstep(vec2(0.0, 1.0), vec2(0.9, 0.9), vec2(t2));

    vec2 uv2 = uv * size + r * 100.0 + (r - 0.5) * iTime * 0.25;
    res += starField(uv2) * fade.x * fade.y * 0.65;
    tex = texture(iChannel1, uv2*0.1 + tex * 0.15).x;
    disto += vec2(tex) / itter;
    res += tex * tex * 3.0 * fade.x * fade.y * vec4(0.25, 0.35, 0.5, 1.0) / itter;
  }

  vec2 distuv = uv + disto*0.15;
  vec4 sun = smoothstep(vec4(0.25, 3.0, 0.0025, 4.5), vec4(0.0, 0.0, 0.0, 0.0), vec4(dot(distuv, distuv)));
  sun = sun * sun * sun * sun;
  sun.xyz *= 0.5;

  vec2 pc;
  pc.x = iTime*0.02;
  pc.y = (atan(distuv.x, distuv.y) / PI) + iTime * 0.05;

  float rays = (texture(iChannel2, pc).x + texture(iChannel2, pc * vec2(1.0, 0.5)).x) * sun.y * 0.5;

  vec4 bg = mix(vec4(0.01, 0.01, 0.075, 1.0), vec4(0.05, 0.065, 0.1, 1.0), clamp(sun.x + rays, 0.0, 1.0) + sun.y);
  bg += sun.z*vec4(0.2, 0.2, 0.05, 0.0) + sun.x * vec4(0.05, 0.05, 0.075, 0.0);

  vec4 c = bg + res * bg * 0.5 + res*0.5;
  c *= sun.w;

  vec4 old = texture(iChannel0, gl_FragCoord.xy/iResolution);
  gl_FragColor = c + old * 0.85;
}
