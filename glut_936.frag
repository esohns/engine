uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;

#define PI 3.14159265359
#define rot(a) mat2(cos(a + PI*0.25*vec4(0,6,2,0)))
#define FOV 2.0
#define HEX vec2(1, 1.73205080757)

#define LIGHT_ENABLE
#define LIGHT_FREQ 0.3
#define LIGHT_COLOR vec3(0.05, 0.2, 0.8)

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * .1031);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hexCenter (vec2 p)
{
  vec2 centerA = (floor(p*HEX)+0.5)/HEX;
  vec2 centerB = (floor((p+HEX*0.5)*HEX)+0.5)/HEX-HEX*0.5;
  vec2 a = p-centerA;
  vec2 b = p-centerB;
  return dot(a,a)<dot(b,b) ? centerA : centerB;
}

vec3
getSphereCenter (vec2 c)
{
  return vec3(c, sin(c.x-c.y*4.3+iTime)*0.2);
}

float
de (vec3 p, vec3 dir, float r, out vec3 color)
{
  p.xy += iTime;
  vec2 center = hexCenter(p.xy);

  float red = floor(iTime*LIGHT_FREQ)+0.5;
  float fRed = smoothstep(0.5, 0.0, abs(fract(iTime*LIGHT_FREQ)-0.5));
  vec3 centerRed = getSphereCenter(hexCenter(red/LIGHT_FREQ + vec2(0.5, 1.5)));

#ifndef LIGHT_ENABLE
  fRed = 0.0;
#endif

  float d = 9e9;
  color = vec3(0);
  float colorAcc = 0.0;
  for (int i = 0 ; i < 7 ; i++)
  {
    float theta = float(i) * (2.0*PI/6.0);
    vec2 offset = vec2(sin(theta), cos(theta))*min(1.0/HEX.y, float(i));
    vec3 sphere = getSphereCenter(center + offset);
    vec3 inCenter = p - sphere;
    float len = length(inCenter);
    vec3 norm = inCenter / len;
    vec3 toRed = sphere-centerRed;

    float dist = len-0.3;
    d = min(d, dist);

    vec3 albedo = vec3(sin(sphere.x*90.0+sphere.y*80.0)*0.45+0.5);
    vec3 colorHere = vec3(0);

    if (dot(toRed, toRed) < 0.001)
    {
      albedo = mix(albedo, vec3(0.0), fRed);
      colorHere += LIGHT_COLOR*fRed*4.0;
    }
    else
    {
      vec3 lightDir = centerRed-p;
      float len = dot(lightDir, lightDir);
      lightDir *= inversesqrt(len);
      vec3 col = LIGHT_COLOR*fRed/(len+1.0)*2.0;
      colorHere += albedo*max(0.0, dot(norm, lightDir)+0.5/len)*col;
      colorHere += albedo*pow(max(0.0, dot(lightDir, reflect(dir, norm))), 8.0)*col;
    }

    const vec3 lightDir = normalize(vec3(1, -1, 3));
    colorHere += albedo*max(0.0, dot(lightDir, norm));
    colorHere += albedo*pow(max(0.0, dot(lightDir, reflect(dir, norm))), 8.0);

    float alpha = max(0.0001, smoothstep(r, -r, dist));
    color += colorHere*alpha;
    colorAcc += alpha;
  }
  color /= colorAcc;

  return d;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution * 0.5) / iResolution.y;
  vec3 from = vec3(0, 0, 1.2);
  vec3 dir = normalize(vec3(uv, -1.0/tan(FOV*0.5)));
  dir.yz *= rot(-0.85);
  dir.xy *= rot(0.2);

  float focal = 2.5;
  if (iMouse.z > 0.0)
    focal = 1.0+iMouse.y/iResolution.y*4.0;
  float sinPix = sin(FOV / iResolution.y);
  vec4 acc = vec4(0, 0, 0, 1);
  vec3 dummy = vec3(0);
  float totdist = de(from, dir, 0.0, dummy)*hash13(vec3(gl_FragCoord.xy, iFrame));
  for (int i = 0 ; i < 100 ; i++)
  {
    vec3 p = from + totdist * dir;
    float r = max(totdist*sinPix, abs((totdist-focal)*0.1));
    vec3 color = vec3(1);
    float dist = de(p, dir, r, color);

    float alpha = smoothstep(r, -r, dist);
    acc.rgb += acc.a * (alpha*color.rgb);
    acc.a *= (1.0 - alpha);

    if (acc.a < 0.01)
      break;

    totdist += max(abs(dist), r*0.5);
  }

  gl_FragColor.rgb = clamp(acc.rgb, vec3(0), vec3(1));
  gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.0 / 2.2));
  gl_FragColor.a = 1.0;
}
