uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define M_PI 3.1415926

vec2
hash2 (vec2 p)
{
  return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

vec3
voronoi (vec2 x)
{
  vec2 n = floor(x);
  vec2 f = fract(x);

  vec2 mg, mr;

  float md = 8.0;
  for (int j = -1; j <= 1; j++)
    for (int i = -1; i <= 1; i++)
    {
      vec2 g = vec2(float(i), float(j));
      vec2 o = hash2(n + g);
      vec2 r = g + o - f;
      float d = dot(r, r);

      if (d < md)
      {
        md = d;
        mr = r;
        mg = g;
      }
    }

  md = 8.0;
  for (int j = -2; j <= 2; j++)
    for (int i = -2; i <= 2; i++)
    {
      vec2 g = mg + vec2(float(i), float(j));
      vec2 o = hash2(n + g);
      vec2 r = g + o - f;

      if (dot(mr - r, mr - r) > 0.00001)
        md = min(md, dot(0.5 * (mr + r), normalize(r - mr)));
    }

  return vec3(md, mr);
}

#define rot(a) mat2(cos(a),-sin(a),sin(a),cos(a))
#define hash21(p) fract(sin(dot(p,vec2(127.1,311.7)))*43758.5453123)
float
noise2_ (vec2 p)
{
  vec2 i = floor(p);
  vec2 f = fract(p);
  f = f * f * (3. - 2. * f);

  float v = mix(mix(hash21(i+vec2(0,0)), hash21(i+vec2(1,0)), f.x),
                mix(hash21(i+vec2(0,1)), hash21(i+vec2(1,1)), f.x), f.y);
  return 2. * v - 1.;
}

#define noise22(p) vec2(noise2_(p),noise2_(p+17.7))
vec2
fbm22 (vec2 p)
{
  vec2 v = vec2(0);
  float a = .5;
  mat2 R = rot(.37);

  for (int i = 0; i < 6; i++, p *= 2., a /= 2.) 
    p *= R, v += a * noise22(p);

  return v;
}

float
fbm2 (vec2 p)
{
  float v = 0., a = .5;
  mat2 R = rot(.37);

  for (int i = 0; i < 9; i++, p *= 2., a /= 2.) 
    p *= R, v += a * noise2_(p);

  return v;
}

float
luminance (vec3 col)
{
  return dot(vec3(0.298912, 0.586611, 0.114478), col);
}

vec3
contrast (vec3 col, float scale)
{
  return (col - 0.5) * scale + 0.5;
}

#define saturate(x) clamp(x, 0.0, 1.0)

float
remap (float val, float inMin, float inMax, float outMin, float outMax)
{
  return clamp(outMin + (val - inMin) * (outMax - outMin) / (inMax - inMin), outMin, outMax);
}

float
roadTex (vec2 p)
{
  p *= 0.1;
  float n = noise2_(p * 1000.0);
  float f = remap(fbm2(p * 12.0), -0.2, 0.4, 0.0, 1.0);

  float d = 999999.9;

  float height = 1.0;
  float alpha = 2.0;

  for (float i = 0.; i < 5.; i++)
  {
    vec3 c = voronoi(8.0 * p + fbm22(p * 8.0));
    height -= smoothstep(0.05, 0.0, c.x) * alpha * remap(fbm2(p * 12.0), 0.0, 0.4, 0.0, 1.0);
    alpha *= 0.55;
    d = min(d, c.x);
    p *= 1.5 * rot(.37);
  }

  vec3 col = vec3(height + n * 0.1 - f);

  col = contrast(col, 0.05);
  return col.x;
}

const vec3 sunDir = normalize(vec3(.3, .25, .5));
vec3
skyColor (vec3 rd)
{
  float sundot = clamp(dot(rd, sunDir), 0.0, 1.0);

  vec3 col = mix(vec3(0.2, 0.5, 0.85) * 1.1, vec3(0.0, 0.15, 0.7), rd.y);
  col = mix(col, 0.85 * vec3(0.8, 0.8, 0.7), pow(1.0 - max(rd.y, 0.0), 4.0));

  col += 0.3 * vec3(1.0, 0.7, 0.4) * pow(sundot, 5.0);
  col += 0.5 * vec3(1.0, 0.8, 0.6) * pow(sundot, 64.0);
  col += 6.0 * vec3(1.0, 0.8, 0.6) * pow(sundot, 1024.0);

  return col * 10.0;
}

float
trace (vec3 p, vec3 ray)
{
  float dist = 99999.9;
  float t = -p.y / ray.y;
  if (t > 0.0)
    dist = t;
 
  return dist;
}

mat3
camera (vec3 ro, vec3 ta, float cr)
{
  vec3 cw = normalize(ta - ro);
  vec3 cp = vec3(sin(cr), cos(cr), 0.);
  vec3 cu = normalize(cross(cw, cp));
  vec3 cv = normalize(cross(cu, cw));
  return mat3(cu, cv, cw);
}

vec3
reinhard (vec3 col, float exposure, float white)
{
  col *= exposure;
  white *= exposure;
  float lum = luminance(col);
  return (col * (lum / (white * white) + 1.0) / (lum + 1.0));
}

vec3
acesFilm (vec3 x)
{
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3
normal (vec3 pos)
{
  vec3 e = vec3(0.001, 0.0, 0.0);
  return normalize(vec3(roadTex(pos.xz - e.xy) - roadTex(pos.xz + e.xy),
                        20.0 * e.x,
                        roadTex(pos.xz - e.yx) - roadTex(pos.xz + e.yx)));
}

float
ndfGGX (float NdotH, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSq = alpha * alpha;

  float denom = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
  return alphaSq / (M_PI * denom * denom);
}

float
gaSchlickG1 (float theta, float k)
{
  return theta / (theta * (1.0 - k) + k);
}

float
gaSchlickGGX (float NdotL, float NdotV, float roughness)
{
  float r = roughness + 1.0;
  float k = (r * r) / 8.0;
  return gaSchlickG1(NdotL, k) * gaSchlickG1(NdotV, k);
}

vec3
fresnelSchlick_roughness (vec3 F0, float cosTheta, float roughness)
{
  return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3
PBR (vec3 pos, vec3 albedo, float metalness, float roughness, vec3 N, vec3 V, vec3 L, vec3 Lradiance)
{
  vec3 H = normalize(L + V);
  float NdotV = max(0.0, dot(N, V));
  float NdotL = max(0.0, dot(N, L));
  float NdotH = max(0.0, dot(N, H));
    
  vec3 F0 = mix(vec3(0.04), albedo, metalness);

  vec3 F = fresnelSchlick_roughness(F0, max(0.0, dot(H, L)), roughness);
  float D = ndfGGX(NdotH, roughness);
  float G = gaSchlickGGX(NdotL, NdotV, roughness);

  vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);
    
  vec3 diffuseBRDF = kd * albedo / M_PI;
  vec3 specularBRDF = (F * D * G) / max(0.0001, 4.0 * NdotL * NdotV);

  return (diffuseBRDF + specularBRDF) * Lradiance * NdotL;
}

vec3
EnvBRDFApprox (vec3 SpecularColor, float Roughness, float NoV)
{
  const vec4 c0 = vec4(-1, -0.0275, -0.572, 0.022);
  const vec4 c1 = vec4(1, 0.0425, 1.04, -0.04);
  vec4 r = Roughness * c0 + c1;
  float a004 = min(r.x * r.x, exp2(-9.28 * NoV)) * r.x + r.y;
  vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
  return SpecularColor * AB.x + AB.y;
}

vec3
calcAmbient (vec3 pos, vec3 albedo, float metalness, float roughness, vec3 N, vec3 V)
{
  vec3 F0 = mix(vec3(0.04), albedo, metalness);
  vec3 F = fresnelSchlick_roughness(F0, max(0.0, dot(N, V)), roughness);
  vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

  vec3 ambientColor = mix(vec3(0.2, 0.5, 0.85) * 0.8, vec3(0.1, 0.25, 0.8) * 0.5, 0.5 + 0.5 * N.y);
    
  vec3 diffuseAmbient = kd * albedo * ambientColor * min(1.0, 0.75 + 0.5 * N.y) * 3.0;
  vec3 R = reflect(-V, N);
    
  vec3 col = mix(skyColor(R) * pow(1.0 - max(-R.y, 0.0), 4.0), mix(vec3(0.2, 0.5, 0.85) * 0.8, vec3(0.1, 0.25, 0.8) * 0.5, 0.5 + 0.5 * R.y) * 3.0 * (0.5 + 0.5 * R.y), pow(roughness, 0.5));

  vec3 ref = EnvBRDFApprox(F0, roughness, max(dot(N, V), 0.0));

  vec3 specularAmbient = col * ref;

  return vec3(diffuseAmbient + specularAmbient);
}

void
wet (inout vec3 albedo, inout float roughness, inout vec3 norm, float w)
{
  albedo = mix(albedo, albedo * albedo, remap(w, 0.0, 0.35, 0.0, 1.0) * roughness);
  roughness = mix(roughness, 0.05, remap(w, 0.25, 0.5, 0.0, 1.0));
  norm = mix(norm, vec3(0.0, 1.0, 0.0), vec3(remap(w, 0.45, 0.95, 0.0, 1.0)));
}

void
main ()
{
  vec2 p = (gl_FragCoord.xy * 2.0 - iResolution.xy) / min(iResolution.x, iResolution.y);

  float mousex = 10.0 * iMouse.x / iResolution.x + 0.75;
  float mousey = remap(iMouse.y / iResolution.y, 0.0, 1.0, 1.5, 10.0);
  vec3 ro = vec3(-5.0 * cos(mousex), mousey, -5.0 * sin(mousex) + iTime);
  vec3 ta = vec3(0.0, 0.0, iTime);
  mat3 c = camera(ro, ta, 0.0);
  vec3 ray = c * normalize(vec3(p, 3.5));
  float depth = trace(ro, ray);

  vec3 pos = ro + ray * depth;

  float sundot = clamp(dot(ray, sunDir), 0.0, 1.0);
  float height = roadTex(pos.xz);
  vec3 albedo = vec3(pow(height, 2.2));
  vec3 n = normal(pos);
  float roughness = 1.0 - albedo.x;

  float tt = remap(sin(iTime * 0.5) * 0.5 + 0.5, 0.0, 1.0, 0.33, 0.5);
  wet(albedo, roughness, n, smoothstep(tt + 0.03, tt, height));

  vec3 col = PBR(pos, albedo, 0.0, roughness, n, -ray, sunDir, vec3(50.0));
  col += calcAmbient(pos, albedo, 0.0, roughness, n, -ray);

  col += 1.5 * vec3(1.0, 0.7, 0.3) * pow(sundot, 4.0);
  col = acesFilm(col * 0.08);
  col = pow(col, vec3(1.0 / 2.2));

  gl_FragColor = vec4(col, 1.0);
}
