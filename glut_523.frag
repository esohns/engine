// glut_523_common.glsl
#define PI 3.14159265359

float
iSphere (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - sph.w*sph.w;
  float h = b*b - c;
  if( h<0.0 ) return -1.0;
  return -b - sqrt( h );
}

#define ITERATIONS 4

#define HASHSCALE1 .1031
#define HASHSCALE3 vec3(.1031, .1030, .0973)
#define HASHSCALE4 vec4(.1031, .1030, .0973, .1099)

float
hash11 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

float
hash12 (vec2 p)
{
  vec3 p3  = fract(vec3(p.xyx) * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

float
hash13 (vec3 p3)
{
  p3  = fract(p3 * HASHSCALE1);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.x + p3.y) * p3.z);
}

vec2
hash21 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE3);
  p3 += dot(p3, p3.yzx + 19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xx+p3.yz)*p3.zy);

}

vec2
hash23 (vec3 p3)
{
  p3 = fract(p3 * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xx+p3.yz)*p3.zy);
}

vec3
hash31 (float p)
{
  vec3 p3 = fract(vec3(p) * HASHSCALE3);
  p3 += dot(p3, p3.yzx+19.19);
  return fract((p3.xxy+p3.yzz)*p3.zyx); 
}

vec3
hash32 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * HASHSCALE3);
  p3 += dot(p3, p3.yxz+19.19);
  return fract((p3.xxy+p3.yzz)*p3.zyx);
}

vec3
hash33 (vec3 p3)
{
  p3 = fract(p3 * HASHSCALE3);
  p3 += dot(p3, p3.yxz+19.19);
  return fract((p3.xxy + p3.yxx)*p3.zyx);
}

vec4
hash41 (float p)
{
  vec4 p4 = fract(vec4(p) * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash42 (vec2 p)
{
  vec4 p4 = fract(vec4(p.xyxy) * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash43 (vec3 p)
{
  vec4 p4 = fract(vec4(p.xyzx)  * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

vec4
hash44 (vec4 p4)
{
  p4 = fract(p4  * HASHSCALE4);
  p4 += dot(p4, p4.wzxy+19.19);
  return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

float
Noise (vec3 p)
{
  vec3 i = floor(p);
  vec3 f = fract(p); 
  f *= f * (3.0-2.0*f);

  return mix(mix(mix(hash13(i + vec3(0.,0.,0.)), hash13(i + vec3(1.,0.,0.)),f.x),
                  mix(hash13(i + vec3(0.,1.,0.)), hash13(i + vec3(1.,1.,0.)),f.x),
                  f.y),
              mix(mix(hash13(i + vec3(0.,0.,1.)), hash13(i + vec3(1.,0.,1.)),f.x),
                  mix(hash13(i + vec3(0.,1.,1.)), hash13(i + vec3(1.,1.,1.)),f.x),
                  f.y),
              f.z);
}

const mat3 m = mat3( 0.00,  0.80,  0.60,
                    -0.80,  0.36, -0.48,
                    -0.60, -0.48,  0.64 ) * 1.7;

float
FBM (vec3 p)
{
  float f;
  f = 0.5000 * Noise(p); p = m*p;
  f += 0.2500 * Noise(p); p = m*p;
  f += 0.1250 * Noise(p); p = m*p;
  f += 0.0625   * Noise(p); p = m*p;
  f += 0.03125  * Noise(p); p = m*p;
  f += 0.015625 * Noise(p);
  return f;
}
// glut_523_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel0;

float
craters (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  float va = 0.;
  float wt = 0.;
  for (int i = -2; i <= 2; i++)
    for (int j = -2; j <= 2; j++)
      for (int k = -2; k <= 2; k++)
      {
        vec3 g = vec3(i,j,k);
        vec3 o = 0.8 * hash33(p + g);
        float d = distance(f - g, o);
        float w = exp(-4. * d);
        va += w * sin(2.*PI * sqrt(d));
        wt += w;
      }
  return abs(va / wt);
}

void
main ()
{
  if (iFrame > 10)
  {
    gl_FragColor = texture(iChannel0, gl_FragCoord.xy/iResolution.xy);
    if (gl_FragColor.zw == iResolution.xy)
      return;
  }

  float lat = 180. * gl_FragCoord.y/iResolution.y - 90.;
  float lon = 360. * gl_FragCoord.x/iResolution.x;
  vec3 p = 1.5 * vec3(sin(lon*PI/180.) * cos(lat*PI/180.), sin(lat*PI/180.), cos(lon*PI/180.) * cos(lat*PI/180.));
  gl_FragColor.x = 0.;
  for (float i = 0.; i < 5.; i++)
  {
    float c = craters(0.4 * pow(2.2, i) * p);
    float noise = 0.4 * exp(-3. * c) * FBM(10. * p);
    float w = clamp(3. * pow(0.4, i), 0., 1.);
    gl_FragColor.x += w * (c + noise);
  }
  gl_FragColor.x = pow(gl_FragColor.x, 3.);
  gl_FragColor.zw = iResolution.xy;
}
