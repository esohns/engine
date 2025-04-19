#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

float time = 0.0;

vec3
rotate (vec3 r, float v) { return vec3(r.x*cos(v)+r.z*sin(v),r.y,r.z*cos(v)-r.x*sin(v)); }

float
noise (vec3 x)
{
  float  z = x.z*64.0;
  vec2 offz = vec2(0.217,0.123);
  vec2 uv1 = x.xy + offz*floor(z); 
  vec2 uv2 = uv1  + offz;

  return mix(texture( iChannel0, uv1 ,-1000.0).x,texture( iChannel0, uv2 ,-1000.0).x,fract(z))-0.5;
}

float
noises (vec3 p)
{
  float a = 0.0;
  for(float i=1.0;i<4.0;i++)
  {
    a += noise(p)/i;
    p = p*2.5 + vec3(i*a*0.02,a*0.01,a*0.1);
  }

  return a;
}

float
ground (vec3 p)
{
  return noise(vec3(p.x,p.y,p.z)*0.00002)*1000.0-noise(p.zxy*0.00013)*80.0+p.y;
}

float
clouds (vec3 p)
{
  return noises(vec3(p.x*0.2+time*10.0,p.y*2.0,p.z)*0.00004);
}

void
main ()
{
  time        = iTime*4.8+floor(iTime*0.1)*120.0+210.0;

  vec2 uv     = gl_FragCoord.xy/(iResolution.xx*0.5)-vec2(1.0,iResolution.y/iResolution.x);
  vec3 campos = vec3(80.0,400.0,time*3.0);
  vec3 ray   = rotate(normalize(vec3(uv.x,uv.y-sin(time*0.02)*0.2-0.2,1.0).xyz),time*0.01);
  vec3 pos    = campos+ray*1.0;
  vec3 sun    = vec3(0.0,0.5,-0.5);    	

  float test  = 0.0;
  float fog   = 0.0;
  float dist  = 0.0;

  vec3  p1 = pos;	
  for(float i=1.0;i<60.0;i++)
  {
    test  = ground(p1);
    fog  += max(clouds(p1)+test*0.0003,0.0);
    p1   += ray*min(test,i*1.3);
    dist += test;
  }

  float l     = sin(dot(ray,sun));
  vec3  light = vec3(l,0.0,-l)+ray.y*0.2;

  float amb = smoothstep(-50.0,80.0,ground(p1+vec3(0.0,10.0,0.0)+sun*30.0))-smoothstep(200.0,-0.0,p1.y-100.0);
  vec3  ground = vec3(0.30,0.30,0.25)+sin(p1*0.002)*0.01+noise(vec3(p1*vec3(0.001,0.02,0.005)))*-0.04+amb*0.2+light*0.05;

  float h = smoothstep(4000.,25000.0,dist);
  vec3  sky = vec3(0.6)+light*0.1+ray.y*0.3;

  float f = smoothstep(1.0,30.0,fog);
  vec3  cloud = vec3(0.95,0.94,0.90)+light*0.1;

  gl_FragColor = vec4(sqrt(smoothstep(0.1,1.0,mix(mix(ground,sky,h),cloud,f))),1.0);
}
