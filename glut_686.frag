#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define PI 3.14159265
#define FAR 60.

#define SMOOTHING 

const mat2 rM = mat2 (.7071, .7071, -.7071, .7071);

mat2 rot2 (float a) { vec2 v = sin(vec2(1.570796, 0) + a);	return mat2(v, -v.y, v.x); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max(abs(n), 0.001);//n = max((abs(n) - 0.2)*7., 0.001); //  etc.
  n /= (n.x + n.y + n.z ); 
  p = (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
  return p*p;
}

float
smaxP (float a, float b, float s)
{
  float h = clamp( 0.5 + 0.5*(a-b)/s, 0., 1.);
  return mix(b, a, h) + h*(1.0-h)*s;
}

vec2
sminP (vec2 a, vec2 b , float s)
{
  vec2 h = clamp( 0.5 + 0.5*(b-a)/s, 0. , 1.);
  return mix(b, a, h) - h*(1.0-h)*s;
}

float
sminP (float a, float b , float s)
{
  float h = clamp( 0.5 + 0.5*(b-a)/s, 0. , 1.);
  return mix(b, a, h) - h*(1.0-h)*s;
}

float
drawObject (vec3 p)
{
  p = fract(p)-.5;
  return dot(p, p);
}

float
cellTile (vec3 p)
{
  vec4 v, d; 

  d.x = drawObject(p - vec3(.81, .62, .53));
  p.xy *= rM;
  d.y = drawObject(p - vec3(.6, .82, .64));
  p.yz *= rM;
  d.z = drawObject(p - vec3(.51, .06, .70));
  p.zx *= rM;
  d.w = drawObject(p - vec3(.12, .62, .64));

#ifdef SMOOTHING
  v.xy = sminP(d.xz, d.yw, .05); 
#else
  v.xy = min(d.xz, d.yw);
#endif

#ifdef SMOOTHING
  return  sminP(v.x, v.y, .05)*2.5; 
#else
return  min(v.x, v.y)*2.5; 
#endif
}

vec2
path (float z)
{
  float a = sin(z * 0.11);
  float b = cos(z * 0.14);
  return vec2(a*4. -b*1.5, b*1.7 + a*1.5); 
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p); //p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}

float
map (vec3 p)
{
  p.xy -= path(p.z);

  p/=2.;

  vec3 q = p + (cos(p*2.52 - sin(p.zxy*3.5)))*.2;
  float sf = max(cellTile(q/5.), 0.); 

  p += (cos(p*.945 + sin(p.zxy*2.625)))*.2;
#ifdef SMOOTHING
  float t = .1 - abs(p.x*.05) - abs(p.y);
#else
  float t = .05 - abs(p.x*.05) - abs(p.y);
#endif  

  float n = smaxP(t, (.68 - (1.-sqrt(sf)))*2., 1.);// + abs(p.x*p.y*p.z)*.05;

  return n*3.;
}

float
bumpSurf3D (vec3 p, vec3 n)
{
  return (cellTile(p/2.))*.8 + (cellTile(p*1.5))*.2;
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0);
  float ref = bumpSurf3D(p, nor);
  vec3 grad = (vec3(bumpSurf3D(p - e.xyy, nor),
                    bumpSurf3D(p - e.yxy, nor),
                    bumpSurf3D(p - e.yyx, nor) )-ref)/e.x;

  grad -= nor*dot(nor, grad);          

  return normalize( nor + grad*bumpfactor );
}

vec3
doBumpMap (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.001, 0);

  mat3 m = mat3( tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m;
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf ); // Bumped normal. "bf" - bump factor.
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, h;
  for(int i = 0; i < 128; i++)
  {
    h = map(ro+rd*t);
    if(abs(h)<0.0025*(t*.125 + 1.) || t>FAR)
      break;
    t += h*.8;
  }

  return min(t, FAR);
}

float
calculateAO (vec3 p, vec3 n)
{
  float ao = 0.0, l;
  const float maxDist = 2.;
  const float nbIte = 6.0;
  for( float i=1.; i< nbIte+.5; i++ )
  {
    l = (i*.75 + fract(cos(i)*45758.5453)*.25)/nbIte*maxDist;

    ao += (l - map( p + n*l ))/(1.+ l);// / pow(1.+l, falloff);
  }

  return clamp(1.- ao/nbIte, 0., 1.);
}

vec3
calcNormal (vec3 p)
{
  vec2 e = vec2(0.0025, -0.0025); 
  return normalize(e.xyy * map(p + e.xyy) + e.yyx * map(p + e.yyx) + e.yxy * map(p + e.yxy) + e.xxx * map(p + e.xxx));
}

float
shadows (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.0;
  const int shadIter = 24; 

  float dist = start;

  for (int i=0; i<shadIter; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, k*h/dist);

    dist += clamp(h, 0.02, 0.2);

    if ((h)<0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + 0.2, 1.0); 
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  vec3 lookAt = vec3(0, 0, iTime*8. + 0.1);
  vec3 camPos = lookAt + vec3(0.0, 0.0, -0.1);

  vec3 lightPos = camPos + vec3(0, 7, 35.);

  lookAt.xy += path(lookAt.z);
  camPos.xy += path(camPos.z);

  float FOV = PI/3.;
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

  rd.xy = rot2( path(lookAt.z).x/16. )*rd.xy;

  vec2 ms = vec2(0);
  if (iMouse.z != 0.0) ms = (2.*iMouse.xy - iResolution.xy)/iResolution.xy;
  vec2 a = sin(vec2(1.5707963, 0) - ms.x); 
  mat2 rM = mat2(a, -a.y, a.x);
  rd.xz = rd.xz*rM; 
  a = sin(vec2(1.5707963, 0) - ms.y); 
  rM = mat2(a, -a.y, a.x);
  rd.yz = rd.yz*rM;

  float t = trace(camPos, rd);    

  vec3 sceneCol = vec3(0);
  if(t<FAR)
  {
    vec3 sp = camPos + rd*t;

    vec3 sn = calcNormal(sp);

    vec3 snNoBump = sn;

    const float tSize0 = 1./2.;
    sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.1);
    vec3 tsp =  sp;// + vec3(0, 0, iTime/8.);// + vec3(path(sp.z), 0.)

    sn = doBumpMap(tsp, sn, .5);

    float ao = calculateAO(sp, sn);//*.75 + .25;

    vec3 ld = lightPos-sp;

    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float atten = 1./(1. + lDist*.007);

    float ambience = 0.25;
    float diff = max( dot(sn, ld), 0.0);
    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.);
    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 1.);

    vec3 texCol = vec3(.8, .9, 1.);
    texCol *= min(tex3D(iChannel0, sp*tSize0, sn)*3.5, 1.);
    texCol *= bumpSurf3D(sp, sn)*.5 + .5;

    float shading = shadows(sp + sn*.005, ld, .05, lDist, 8.);

    sceneCol = texCol*(diff + spec + ambience);

    sceneCol += texCol*vec3(.8, .95, 1)*pow(fre, 1.)*.5;
    sceneCol *= atten*shading*ao;
  }

  vec3 fog = vec3(.6, .8, 1)/2.*(rd.y*.5 + .5);    
  sceneCol = mix(sceneCol, fog, smoothstep(0., .95, t/FAR));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
