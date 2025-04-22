#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define Pi 3.1415927
vec2 m; // mouse

float
hash (vec2 uv)
{
  return texture(iChannel0,.5*m.y*uv*iResolution.y/vec2(textureSize(iChannel0,0))).r;
}

vec3
hashRGB (vec2 uv)
{
  return texture(iChannel0,.5*m.y*uv*iResolution.y/vec2(textureSize(iChannel0,0))).rgb;
}

mat2 rot (float a) { return mat2(cos(a), -sin(a), sin(a), cos(a)); }

int mod2 (int i) { return i-2*(i/2); }

bool
drawCircle (inout vec3 v,vec2 uv, vec2 pt, vec2 ofs, float r, float R, float a, int mode)
{
  vec2 newUV = pt - uv;

  float d = (mod2(mode)==0) ? length(newUV)/R : max(abs(newUV.x),abs(newUV.y))/R;
  if ((d<1.)&&(d>r/R))
  {
    vec3 noiseT = hashRGB(rot(a)*(newUV-ofs));
#if 1
    v = noiseT;
#else
    mode /= 2;
    if (mode==0) v.rg = noiseT.rg; 
    if (mode==1) v.gb = noiseT.gb; 
    if (mode==2) v.br = noiseT.br; 
#endif
    return true;
  }

  return false;
}

void
main ()
{
  float t = .05*iTime;
  m = iMouse.xy/ iResolution.y;
  if ((length(m)==0.) || (iMouse.z==0.))
    m = vec2(.9,.9);

  vec2 uv = 2.*(gl_FragCoord.xy / iResolution.y - vec2(.8,.5));
  vec3 v = hashRGB(uv);

  for(float i=0.; i<150.; i++)
  {
    vec2 p = vec2( 1.6*sin(10.*t+2.17*i)+.8*sin(5.*t-7.*i),
                   cos(7.345*t-3.*i)+.7*cos(6.123*t+4.*i) );
    float R = .2/(1.-.65*sin(.3*t-i)),
          r = R*(1.+sin(17.*i))/2.,
          a = 55.*sin(22.*i)*t;
    if (drawCircle(v,uv, p, vec2(0.,0.), r,R, a, int(6.*(1.+cos(13.*i))/2.))) 
      break;
  }

  vec3 col = vec3(v); 
  gl_FragColor = vec4(col,1.0);
}
