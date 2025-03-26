#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define DETAIL

#define PI 3.1415926535898

const float freqA = .15;
const float freqB = .25;
const float ampA = 2.4;
const float ampB = 1.7;

float getGrey (vec3 p) { return p.x*0.299 + p.y*0.587 + p.z*0.114; }

vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(7, 157, 113)));
  return fract(vec3(2097152, 262144, 32768)*n);
}

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max((abs(n) - .2)*7., .001);
  n /= (n.x + n.y + n.z );  

  p = (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;

  return p*p;
}

vec3 tri (vec3 x) { return abs(x-floor(x)-.5); }
vec3 triSmooth (vec3 x) { return cos(x*6.2831853)*0.25+0.25; }

#ifdef DETAIL
float surfFuncL (vec3 p)
#else
float surfFunc (vec3 p)
#endif
{
  float n = dot(tri(p*0.48 + tri(p*0.24).yzx), vec3(0.444));
  p.xz = vec2(p.x + p.z, p.z - p.x) * 0.7071;
  return dot(tri(p*0.72 + tri(p*0.36).yzx), vec3(0.222)) + n;
}

#ifdef DETAIL
float
surfFunc (vec3 p)
{
  return mix(surfFuncL(p/1.5), surfFuncL(p.yzx*3.), 1./8.);
}
#endif

float
smoothMinP (float a, float b, float smoothing)
{
  float h = clamp((b-a)*0.5/smoothing + 0.5, 0.0, 1.0 );
  return mix(b, a, h) - smoothing*h*(1.0-h);
}

vec2 path (float z) { return vec2(ampA*sin(z * freqA), ampB*cos(z * freqB)); }
vec2 path2 (float z) { return vec2(ampB*sin(z * freqB*1.5), ampA*cos(z * freqA*1.3)); }

float
map (vec3 p)
{
  vec2 tun = p.xy - path(p.z);
  vec2 tun2 = p.xy - path2(p.z);
#ifdef DETAIL
  const float sfF = 1.35;
#else
  const float sfF = 1.;
#endif
  return 1.- smoothMinP(length(tun), length(tun2), 4.) + (0.5-surfFunc(p))*sfF;
}

vec3
doBumpMap (sampler2D tex, in vec3 p, vec3 nor, float bumpfactor)
{
  const float eps = 0.001;
  float ref = getGrey(tex3D(tex,  p , nor));
  vec3 grad = vec3( getGrey(tex3D(tex, vec3(p.x-eps, p.y, p.z), nor))-ref,
                    getGrey(tex3D(tex, vec3(p.x, p.y-eps, p.z), nor))-ref,
                    getGrey(tex3D(tex, vec3(p.x, p.y, p.z-eps), nor))-ref )/eps;

  grad -= nor*dot(nor, grad);

  return normalize( nor + grad*bumpfactor );
}

vec3
getNormal (vec3 p)
{
  const float eps = 0.001;
  return normalize(vec3(
    map(vec3(p.x+eps,p.y,p.z))-map(vec3(p.x-eps,p.y,p.z)),
    map(vec3(p.x,p.y+eps,p.z))-map(vec3(p.x,p.y-eps,p.z)),
    map(vec3(p.x,p.y,p.z+eps))-map(vec3(p.x,p.y,p.z-eps))
  ));
}

float
calculateAO (vec3 p, vec3 n)
{
  const float AO_SAMPLES = 5.0;
  float r = 0.0, w = 1.0, d;

  for (float i= 1.; i<AO_SAMPLES + 1.1; i++)
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

  vec3 lookAt = vec3(0.0, 0.0, iTime*4.);  // "Look At" position.
  vec3 camPos = lookAt + vec3(0.0, 0.0, -0.1); // Camera position, doubling as the ray origin.

  vec3 light_pos = camPos + vec3(0, .125, -.25);// Put it a bit in front of the camera.
  vec3 light_pos2 = camPos + vec3(0, 0, 6);// Put it a bit in front of the camera.

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);
  light_pos.xy += path(light_pos.z);
  light_pos2.xy += path(light_pos2.z);

  float FOV = PI/3.; // FOV - Field of view.
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

  float t = 0., dt;
  for(int i = 0; i<128; i++)
  {
    dt = map(camPos + rd*t);
    if(dt<.0025 || t>150.){ break; } 
    t += dt*.75;
  }

  vec3 sceneCol = vec3(0.);

  if(dt<.0025)
  {
    vec3 sp = t * rd+camPos;
    vec3 sn = getNormal(sp);

    const float tSize0 = 1./3.; 

    sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.06);

    float ao = calculateAO(sp, sn);

    vec3 ld = light_pos-sp;
    vec3 ld2 = light_pos2-sp;

    float distlpsp = max(length(ld), 0.001);
    float distlpsp2 = max(length(ld2), 0.001);

    ld /= distlpsp;
    ld2 /= distlpsp2;

    float atten = 1./(1. + distlpsp*.3 + distlpsp*distlpsp*.05);
    float atten2 =  1./(1. + distlpsp2*.3 + distlpsp2*distlpsp2*.05);

    float ambience = 0.125;

    float diff = max( dot(sn, ld), 0.0);
    float diff2 = max( dot(sn, ld2), 0.0);

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 8.);
    float spec2 = pow(max( dot( reflect(-ld2, sn), -rd ), 0.0 ), 8.);

    float crv = clamp(curve(sp, 0.35)*0.75 + 0.5, .0, 1.);

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 1.);

    vec3 texCol = tex3D(iChannel0, sp*tSize0, sn);
    texCol *= mix(vec3(.8, 1, .6), vec3(1.2, 1, .8)*2., crv);

    float shading = crv*0.5 + 0.5; 

    sceneCol += (texCol*(diff*vec3(1.0, 0.96, 0.92) + ambience) + spec + fre*texCol*.125)*atten;
    sceneCol += (texCol*(diff2*vec3(1.0, 0.96, 0.92) + ambience) + spec2 + fre*texCol*.125)*atten2;

    sceneCol *= shading*ao;
  }

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
