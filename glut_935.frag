#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define _BaseSize         0.18
#define _BaseOffset       0.2
#define _BaseGradient    -1.8
#define _Animate          1.0
#define _AnimateBase      0.53
#define _AnimateDetail   -0.11
#define _AnimateDetail2   0.1
#define _AnimateBaseV     vec3(0.0, 0.00, 1.00)
#define _AnimateDetailV   vec3(1.0, 0.31, 0.55)
#define _AnimateDetail2V  vec3(1.0, 0.00, 0.00)
#define _BaseShape        1.54
#define _BaseWeight       2.06
#define _DetailWeight     0.93
#define _DetailCombine   -0.25
#define _DensityCutoff    0.47
#define _LowDensityColor  vec4(0.668, 0.586, 0.801, 0.043)
#define _HighDensityColor vec4(0.317, 0.250, 0.368, 0.270)
#define _SunColor         vec4(1.000, 0.572, 0.410, 0.620)
#define _BackgroundC      vec4(0.317, 0.250, 0.368, 0.886)
#define _BackgroundSunC   vec4(0.698, 0.321, 0.239, 0.317)
#define _BackgroundSunSize 0.22
#define _Near             6.0
#define _Far              42.0
#define _FarFade          0.75
#define _SunDir           vec3(-0.61, -0.1, 1.96)
#define _SunCurve         0.286
#define _SunOffset        0.963
#define _Gamma            4.0
#define _DetailCutoff    -0.4
#define _AlphaMax         0.93
#define _StepSizeInside   0.27
#define _StepSizeOutside  0.66
#define _Jitter           2.0
#define _Debug            0.0  // 0.8 to see iterations, 0.6 to see 1st density hit

#define Loop_Max 159

float
hashu (uvec2 q)
{
  q *= uvec2(1597334677U, 3812015801U);
  uint n = (q.x ^ q.y) * 1597334677U;
  return float(n) * (1.0 / float(0xffffffffU));
}

float
hash (vec2 n)
{
  return hashu(uvec2(n * 9e5));
}

float
noise (vec3 c)
{
  vec3 p = floor(c);
  vec3 f = fract(c);
  f = f*f*(3.0-2.0*f);
  vec2 uv = (p.xy+vec2(37.0,239.0)*p.z) + f.xy;
  vec2 texSize = vec2(textureSize(iChannel0, 0));
  vec2 rg = textureLod(iChannel0, (uv + 0.5) / texSize.x, 0.0).yx;
  return mix( rg.x, rg.y, f.z )*2.0-1.0;
}

float
cloudDensity (vec3 p, float len)
{
  float linearField = (p.y * _BaseGradient + _BaseOffset) * _BaseWeight;
  if (linearField < -_BaseWeight)
    return -1000.;
  float time = iTime * _Animate;
  p += time * _AnimateBase * _AnimateBaseV;

  float basenoise = noise(mat3(0.5, -0.5, 0.70711, 0.85355, 0.14644, -0.5, 0.14644, 0.85355, 0.5) * p);

  float nd = 0.5 + 0.5 * basenoise;
  nd = pow(nd, _BaseShape);
  float d = _BaseWeight * (nd*2.-1.);
  d += linearField;

  if (d > _DetailCutoff)
  {
    p += time * _AnimateDetail * _AnimateDetailV;
    float f = 0.28 * noise(p * 5.);
    f += 0.2 * noise(p * 10.1);
    p += time * _AnimateDetail2 * _AnimateDetail2V;
    f += 0.1 * noise(p * 22.52);

    float fadeDist = 6.;
    if (len < fadeDist)
    {
      f += 0.057 * noise(p * 54.28);

      float fadeByDist = (fadeDist - len) * (1. / fadeDist);
      f += 0.1 * fadeByDist * noise(p * 154.28);
    }

    d += _DetailWeight * f * (1.0 - _DetailCombine * (nd - 0.5));
  }

  return d;
}

float gm (float c) { return pow(c, _Gamma); }
vec3 gm (vec3 c) { return pow(c.rgb, vec3(_Gamma)); }
vec4 gm (vec4 c) { c.rgb = pow(c.rgb, vec3(_Gamma)); return c; }

vec4
rm (vec3 ro, vec3 rd, vec2 uv)
{
  float len = 0.0;
  vec4 sum = vec4(0.0);
  float depthAlphaSum = 0.0;

  len = _Jitter * hash(uv+fract(iTime));

  vec4 ldColor = gm(_LowDensityColor);  
  vec4 hdColor = gm(_HighDensityColor);
  vec3 sunColor = gm(_SunColor).rgb;
  float ambient = gm(_SunColor.a);

  int n = 0;
  for (; n < Loop_Max; ++n)
  {
    vec3 p = ro + len * rd;
    p *= _BaseSize;
    float d = cloudDensity(p, len * _BaseSize);

    if (d < -100. && sign(_BaseGradient)*rd.y < 0.)
      break;

    if (d > _DensityCutoff)
    {
      if(_Debug > 0.5 && _Debug < 0.75)
      { 
        vec4 col = vec4(d, d, d, 1.0);
        sum += col * (1.0 - sum.a);
      }
      else
      {
        float sundiff = cloudDensity(p+0.06*normalize(_SunDir), len * _BaseSize)-d;

        float sunCurve = pow(max(0.0, _SunOffset - p.y*_BaseGradient*_SunCurve), 8.);

        float sun = max(0.0, -sunCurve * sundiff / 0.2);

        d = clamp(d, 0., 1.);
        vec4 col = mix(ldColor, hdColor, d);
        col.rgb *= ambient + sunColor * sun * 5.;

        col.a *= min(1.0, len / _Near);

        float weight = col.a * (1.0 - sum.a);
        depthAlphaSum += weight * smoothstep(1.0 - _FarFade, 1.0, len / _Far);                               
        sum.rgb += weight * col.rgb;
        sum.a += weight;
      }
    }

    len += d > _DetailCutoff ? _StepSizeInside : _StepSizeOutside;

    if (len > _Far || sum.a > _AlphaMax)
      break;
  }

  if (_Debug >= 0.75)
    return vec4(vec3((float(n)/float(Loop_Max))), 1.0);

  float depthAlpha = 1.0 - depthAlphaSum / (sum.a + 0.00001);

  sum.a = sum.a / _AlphaMax;

  sum.a *= depthAlpha;

  sum.a = min(1.0, sum.a);

  return sum;
}

vec4
renderPixel (vec3 ro, vec3 rd, vec2 uv)
{
  vec4 col = rm(ro, rd, uv);
  float sun = 0.5 + 0.5 * dot(normalize(_SunDir), rd);
  sun = pow((1. - pow(1. - sun, _BackgroundSunSize) ),1./ _BackgroundSunSize)* 5.;

  sun *= 1. + col.a * 6.;

  vec4 bg = gm(_BackgroundC) + _BackgroundSunC.a * sun * gm(_BackgroundSunC);

  col = mix(bg, vec4(col.rgb,1.0), col.a);

  col.rgb = pow(col.rgb, vec3(1.0 / _Gamma));

  col.rgb *= max(1.0, col.a);
  col.a = min(1.0, col.a);

  return col;
}

vec2 rot (vec2 p, float r) { float s = sin(r),  c = cos(r); return vec2(p.x * c - p.y * s, p.x * s + p.y * c); } 

vec3
camraydir (vec2 uv, float aspect, vec3 pos, vec3 target, float fov, float roll)
{
  vec3 forward = normalize(target - pos);
  vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
  vec3 up = normalize(cross(forward, right));
  float halfH = tan(fov * 0.5);
  uv = (2.0 * uv - 1.0) * vec2(halfH * aspect, halfH);
  vec2 rotatedUV = mat2(cos(roll), -sin(roll), sin(roll), cos(roll)) * uv;
  return normalize(forward + rotatedUV.x * right + rotatedUV.y * up);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution;
  float aspect = iResolution.x / iResolution.y;
  vec2 mouse = (iMouse.xy-0.5*iResolution)/iResolution.y;

  vec3 campos = vec3(0.0,1.7,-3.0);
  vec3 camtarget = vec3(0.0,1.7,6.0);

  float orbit = 0.0;
  float updown = 0.0;
  if (iMouse.z > 0.)
  {
    updown = clamp(-4.*mouse.y, -3.14159/2.0, 3.14159/2.0);
    orbit = -4.*mouse.x;
  }
  campos -= camtarget;
  campos.yz = rot(campos.yz, updown);
  campos.xz = rot(campos.xz, orbit);    
  campos += camtarget;

  vec3 ro = campos;
  vec3 rd = camraydir(uv, aspect, campos, camtarget, radians(40.), 0.0);
  ro += vec3(125.,0.,64.);
  gl_FragColor = renderPixel(ro, rd, uv);
}
