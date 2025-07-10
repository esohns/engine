#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

const float tau = 6.28318530717958647692;

#define GAMMA (2.2)

vec3
ToLinear (vec3 col)
{
  return pow( col, vec3(GAMMA) );
}

vec3
ToGamma (vec3 col)
{
  return pow( col, vec3(1.0/GAMMA) );
}

vec4
Noise (ivec2 x)
{
  return texture( iChannel0, (vec2(x)+0.5)/256.0, -100.0 );
}

vec4
Rand (int x)
{
  vec2 uv;
  uv.x = (float(x)+0.5)/256.0;
  uv.y = (floor(uv.x)+0.5)/256.0;
  return texture( iChannel0, uv, -100.0 );
}

void
main ()
{
  vec3 ray;
  ray.xy = 2.0*(gl_FragCoord.xy-iResolution*.5)/iResolution.x;
  ray.z = 1.0;

  float offset = iTime*.5;	
  float speed2 = (cos(offset)+1.0)*2.0;
  float speed = speed2+.1;
  offset += sin(offset)*.96;
  offset *= 2.0;
  
  vec3 col = vec3(0);

  vec3 stp = ray/max(abs(ray.x),abs(ray.y));

  vec3 pos = 2.0*stp+.5;
  for ( int i=0; i < 20; i++ )
  {
    float z = Noise(ivec2(pos.xy)).x;
    z = fract(z-offset);
    float d = 50.0*z-pos.z;
    float w = pow(max(0.0,1.0-8.0*length(fract(pos.xy)-.5)),2.0);
    vec3 c = max(vec3(0),vec3(1.0-abs(d+speed2*.5)/speed,1.0-abs(d)/speed,1.0-abs(d-speed2*.5)/speed));
    col += 1.5*(1.0-z)*c*w;
    pos += stp;
  }

  gl_FragColor = vec4(ToGamma(col),1.0);
}
