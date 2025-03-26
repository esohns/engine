#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define PI 3.14159265358979

float getGrey (vec3 p) { return p.x*0.299 + p.y*0.587 + p.z*0.114; }

mat2 rot2 (float a) { float c = cos(a), s = sin(a);	return mat2( c, -s,	s, c ); }

vec3
hash33 (vec3 p)
{
  float n = sin(dot(p, vec3(7, 157, 113)));    
  return fract(vec3(2097152, 262144, 32768)*n); 
}

vec3
tex3D (sampler2D tex, in vec3 p, in vec3 n)
{
  n = max((abs(n) - 0.2)*7., 0.001); // n = max(abs(n), 0.001), etc.
  n /= (n.x + n.y + n.z ); 
  p = (texture(tex, p.yz)*n.x + texture(tex, p.zx)*n.y + texture(tex, p.xy)*n.z).xyz;
  return p*p;
}

vec3 tri (vec3 x) { return abs(x-floor(x)-.5); }
vec3 triSmooth (vec3 x) { return cos(x*6.2831853)*0.25+0.25; }

float
surfFunc (vec3 p)
{
  float n = dot(tri(p*0.15 + tri(p.yzx*0.075)), vec3(0.444));
  p = p*1.5773;// - n; // The "n" mixes things up more.
  p.yz = vec2(p.y + p.z, p.z - p.y) * 0.866;
  p.xz = vec2(p.x + p.z, p.z - p.x) * 0.866;
  n += dot(tri(p*0.225 + tri(p.yzx*0.1125)), vec3(0.222)); 

  return abs(n-0.5)*1.9 + (1.-abs(sin(n*9.)))*0.05;
}

vec2
path (float z)
{
  float s = sin(z/36.)*cos(z/18.);
  return vec2(s*16., 0.);
}

float
map (vec3 p)
{
  return p.y - surfFunc(p)*1.5;
}

vec3
doBumpMap (sampler2D tex, vec3 p, vec3 nor, float bumpfactor)
{
  const float eps = 0.001;
  vec3 grad = vec3( getGrey(tex3D(tex, vec3(p.x-eps, p.y, p.z), nor)),
                    getGrey(tex3D(tex, vec3(p.x, p.y-eps, p.z), nor)),
                    getGrey(tex3D(tex, vec3(p.x, p.y, p.z-eps), nor)));
    
  grad = (grad - getGrey(tex3D(tex,  p , nor)))/eps; 
            
  grad -= nor*dot(nor, grad);          
                      
  return normalize( nor + grad*bumpfactor );
}

vec3
getNormal (vec3 p)
{
  vec2 e = vec2(0.5773,-0.5773)*0.001;
  return normalize( e.xyy*map(p+e.xyy ) + e.yyx*map(p+e.yyx ) + e.yxy*map(p+e.yxy ) + e.xxx*map(p+e.xxx ));
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
curve (vec3 p, float w)
{
  vec2 e = vec2(-1., 1.)*w;

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return 0.125/(w*w) *(t1 + t2 + t3 + t4 - 4.*map(p));
}

vec3
blackbodyPalette (float t)
{
  t *= 3200.;

  float cx = (0.860117757 + 1.54118254e-4*t + 1.28641212e-7*t*t)/(1.0 + 8.42420235e-4*t + 7.08145163e-7*t*t);
  float cy = (0.317398726 + 4.22806245e-5*t + 4.20481691e-8*t*t)/(1.0 - 2.89741816e-5*t + 1.61456053e-7*t*t);

  float d = (2.*cx - 8.*cy + 4.);
  vec3 XYZ = vec3(3.*cx/d, 2.*cy/d, 1. - (3.*cx + 2.*cy)/d);

  vec3 RGB = mat3(3.240479, -0.969256, 0.055648, -1.537150, 1.875992, -0.204043, 
                  -0.498535, 0.041556, 1.057311) * vec3(1./XYZ.y*XYZ.x, 1., 1./XYZ.y*XYZ.z);

  return max(RGB, 0.)*pow(t*0.0004, 4.); 
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  vec3 lookAt = vec3(0.0, 2.25, iTime*5.);
  vec3 camPos = lookAt + vec3(0.0, 0.2, -0.5);

  vec3 lp = camPos + vec3(0.0, 0.5, 2.0);
  vec3 lp2 = camPos + vec3(0.0, 0.5, 9.0);

  lookAt.xy += path (lookAt.z);
  camPos.xy += path (camPos.z);
  lp.xy += path(lp.z);
  lp2.xy += path(lp2.z);

  float FOV = PI/3.; // FOV - Field of view.
  vec3 forward = normalize(lookAt-camPos);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right); 

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

  rd.xy *= rot2( path(lookAt.z).x/32. );

  float t = 0.0, dt;
  for(int i=0; i<128; i++)
  {
    dt = map(camPos + rd*t);
    if(abs(dt)<0.001*(1. + t*0.125) || t>40.0)
      break; 
    t += dt*0.7;
  }
  t = min(t, 40.0);
  
  vec3 sceneCol = vec3(0.0);

  if(t<40.0001)
  {
    vec3 sp = camPos + rd*t;
    vec3 sn = getNormal(sp);

    const float tSize0 = 1./3.;
    sn = doBumpMap(iChannel0, sp*tSize0, sn, 0.04);

    vec3 texCol = tex3D(iChannel0, sp*tSize0, sn);

    float ao = calculateAO(sp, sn);

    vec3 ld = lp-sp;
    vec3 ld2 = lp2-sp;

    float lDist = max(length(ld), 0.001);
    float lDist2 = max(length(ld2), 0.001);

    ld /= lDist;
    ld2 /= lDist2;

    float atten = 1./(1. + lDist*lDist*0.075);
    float atten2 = 1./(1. + lDist2*lDist2*0.075);

    float ambience = 0.05;

    float diff = max( dot(sn, ld), 0.0);
    float diff2 = max( dot(sn, ld2), 0.0);

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 8.);
    float spec2 = pow(max( dot( reflect(-ld2, sn), -rd ), 0.0 ), 8.);

    float crv = clamp(curve(sp, 0.125)*0.5 + 0.5, .0, 1.);

    float shading =  crv*0.75 + 0.25; //surfFunc(sp)*0.5+0.5;//

    float rnd = fract(sin(dot(sp, vec3(7, 157, 113)))*43758.4543);
    float tRange = 0.25;
    float nDepth = 0.03;
    vec3 hf = normalize(rd+ld);
    float ss = tRange*0.5-map( sp-sn*(nDepth+rnd*0.005) + hf*tRange*0.5 ) + (tRange-map( sp-sn*(nDepth*2.+rnd*0.01) + hf*tRange ))*0.5;
    hf = normalize(rd+ld2);
    float ss2 = tRange*0.5-map( sp-sn*(nDepth+rnd*0.005) + hf*tRange*0.5 ) + (tRange-map( sp-sn*(nDepth*2.+rnd*0.01) + hf*tRange ))*0.5;

    ss = max(ss, 0.);
    ss2 = max(ss2, 0.);

    vec3 rCol = getGrey(texCol)*0.5 + texCol*0.5;
    vec3 sCol = texCol*0.5 + 0.5;
    sceneCol += (rCol*(diff + ambience) + blackbodyPalette(ss*.6)*sCol*12.0 + spec*texCol*2.)*atten;
    sceneCol += (rCol*(diff2 + ambience) + blackbodyPalette(ss2*.6)*sCol*12.0 + spec2*texCol*2.)*atten2;

    sceneCol *= shading*ao;
  }

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
