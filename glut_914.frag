#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;

float coast2water_fadedepth = 0.10;
float large_waveheight      = 0.50;
float large_wavesize        = 4.;
float small_waveheight      = .6;
float small_wavesize        = .5;
float water_softlight_fact  = 15.;
float water_glossylight_fact= 120.;
float particle_amount       = 70.;
vec3 watercolor             = vec3(0.43, 0.60, 0.66);
vec3 watercolor2            = vec3(0.06, 0.07, 0.11);
vec3 water_specularcolor    = vec3(1.3, 1.3, 0.9);
vec3 light;

#define USETEXTUREHEIGHT 0

float
hash (float n)
{
  return fract(sin(n)*43758.5453123);
}

float
noise1 (vec2 x)
{
  vec2 p  = floor(x);
  vec2 f  = smoothstep(0.0, 1.0, fract(x));
  float n = p.x + p.y*57.0;
  return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x), mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
}

float
noise (vec2 p)
{
  vec2 texSize = vec2(textureSize(iChannel0, 0)); // assume texture is square
  return textureLod(iChannel0, p * vec2(1. / texSize.x), 0.0).x;
}

float
height_map (vec2 p)
{
#if USETEXTUREHEIGHT
  float f = 0.15+textureLod(iChannel2, p*0.6, 0.0).r*2.;
#else
  mat2 m = mat2( 0.9563*1.4,  -0.2924*1.4,  0.2924*1.4,  0.9563*1.4 );
  p = p*6.;
  float f = 0.6000*noise1( p ); p = m*p*1.1;
  f += 0.2500*noise1( p ); p = m*p*1.32;
  f += 0.1666*noise1( p ); p = m*p*1.11;
  f += 0.0834*noise( p ); p = m*p*1.12;
  f += 0.0634*noise( p ); p = m*p*1.13;
  f += 0.0444*noise( p ); p = m*p*1.14;
  f += 0.0274*noise( p ); p = m*p*1.15;
  f += 0.0134*noise( p ); p = m*p*1.16;
  f += 0.0104*noise( p ); p = m*p*1.17;
  f += 0.0084*noise( p );
  const float FLAT_LEVEL = 0.525;
  if (f<FLAT_LEVEL)
    f = f;
  else
    f = pow((f-FLAT_LEVEL)/(1.-FLAT_LEVEL), 2.)*(1.-FLAT_LEVEL)*2.0+FLAT_LEVEL;
#endif
  return clamp(f, 0., 10.);
}

vec3
terrain_map (vec2 p)
{
  return vec3(0.7, .55, .4)+texture(iChannel1, p*2.).rgb*.5;
}

const mat2 m = mat2( 0.72, -1.60,  1.60,  0.72 );

float
water_map (vec2 p, float height)
{
  vec2 p2 = p*large_wavesize;
  vec2 shift1 = 0.001*vec2( iTime*160.0*2.0, iTime*120.0*2.0 );
  vec2 shift2 = 0.001*vec2( iTime*190.0*2.0, -iTime*130.0*2.0 );

  float f = 0.6000*noise( p );
  f += 0.2500*noise( p*m );
  f += 0.1666*noise( p*m*m );
  float wave = sin(p2.x*0.622+p2.y*0.622+shift2.x*4.269)*large_waveheight*f*height*height ;

  p *= small_wavesize;
  f = 0.;
  float amp = 1.0, s = .5;
  for (int i=0; i<9; i++)
  {
    p = m*p*.947;
    f -= amp*abs(sin((noise( p+shift1*s )-.5)*2.));
    amp = amp*.59;
    s*=-1.329;
  }
 
  return wave+f*small_waveheight;
}

float
nautic (vec2 p)
{
  p *= 18.;
  float f = 0.;
  float amp = 1.0, s = .5;
  for (int i=0; i<3; i++)
  {
    p = m*p*1.2;
    f += amp*abs(smoothstep(0., 1., noise( p+iTime*s ))-.5);
    amp = amp*.5;
    s*=-1.227;
  }

  return pow(1.-f, 5.);
}

float
particles (vec2 p)
{
  p *= 200.;
  float f = 0.;
  float amp = 1.0, s = 1.5;
  for (int i=0; i<3; i++)
  {
    p = m*p*1.2;
    f += amp*noise( p+iTime*s );
    amp = amp*.5;
    s*=-1.227;
  }

  return pow(f*.35, 7.)*particle_amount;
}

float
test_shadow (vec2 xy, float height)
{
  vec3 r0 = vec3(xy, height);
  vec3 rd = normalize( light - r0 );
    
  float hit = 1.0;
  float t   = 0.001;
  for (int j=1; j<25; j++)
  {
    vec3 p = r0 + t*rd;
    float h = height_map( p.xy );
    float height_diff = p.z - h;
    if (height_diff<0.0)
      return 0.0;
    t += 0.01+height_diff*.02;
    hit = min(hit, 2.*height_diff/t);
  }

  return hit;
}

vec3
CalcTerrain (vec2 uv, float height)
{
  vec3 col = terrain_map( uv );
  float h1 = height_map(uv-vec2(0., 0.01));
  float h2 = height_map(uv+vec2(0., 0.01));
  float h3 = height_map(uv-vec2(0.01, 0.));
  float h4 = height_map(uv+vec2(0.01, 0.));
  vec3 norm = normalize(vec3(h3-h4, h1-h2, 1.));
  vec3 r0 = vec3(uv, height);
  vec3 rd = normalize( light - r0 );
  float grad = dot(norm, rd);
  col *= grad+pow(grad, 8.);
  float terrainshade = test_shadow( uv, height );
  col = mix(col*.25, col, terrainshade);

  return col;
}

void
main ()
{
  light = vec3(-0., sin(iTime*0.5)*.5 + .35, 2.8);
  vec2 uv = (gl_FragCoord.xy / iResolution - vec2(-0.12, +0.25));

  float WATER_LEVEL = 0.94;
  if (iMouse.z>0.)
    WATER_LEVEL = iMouse.x*.003;
  float deepwater_fadedepth   = 0.5 + coast2water_fadedepth;
  if (iMouse.z>0.)
    deepwater_fadedepth = iMouse.y*0.003 + coast2water_fadedepth;

  float height = height_map( uv );
  vec3 col;

  float waveheight = clamp(WATER_LEVEL*3.-1.5, 0., 1.);
  float level = WATER_LEVEL + .2*water_map(uv*15. + vec2(iTime*.1), waveheight);
  if (height > level)
    col = CalcTerrain(uv, height);
  else
  {
    vec2 dif = vec2(.0, .01);
    vec2 pos = uv*15. + vec2(iTime*.01);
    float h1 = water_map(pos-dif,waveheight);
    float h2 = water_map(pos+dif,waveheight);
    float h3 = water_map(pos-dif.yx,waveheight);
    float h4 = water_map(pos+dif.yx,waveheight);
    vec3 normwater = normalize(vec3(h3-h4, h1-h2, .125));
    uv += normwater.xy*.002*(level-height);

    col = CalcTerrain(uv, height);

    float coastfade = clamp((level-height)/coast2water_fadedepth, 0., 1.);
    float coastfade2= clamp((level-height)/deepwater_fadedepth, 0., 1.);
    float intensity = col.r*.2126+col.g*.7152+col.b*.0722;
    watercolor = mix(watercolor*intensity, watercolor2, smoothstep(0., 1., coastfade2));

    vec3 r0 = vec3(uv, WATER_LEVEL);
    vec3 rd = normalize( light - r0 );
    float grad     = dot(normwater, rd);
    float specular = pow(grad, water_softlight_fact);
    float specular2= pow(grad, water_glossylight_fact);
    float gradpos  = dot(vec3(0., 0., 1.), rd);
    float specular1= smoothstep(0., 1., pow(gradpos, 5.));
    float watershade  = test_shadow( uv, level );
    watercolor *= 2.2+watershade;
    watercolor += (.2+.8*watershade) * ((grad-1.0)*.5+specular) * .25;
    watercolor /= (1.+specular1*1.25);
    watercolor += watershade*specular2*water_specularcolor;
    watercolor += watershade*coastfade*(1.-coastfade2)*(vec3(.5, .6, .7)*nautic(uv)+vec3(1., 1., 1.)*particles(uv));

    col = mix(col, watercolor, coastfade);
  }

  gl_FragColor = vec4(col , 1.0);
}
