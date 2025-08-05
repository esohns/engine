#version 130

// glut_977_common.glsl
float
hash (float x)
{
  return fract(x * 0.9125 + sin(x * 234.1412) * 231.612 + cos(x * 9151.524) * 12451.512);
}

float
hash (vec3 x)
{
  float hz = hash(x.z * 6.1212);

  return hash(hash(x.x * 0.92342 + hz * 23.612) + hash(x.y * 12.2352 + hz * 125.523));
}

const float a_ = 0.9;
const float blend = 0.01;
// glut_977_common.glsl

uniform vec2 iResolution;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

const float plm_ = 0.03;

//#define explosion

void
main ()
{
  ivec2 fc = ivec2(gl_FragCoord.xy);
  
  if (length(gl_FragCoord.xy - iMouse.xy) < 5. && iMouse.z > 0.0)
  {
    gl_FragColor = vec4(10.0, 0., 0., 0.);
    return;
  }
  
  if (iFrame < 10)
  {
    if (length(gl_FragCoord.xy - 50.) < 5.)
    {
      gl_FragColor = vec4(10.);
      return;
    }
  }

  float m = min(min(gl_FragCoord.x, gl_FragCoord.y), min(iResolution.x - gl_FragCoord.x, iResolution.y - gl_FragCoord.y));

  float plm = clamp(pow(m*plm_, 0.1), 0.0, 1.0);

  //vec2 p = (gl_FragCoord.xy - iResolution * 0.5) / iResolution.y * 20.;
  float s = 1.0;// 1. / dot(p, p);
  float speed = clamp(s, 0.0, 1.0); //clamp(dot(p, p) * 0.01, 0.0, 1.0);
  
  if (length(gl_FragCoord.xy - iResolution * 0.5) < iResolution.y * 0.25)
  {
#ifdef explosion
    speed = mix(0.5, 0.1, hash(vec3(gl_FragCoord.xy, iFrame)));
#else
    speed = 0.5;
#endif
    plm *= 1.0;
  }

  vec4 old = texelFetch(iChannel0, fc, 0);

  float a = texelFetch(iChannel0, ivec2(fc.x-1, fc.y), 0).x;
  float c = texelFetch(iChannel0, ivec2(fc.x+1, fc.y), 0).x;
  float ddx = 0.5*(c+a) - old.x;

  a = texelFetch(iChannel0, ivec2(fc.x, fc.y-1), 0).x;
  c = texelFetch(iChannel0, ivec2(fc.x, fc.y+1), 0).x;
  float ddy = 0.5*(c+a) - old.x;
  float acc = (ddy+ddx)*speed;

  gl_FragColor = vec4(old.x + old.y + acc, old.y + acc, mix(acc, old.z, 0.98), 0.)*plm;
}
