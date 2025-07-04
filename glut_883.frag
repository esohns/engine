#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define FIRE_REFLECTION

#define sEPS 0.005
#define FAR 20.

float getGrey (vec3 p) { return p.x*0.299 + p.y*0.587 + p.z*0.114; }

float
sminP (float a, float b, float smoothing)
{
  float h = clamp( 0.5+0.5*(b-a)/smoothing, 0.0, 1.0 );
  return mix( b, a, h ) - smoothing*h*(1.0-h);
}

mat2 rot (float th) { float cs = cos(th), si = sin(th); return mat2(cs, -si, si, cs); }

vec3
tex3D (sampler2D tex, vec3 p, vec3 n)
{
  n = max((abs(n) - 0.2)*7., 0.001);
  n /= (n.x + n.y + n.z );

  return (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
}

vec3
blackbodyPalette (float t)
{
  t *= 4000.;

  float cx = (0.860117757 + 1.54118254e-4*t + 1.28641212e-7*t*t)/(1.0 + 8.42420235e-4*t + 7.08145163e-7*t*t);
  float cy = (0.317398726 + 4.22806245e-5*t + 4.20481691e-8*t*t)/(1.0 - 2.89741816e-5*t + 1.61456053e-7*t*t);

  float d = (2.*cx - 8.*cy + 4.);
  vec3 XYZ = vec3(3.*cx/d, 2.*cy/d, 1. - (3.*cx + 2.*cy)/d);

  vec3 RGB = mat3(3.240479, -0.969256, 0.055648, 
                  -1.537150, 1.875992, -0.204043, 
                  -0.498535, 0.041556, 1.057311) * vec3(1./XYZ.y*XYZ.x, 1., 1./XYZ.y*XYZ.z);

  return max(RGB, 0.)*pow(t*0.0004, 4.); 
}

float
bumpSurf3D (vec3 p, vec3 n)
{
  p = abs(mod(p, 0.0625)-0.03125);
  float x = min(p.x,min(p.y,p.z))/0.03125;

  p = sin(p*380.+sin(p.yzx*192.+64.));
  float surfaceNoise = (p.x*p.y*p.z);

  return clamp(x + surfaceNoise*0.05, 0., 1.);
}

vec3
doBumpMap (vec3 p, vec3 nor, float bumpfactor)
{
  const float eps = 0.001;
  float ref = bumpSurf3D(p, nor);                 
  vec3 grad = vec3(bumpSurf3D(vec3(p.x-eps, p.y, p.z), nor)-ref,
                   bumpSurf3D(vec3(p.x, p.y-eps, p.z), nor)-ref,
                   bumpSurf3D(vec3(p.x, p.y, p.z-eps), nor)-ref )/eps;                     

  grad -= nor*dot(nor, grad);          

  return normalize( nor + bumpfactor*grad );
}

float
map (vec3 p)
{
    p = mod(p, 2.)-1.;
    float x1 = sminP(length(p.xy),sminP(length(p.yz),length(p.xz), 0.25), 0.25)-0.5;

    p = abs(mod(p, 0.5)-0.25);
    float x2 = min(p.x,min(p.y,p.z));

    return sqrt(x1*x1+x2*x2)-.05;
}

float
raymarch (vec3 ro, vec3 rd)
{
  float d, t = 0.0;

  for (int i = 0; i < 128; i++)
  {
    d = map(ro + rd *t);

    if (d<sEPS || t>FAR)
      break;

    t += d*0.75;
  }

  return t;
}

float
calculateAO (vec3 p, vec3 n)
{
  const float AO_SAMPLES = 5.0;
  float r = 0.0, w = 1.0, d;

  for (float i=1.0; i<AO_SAMPLES+1.1; i++)
  {
    d = i/AO_SAMPLES;
    r += w*(d - map(p + n*d));
    w *= 0.5;
  }

  return 1.0-clamp(r,0.0,1.0);
}

float
softShadow (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.0;
  const int maxIterationsShad = 16;

  float dist = start;
  float stepDist = end/float(maxIterationsShad);

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, k*h/dist);

    dist += clamp(h, 0.0005, stepDist*2.);

    if (h<0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + 0.4, 1.0); 
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
curve (vec3 p)
{
  vec2 e = vec2(-1., 1.)*0.05;

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return 7. * (t1 + t2 + t3 + t4 - 4.*map(p));
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*0.5) / iResolution.y;

  vec3 rd = normalize(vec3(uv, 0.5));

  rd.xy *= rot(iTime*0.5);
  rd.xz *= rot(iTime*0.25);

  vec3 ro = vec3(0.0, 0.0, iTime*1.0);

  vec3 lp = vec3(0.0, 0.125, -0.125);
  lp.xy *= rot(iTime*0.5);
  lp.xz *= rot(iTime*0.25);
  lp += ro + vec3(0.0, 1.0, 0.0);

  vec3 sceneCol = vec3(0.);

  float dist = raymarch(ro, rd);
  if (dist < FAR)
  {
    vec3 sp = ro + rd*dist;

    vec3 sn = getNormal(sp);

    sn = doBumpMap(sp, sn, 0.01);

    vec3 ld = lp-sp;

    vec3 objCol = tex3D( iChannel0, sp, sn );
    objCol *= bumpSurf3D(sp, sn)*0.5 + 0.5;

    float lDist = max(length(ld), 0.001);
    ld /= lDist;
    float atten = min( 1.0 /( lDist*0.5 + lDist*lDist*0.1 ), 1.0 );

    float ambient = .25;
    float diffuse = max( 0.0, dot(sn, ld) );
    float specular = max( 0.0, dot( reflect(-ld, sn), -rd) );
    specular = pow(specular, 8.0);

    float shadow = softShadow(sp, ld, sEPS*2., lDist, 32.);

    float ao = calculateAO(sp, sn)*0.5 + 0.5;

#ifdef FIRE_REFLECTION
    vec3 sf = reflect(rd, sn);

    float crv = clamp(curve(sp), 0., 1.);

    float refShade = getGrey(tex3D( iChannel1, sp/4. + iTime/64., sf ));
    refShade = refShade*0.4 + max(dot(sf, vec3(0.166)), .0);
    vec3 refCol = blackbodyPalette(refShade*(crv*0.5+0.5));
#endif

    sceneCol = objCol*(vec3(1., 0.97, 0.92)*diffuse + ambient)  + vec3(1.,0.9,0.92)*specular*0.75;
#ifdef FIRE_REFLECTION
    sceneCol += refCol;
#endif

    sceneCol *= atten*ao*shadow;
  }

  gl_FragColor = vec4(clamp(sceneCol, 0., 1.), 1);
}
