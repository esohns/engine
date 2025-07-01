#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define PI 3.1415926535898
#define FH 1.0

float getGrey (vec3 p) { return p.x*0.299 + p.y*0.587 + p.z*0.114; }

vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(7, 157, 113)));    
  return fract(vec3(2097152, 262144, 32768)*n); 
}

mat2
rot2 (float a)
{
  float c = cos(a); float s = sin(a);
  return mat2(c, s, -s, c);
}

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max((abs(n) - 0.2)*7., 0.001);
  n /= (n.x + n.y + n.z );

  return (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
}

vec3 tri (vec3 x) { return abs(x-floor(x)-.5); }

float
surfFunc (vec3 p)
{
  return dot(tri(p*0.5 + tri(p*0.25).yzx), vec3(0.666));
}

vec2 path (float z) { float s = sin(z/24.)*cos(z/12.); return vec2(s*12., 0.); }

float
map (vec3 p)
{
  float sf = surfFunc(p - vec3(0, cos(p.z/3.)*.15, 0));
  vec2 tun = abs(p.xy - path(p.z))*vec2(0.5, 0.7071);
  float n = 1. - max(tun.x, tun.y) + (0.5 - sf);
  return min(n, p.y + FH);
}

vec3
doBumpMap (sampler2D tex, vec3 p, vec3 nor, float bumpfactor)
{
  const float eps = 0.001;
  float ref = getGrey(tex3D(tex,  p , nor));                 
  vec3 grad = vec3(getGrey(tex3D(tex, vec3(p.x - eps, p.y, p.z), nor)) - ref,
                   getGrey(tex3D(tex, vec3(p.x, p.y - eps, p.z), nor)) - ref,
                   getGrey(tex3D(tex, vec3(p.x, p.y, p.z - eps), nor)) - ref )/eps;

  grad -= nor*dot(nor, grad);          

  return normalize( nor + grad*bumpfactor );
}

vec3
getNormal (vec3 p)
{
  const float eps = 0.001;
  return normalize(vec3(
    map(vec3(p.x + eps, p.y, p.z)) - map(vec3(p.x - eps, p.y, p.z)),
    map(vec3(p.x, p.y + eps, p.z)) - map(vec3(p.x, p.y - eps, p.z)),
    map(vec3(p.x, p.y, p.z + eps)) - map(vec3(p.x, p.y, p.z - eps))
  ));
}

float
calculateAO (vec3 p, vec3 n)
{
  const float AO_SAMPLES = 5.0;
  float r = 0.0, w = 1.0, d;

  for (float i = 1.0; i<AO_SAMPLES + 1.1; i++)
  {
    d = i/AO_SAMPLES;
    r += w*(d - map(p + n*d));
    w *= 0.5;
  }

  return 1.0 - clamp(r, 0.0, 1.0);
}

float
curve (vec3 p, float w)
{
  vec2 e = vec2(-1., 1.)*w;

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return 0.125/(w*w) *(t1 + t2 + t3 + t4 - 4.*map(p));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  vec3 camPos = vec3(0.0, 0.0, iTime*5.);
  vec3 lookAt = camPos + vec3(0.0, 0.1, 0.5);

  vec3 light_pos = camPos + vec3(0.0, 0.125, -0.125);
  vec3 light_pos2 = camPos + vec3(0.0, 0.0, 6.0);

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);
  light_pos.xy += path(light_pos.z);
  light_pos2.xy += path(light_pos2.z);

  float FOV = PI/3.;
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

  rd.xy = rot2( path(lookAt.z).x/32. )*rd.xy;

  float t = 0.0, dt;
  for(int i=0; i<128; i++)
  {
    dt = map(camPos + rd*t);
    if(dt<0.005 || t>150.)
      break;
    t += dt*0.75;
  }

  vec3 sceneCol = vec3(0.);

  if(dt<0.005)
  {
    vec3 sp = t * rd+camPos;
    vec3 sn = getNormal(sp);

    const float tSize0 = 1./1.; 
    const float tSize1 = 1./4.;

    if (sp.y<-(FH-0.005)) sn = doBumpMap(iChannel1, sp*tSize1, sn, 0.025);
    else sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.025);

    float ao = calculateAO(sp, sn);

    vec3 ld = light_pos-sp;
    vec3 ld2 = light_pos2-sp;

    float distlpsp = max(length(ld), 0.001);
    float distlpsp2 = max(length(ld2), 0.001);

    ld /= distlpsp;
    ld2 /= distlpsp2;

    float atten = min(1./(distlpsp) + 1./(distlpsp2), 1.);

    float ambience = 0.25;

    float diff = max( dot(sn, ld), 0.0);
    float diff2 = max( dot(sn, ld2), 0.0);

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 8.);
    float spec2 = pow(max( dot( reflect(-ld2, sn), -rd ), 0.0 ), 8.);

    float crv = clamp(curve(sp, 0.125)*0.5 + 0.5, .0, 1.);

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 1.);

    vec3 texCol;
    if (sp.y<-(FH - 0.005)) texCol = tex3D(iChannel1, sp*tSize1, sn);
    else texCol = tex3D(iChannel0, sp*tSize0, sn);

    float shading =  crv*0.5 + 0.5; 

    sceneCol = getGrey(texCol)*((diff + diff2)*0.75 + ambience*0.25) + (spec + spec2)*texCol*2. + fre*crv*texCol.zyx*2.;

    sceneCol *= atten*shading*ao;

    sceneCol *= clamp(1.-abs(curve(sp, 0.0125)), .0, 1.);        
  }

  gl_FragColor = vec4(clamp(sceneCol, 0., 1.), 1.0);
}
