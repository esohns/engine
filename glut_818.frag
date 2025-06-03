#version 130

// glut_818_common.glsl
float gyroid (vec3 seed) { return dot(sin(seed),cos(seed.yzx)); }

float
fbm (vec3 seed)
{
  float result = 0., a = .5;
  for (int i = 0; i < 3; ++i, a/=2.)
    result += (gyroid(seed/a))*a;
  return result;
}

float
hash11 (float p)
{
  p = fract(p * .1031);
  p *= p + 33.33;
  p *= p + p;
  return fract(p);
}

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx + 33.33);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec4
hash41 (float p)
{
  vec4 p4 = fract(vec4(p) * vec4(.1031, .1030, .0973, .1099));
  p4 += dot(p4, p4.wzxy+33.33);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}
// glut_818_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform float iTimeDelta;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

const float count = 100.;

float speed = .3;
float friction = 3.;
float fade = 0.1;
float thin = 0.02;

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;
  vec2 p = (2. * gl_FragCoord.xy - iResolution.xy) / iResolution.y;

  vec4 frame = texture(iChannel0, uv);

  if (gl_FragCoord.y < 1. && gl_FragCoord.x < count)
  {
    float id = gl_FragCoord.x;

    if (iFrame < 1)
    {
      frame = vec4(hash41(id)*2.-1.);
      frame.zw *= .01;
    }
    else
    {
      vec2 aspect = vec2(iResolution.x/iResolution.y, 1.);
      vec2 p = frame.xy;
      vec2 offset = vec2(0);
      vec2 target = vec2(0);

      float t = iTime * 10.;
      float idd = id+floor(t) * count;
      if (hash11(idd) > .95 && fract(t) < .1)
      {
        frame = hash41(idd)*2.-1.;
        frame.xy *= aspect;
        frame.zw *= .01;
        gl_FragColor = frame;
        return;
      }

      if (iMouse.z > 0.)
        target = (2.*iMouse.xy-iResolution.xy)/iResolution.y;

      float noise = fbm(vec3(p, length(p) + iTime));
      float a = noise * 6.28;
      offset += vec2(cos(a), sin(a));

      offset += normalize(target.xy-p) * 2. * length(target.xy-p);

      offset += (hash21(id)*2.-1.)*(.5+.5*sin(iTime));

      vec2 velocity = frame.zw;
      velocity = velocity * (1.-friction*iTimeDelta) + offset * speed * iTimeDelta;

      frame.xy += velocity;
      frame.zw = velocity;
    }
  }
  else
  {
    float matID = 0.;
    float dist = 100.;
    float dither = texture(iChannel1, gl_FragCoord.xy/1024.).r;

    for (float i = 0.; i < count; ++i)
    {
      vec4 data = texelFetch(iChannel0, ivec2(i,0), 0);

      vec2 pos = data.xy - data.zw * dither;
      float shape = length(pos-p);
      matID = shape < dist ? i : matID;
      dist = min(dist, shape);
    }

    float shade = smoothstep(thin,.0,dist);

    frame.r = max(frame.r - fade, shade);

    if (dist < thin)
      frame.g = matID;
  }

  gl_FragColor = frame;
}
