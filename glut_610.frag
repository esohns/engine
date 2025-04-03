#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform samplerCube iChannel0;

#define limited_displace

float dstepf = 0.0;
const vec2 RMPrec = vec2(0.2, 0.05); 
const vec3 DPrec = vec3(1e-3, 12., 1e-8); 
vec2 Params;

vec4
map (vec3 p)
{
  vec4 col = vec4(p,1);
  vec2 i = col.xz*Params.y;
  i=i/col.y+iTime;
  i-=col.xy=ceil(i+=i.x*=.577);
  col.xy+=step(1.,col.z=mod(col.x+col.y,3.))-step(2.,col.z)*step(i,i.yx);
  col.z=0.;
  col=.5+.5*sin(col);

  dstepf += 0.005;

  float disp;

#ifdef limited_displace
  disp = dot(col.rgb,vec3(Params.x));
#else
  disp = dot(col,vec4(Params.x));
#endif

  float dist = length(p) -4. + smoothstep(0., 1., disp);

  return vec4(dist, col.rgb);
}

vec3
nor (vec3 pos, float prec)
{
  vec2 e = vec2( prec, 0. );
  vec3 n = vec3(
  map(pos+e.xyy).x - map(pos-e.xyy).x,
  map(pos+e.yxy).x - map(pos-e.yxy).x,
  map(pos+e.yyx).x - map(pos-e.yyx).x );
  return normalize(n);
}

vec3
cam (vec2 uv, vec3 ro, vec3 cu, vec3 cv)
{
  vec3 rov = normalize(cv-ro);
  vec3 u =  normalize(cross(cu, rov));
  vec3 v =  normalize(cross(rov, u));
  vec3 rd = normalize(rov + u*uv.x + v*uv.y);
  return rd;
}

void
main ()
{
  vec4 f;
  vec2 g = gl_FragCoord.xy;
  vec2 si = iResolution.xy;
  float t = iTime;
  Params.x = sin(t*.5)*.5+.5;
  Params.y = 10.;
  if (iMouse.z>0.) Params = iMouse.xy / si * vec2(1., 100.);
  f = vec4(0.);
  float ca = t*.2;
  float ce = 4.7;
  float cd = 0.5;
  vec3 cu=vec3(0,1,0);
  vec3 cv=vec3(0,0,0);
  float refl_i = .6;
  float refr_a = 1.2;
  float refr_i = .8;
  float bii = 0.6;
  vec2 uv = (g+g-si)/min(si.x, si.y);
  vec3 ro = vec3(sin(ca)*cd, ce+1., cos(ca)*cd); //
  vec3 rd = cam(uv, ro, cu, cv);
  float b = bii;
  vec3 d = vec3(0.);
  vec3 p = ro+rd*d.x;
  float s = DPrec.y;
  float rmd = sign(map(p).x);
  for(int i=0;i<1000;i++)
  {
    if(s<DPrec.x||s>DPrec.y)
      break;
    s = map(p).x*(s>DPrec.x?RMPrec.x:RMPrec.y);
    if (sign(s) != rmd) break;
    d.y = d.x;
    d.x += s * 0.3/1.3;
    p = ro+rd*d.x;
  }

  float countIter = 0.;
  if (sign(s) == rmd)
  {
    p = ro+rd*d.x;
    rmd = map(p).x;
    for (int i = 0; i < 20; i++)
    {
      countIter += 10.;
      d.z = (d.x + d.y)*.5;
      p = ro+rd*d.z;
      s = map(p).x*RMPrec.y;
      d.x += abs(s);
      if (abs(s) < DPrec.z)break;
      (d.x*rmd < 0. )? (d.x = d.z ): (d.y = d.z);
    }
    d.x = (d.x+d.y) * .5;
  }

  f += pow(b,15.);

  if (d.x<DPrec.y)
  {
    float nPrec = 10./countIter;
    vec3 n = nor(p, nPrec);
    vec3 ray = reflect(rd, n);
    f += texture(iChannel0, ray) * refl_i;
    ray = refract(rd, n, refr_a);
    f += texture(iChannel0, rd) * refr_i;
    f.rgb = mix( f.rgb, map(p).yzw,0.5);
  }
  else
  {
    f = texture(iChannel0, rd);
  }

  f *= dstepf;

  gl_FragColor = f;
}
