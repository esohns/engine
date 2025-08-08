#version 130

// glut_1000_common.glsl
struct ColorTheme
{
  vec3 colorX;
  vec3 colorY;
  vec3 colorZ;
  vec3 diffuseColor;
  vec3 specularColor;
  vec3 lightDirection;
  float specularPower;
  float lightIntensity;
};

#define KERNEL_CONVOLUTION_SIZE_HIGH 6.0

#define uAntiAliasingLevel 4

#define uMouseAngle 5.0
#define uMouseFactor -0.2
#define uMouseScale 10.0

#define uRotationAngle 1.5
#define uRotationIntensity 0.4

#define uTimeStart 7.5
#define uThemeVariationSpeed 0.05
#define uNormalDirection 1.0

#define uBufferMove iChannel0
#define uBufferProp iChannel1
#define uBufferNorm iChannel2
#define uBufferTrail iChannel3

#define getMove(coord) texture(uBufferMove,(coord)/iResolution)
#define getProp(coord) texture(uBufferProp,(coord)/iResolution)
#define getNorm(coord) texture(uBufferNorm,(coord)/iResolution)
#define getTrail(coord) texture(uBufferTrail,(coord)/iResolution)

const int uKernelRadius = 6;
const float normKernRad = 3.0 / float(uKernelRadius);

const vec2 RESET_ACTION_CELL = vec2(0,0);
const vec2 DRAW_MODE_ACTION_CELL = vec2(1,0);
const vec2 THEME_MODE_ACTION_CELL = vec2(2,0);
const vec2 CURRENT_THEME_CELL = vec2(3,0);

const int KEY_RESET_VIEW   = 32; // Space Key : Reset canvas and animation
const int KEY_DRAW_MODE    = 68; // D key     : switching between auto/free draw mode
const int KEY_THEME_MODE   = 82; // R key     : switching between auto/select theme mode
const int KEY_SWITCH_THEME = 84; // T key     : switch all available themes

const int ThemesCount = 3;
const ColorTheme[] THEMES_ARRAY = ColorTheme[] (
  ColorTheme(
    vec3(0,1,0),
    vec3(0,0,1),
    vec3(1,1,1),
    vec3(0.6,0.6,0),            
    vec3(0.3,0.6,0.8),         
    vec3(0.1,-1.0,0.5),       
    8.0,        
    0.6
  ),
  ColorTheme(
    vec3(1,1,1),
    vec3(0,0.4,1),
    vec3(0.6,0.9,1.0),
    vec3(1.0,0.3,0.1),            
    vec3(0.3,0.6,0.8),         
    vec3(0.1,-1.0,0.5),       
    8.0,        
    0.6       
  ),
  ColorTheme(
    vec3(0,1,0),
    vec3(1,1,1),
    vec3(1,1,1),
    vec3(0.2,1.0,0.2),             
    vec3(0.3,0.6,0.8),        
    vec3(0.1,-1.0,0.5),       
    8.0,        
    0.6       
  )
);

bool isInside (vec2 p, vec2 c) { vec2 d = abs(p-0.5-c) - 0.5; return (-max(d.x,d.y) > 0.0); }

vec2
rotateVector (vec2 vector_, float angleInRadians)
{
  float cosAngle = cos(angleInRadians);
  float sinAngle = sin(angleInRadians);
  return vec2(
    vector_.x * cosAngle - vector_.y * sinAngle,
    vector_.x * sinAngle + vector_.y * cosAngle
  );
}                           

float
getGaussWeight (vec2 p, float k)
{
  return exp(k-dot(p, p));
}

ColorTheme
mixColorTheme (ColorTheme a, ColorTheme b, float ratio)
{
  ColorTheme ret;
  ret.colorX = mix(a.colorX, b.colorX, vec3(ratio));
  ret.colorY = mix(a.colorY, b.colorY, vec3(ratio));
  ret.colorZ = mix(a.colorZ, b.colorZ, vec3(ratio));
  ret.diffuseColor = mix(a.diffuseColor, b.diffuseColor, vec3(ratio));
  ret.specularColor = mix(a.specularColor, b.specularColor, vec3(ratio));
  ret.lightDirection = mix(a.lightDirection, b.lightDirection, vec3(ratio));
  ret.specularPower = mix(a.specularPower, b.specularPower, ratio);
  ret.lightIntensity = mix(a.lightIntensity, b.lightIntensity, ratio);

  return ret;
}

float
getThemeMixingRatio (float time)
{
  float lim = max(float(ThemesCount - 0), 1.0);
  time = (time * uThemeVariationSpeed - lim * 3.14159 / 2.0) / lim;

  return (sin(time) * 0.5 + 0.5) * lim;
}
// glut_1000_common.glsl

uniform vec2 iResolution;
uniform sampler2D iChannel3;

void
main ()
{
  vec2 g = gl_FragCoord.xy;

  gl_FragColor = vec4(0.);
  float sumWeight = 0.0;

  vec4 centerPixel = getTrail(g);

	vec2 uvc = (g * 2.0 - iResolution)/iResolution.y;
	float df = clamp( length(uvc), 0.0, 1.0);

  for(int i=-uKernelRadius; i<=uKernelRadius; ++i)
    for(int j=-uKernelRadius; j<=uKernelRadius; ++j)
    {
      vec2 p = vec2(i,j);
      vec2 off = p * normKernRad;

      float len = length(p);
      vec2 dir = vec2(0);
      if(len != 0.0)
        dir = p / len;

      float weight = getGaussWeight(off, 0.0);
      sumWeight += weight;

      vec4 neighborPixel = getTrail(g + p) - centerPixel;
      gl_FragColor.x += weight * dot(neighborPixel.xy, dir);

      float rotationalForce = weight * dot(neighborPixel.xy, rotateVector(dir, uRotationAngle));
      gl_FragColor.y += sqrt(rotationalForce * rotationalForce * uRotationIntensity);

      gl_FragColor.xy += (gl_FragColor.x * 0.1 + gl_FragColor.y * 0.5) * dir * weight;
    }

  gl_FragColor /= sumWeight;
  gl_FragColor = clamp(gl_FragColor, -1.0, 1.0);
}
