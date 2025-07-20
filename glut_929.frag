#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define NUM_LAYERS (7)
#define LAYER_SEPERATION_FACTOR (0.041)
#define ZOOM_FACTOR_PERIOD (40.0)
#define ZOOM_FACTOR_MIN (0.5)
#define ZOOM_FACTOR_MAX  (2.8)
#define SCROLL_SPEED_AT_MIN_ZOOM (4.0)
#define SCROLL_SPEED_AT_MAX_ZOOM (12.000001)
#define ROTATION_MATRIX_MAX_SKEW (0.4)
#define ROTATION_MATRIX_SKEW_PERIOD (7.4)

//#define CRT_FILTER_ON
//#define CRT_VIGNETTE_ON
//#define CRT_EXTRA_NOISE_ON

#define TWO_PI (6.283185307179586476925286766559)
#define LAYER_STEP_SIZE (1.0/float(NUM_LAYERS))

float
Hash_From2D (vec2 Vec)
{
  float f = Vec.x + Vec.y * 37.0;
  return fract(sin(f)*104003.9);
}
 
float
OscilateSinScalar (float Min, float Max, float Period)
{
  return (Max-Min) * (sin(iTime*TWO_PI/Period)*0.5+0.5) + Min;
}

float
GetInterpolant (float Min, float Max, float CurrentValue)
{
  return (CurrentValue-Min) / (Max-Min);
}

mat2
ZRotate_Skewed (float Angle)
{
  float Skew = 1.0 - OscilateSinScalar(0.0, ROTATION_MATRIX_MAX_SKEW, ROTATION_MATRIX_SKEW_PERIOD);

  Angle = cos(Angle*0.1)*cos(Angle*0.7)*cos(Angle*0.73)*2.0;    

  return mat2(sin(Angle * Skew),cos(Angle),-cos(Angle * Skew),sin(Angle));
}

vec4
SampleMaterial (vec2 uv)
{
  float t = iTime * 0.5;

  float Sample0 = texture(iChannel0, uv * 0.1 ).b;
  Sample0 -= 0.5 + sin(t + sin(uv.x) + sin(uv.y)) * 0.7; 
  Sample0 *= 1.6;
  Sample0 = abs(Sample0);
  Sample0 = 1.0/(Sample0*10.0+1.0);

  vec4 Colour = vec4(Sample0) * texture(iChannel0, uv * 0.05);

  return Colour * texture(iChannel1,(uv + (iTime*1.3)) * 0.001735);
}

float
scanline (vec2 uv, float TexHeight) 
{
  return sin(iResolution.y * uv.y * 0.7 - iTime * 10.0);
}

float
slowscan (vec2 uv, float TexHeight) 
{
  return sin(iResolution.y * uv.y * 0.02 + iTime * 6.0);
}

vec2
crt_bend_uv_coords (vec2 coord, float bend)
{
  coord.x *= 1.0 + pow((abs(coord.y) / bend), 2.0);
  coord.y *= 1.0 + pow((abs(coord.x) / bend), 2.0);

  coord  = (coord / 1.0) + 0.5;

  return coord;
}

vec2
scandistort (vec2 uv) 
{
  float scan1 = clamp(cos(uv.y * 2.0 + iTime), 0.0, 1.0);
  float scan2 = clamp(cos(uv.y * 2.0 + iTime + 4.0) * 10.0, 0.0, 1.0);
  float amount = scan1 * scan2 * uv.x; 
  uv.x -= 0.05 * mix( texture(iChannel1, vec2(uv.x, amount)).x * amount, amount, 0.9 );

  return uv;
}

vec3
CRT_Vignette (vec3 Colour, vec2 uv)
{
  uv.x /= iResolution.x / iResolution.y;
  float Vignette = clamp(pow(cos(uv.x * 3.1415), 1.3) * pow(cos(uv.y * 3.1415), 1.3) * 50.0, 0.0, 1.0);
  Colour *= Vignette;

  return Colour;    
}

vec3
CRT_Filter (vec3 Colour, vec2 uv)
{
#ifdef CRT_EXTRA_NOISE_ON    
  Colour.r += Hash_From2D(uv * iTime * 911.911 * 4.0) * 0.19;
  Colour.g += Hash_From2D(uv * iTime * 563.577 * 4.0) * 0.19;
  Colour.b += Hash_From2D(uv * iTime * 487.859 * 4.0) * 0.19;
#endif    

  vec2 sd_uv = uv;
  vec2 crt_uv = crt_bend_uv_coords(sd_uv, 2.0);    
  vec3 scanline_Colour;
  vec3 slowscan_Colour;
  scanline_Colour.x = scanline(crt_uv, iResolution.y);
  slowscan_Colour.x = slowscan(crt_uv, iResolution.y);
  scanline_Colour.y = scanline_Colour.z = scanline_Colour.x;
  slowscan_Colour.y = slowscan_Colour.z = slowscan_Colour.x;
  Colour = mix(Colour, mix(scanline_Colour, slowscan_Colour, 0.5), 0.04);

#ifdef CRT_VIGNETTE_ON
  Colour = CRT_Vignette(Colour, uv);
#endif     

  return Colour;
}

vec3
PostProcessColour (vec3 Colour, vec2 uv)
{
  Colour -= vec3(length(uv*0.1));

  Colour += Hash_From2D(uv*iTime*0.01)*0.02;

#ifdef CRT_FILTER_ON
  Colour = CRT_Filter(Colour, uv);
#endif 
    
  float Brightness = length(Colour);

  Colour = mix(Colour, vec3(Brightness), Brightness - 0.5);

  return Colour;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution - 0.5;
  uv.x *= iResolution.x / iResolution.y;

  vec3 Colour = vec3(0.0, 0.0, 0.0);

  float ScaleValue = OscilateSinScalar(ZOOM_FACTOR_MIN, ZOOM_FACTOR_MAX, ZOOM_FACTOR_PERIOD);

  float ScrollInterpolant = GetInterpolant(ZOOM_FACTOR_MIN, ZOOM_FACTOR_MAX, ScaleValue);

  float ScrollValue = mix(SCROLL_SPEED_AT_MIN_ZOOM, SCROLL_SPEED_AT_MAX_ZOOM, ScrollInterpolant); 

  for (float i = 0.0; i < 1.0; i += LAYER_STEP_SIZE)
  {
    vec2 uv2 = uv;
    uv2 *= ZRotate_Skewed(iTime * i*i * 12.0 * LAYER_SEPERATION_FACTOR);
    uv2 *= ScaleValue * (i*i+1.0); 
    uv2.xy += ScrollValue + iTime*0.125;        

    Colour += SampleMaterial(uv2).xyz*LAYER_STEP_SIZE*3.5;
  }

  Colour = PostProcessColour(Colour, uv);

  gl_FragColor = vec4(Colour, 1.0);
}
