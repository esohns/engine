#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define MAXDIST 15.
// Light color - Reddish purple: 0, Greenish blue: 1 
#define COLOR 0

mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
hash31 (vec3 p)
{
  return fract(sin(dot(p, vec3(113.619, 57.583, 27.897)))*43758.5453); 
} 

vec2
path (float z)
{
  float c = cos(z*3.14159265/4.);
  float s = sin(z*3.14159265/4.);

  return vec2(1. + c*2. - .0, 1. + s*2. - .0); 
}

float
smax (float a, float b, float k)
{
  float f = max(0., 1. - abs(b - a)/k);

  return max(a, b) + k*.25*f*f;
}

vec3
tex3D (sampler2D channel, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(channel, p.yz).xyz;
  vec3 ty = texture(channel, p.xz).xyz;
  vec3 tz = texture(channel, p.xy).xyz;

  return tx*tx*n.x + ty*ty*n.y + tz*tz*n.z;
}

float
apollonian (vec3 p)
{
  const float rad = 1.545;
  float scale = 1.;

  for( int i=0; i<8; i++ )
  {
    p = mod(p - 1., 2.) - 1.;

    float k = rad/dot(p, p);

    p *= k;
    scale *= k;
  }

  return length(p)/scale/4. - .0025;
}

float
lengthP (vec2 p)
{
  return length(p);
}

float gID;
vec3 glow;

float
map (vec3 p)
{
  vec3 q = p - sin(p*8. - cos(p.yzx*8.))*.035; 

  vec3 iq = floor(q/2.);
  q -= (iq + .5)*2.;

  float rnd = hash31(iq + .12);
  if(rnd<.33) q = q.yxz;
  else if(rnd<.66) q = q.xzy;

  float aN = 8., a, n;

  float lat, light;

  vec3 rq = q - vec3(1, 1, 0); 

  vec2 tor = vec2(length(rq.xy) - 1., rq.z); 
  tor = abs(abs(tor) - .45);
  lat = lengthP(tor);

  a = atan(rq.y, rq.x)/6.2831;
  n = (floor(a*aN) + .5)/aN;
  rq.xy *= r2(-6.2831*n);
  rq.x -= 1.;   
  rq.xz = abs(abs(rq.xz) - .45);

  float bx = 1e5;
  if(hash31(iq - vec3(1, 1, 0) + n*.043)<.25) bx = length(rq);
  light = bx;     

  rq = q - vec3(0, -1, -1); 
  a = atan(rq.z, rq.y)/6.2831;
  tor = vec2(length(rq.yz) - 1., rq.x);
  tor = abs(abs(tor) - .45);
  lat = min(lat, lengthP(tor)); 
  n = (floor(a*aN) + .5)/aN;
  rq.yz *= r2(-6.2831*n);
  rq.y -= 1.;
  rq.xy = abs(abs(rq.xy) - .45);
  bx = 1e5;
  if(hash31(iq - vec3(0, -1, -1) + n*.043)<.25) bx = length(rq);
  light = min(light, bx);

  rq = q - vec3(-1, 0, 1);
  a = atan(rq.z, rq.x)/6.2831;
  tor = vec2(length(rq.xz) - 1., rq.y);
  tor = abs(abs(tor) - .45);
  lat = min(lat, lengthP(tor));
  n = (floor(a*aN) + .5)/aN;
  rq.xz *= r2(-6.2831*n);
  rq.x -= 1.;
  rq.xy = abs(abs(rq.xy) - .45);    
  bx = 1e5;
  if(hash31(iq - vec3(-1, 0, 1) + n*.043)<.25) bx = length(rq);
  light = min(light, bx);

  lat -= .01;
  light -= .05;

  light = max(light, abs(lat - .015) - .01);

  float ga = apollonian(p);

  q = mod(p, 2.) - 1.;

  ga = smax(ga, -(length(q) - 1.203), .005);

  gID = lat<ga && lat<light? 2. : ga<light? 1. : 0.;
  if(gID==0.)
    glow += vec3(1, .2, .1)/(1. + light*light*256.);

  return min(lat, min(ga, light)); 
}

float
march (vec3 ro, vec3 rd)
{
  float d, t = hash31(ro + rd)*.15;

  glow = vec3(0);

  vec2 dt = vec2(1e5, 0);

  int i;
  const int iMax = 128;
  for (i = 0; i<iMax; i++) 
  {
    d = map(ro + rd*t);
    dt = d<dt.x? vec2(d, dt.x) : dt;
    if (abs(d)<.001*(1. + t*.1) || t>MAXDIST)
      break;

    t += min(d*.8, .3); 
  }
  if(i>=iMax - 1) t = dt.y;

  return min(t, MAXDIST);
}

vec3
nrm (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.001, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += map(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1){ mp = mp.yzx; e = e.zxy; }
  }

  return normalize(mp);
}

float
shadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int maxIterationsShad = 48; 

  ro += n*.0015;
  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), .0001);

  rd /= end;

  for (int i = min(iFrame, 0); i<maxIterationsShad; i++)
  {
    float d = map(ro + rd*t);
    shade = min(shade, k*d/t);

    t += clamp(d*.8, .005, .25); 

    if (d<0. || t>end)
      break;
  }

  return max(shade, 0.); 
}

float
cAO (vec3 pos, vec3 nor)
{
  float sca = 1.5, occ = 0.0;
  for( int i=0; i<5; i++ )
  {
    float hr = 0.01 + float(i)*0.35/4.0;        
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp( 1.0 - occ, 0.0, 1.0 );    
}

vec4
render (vec3 ro, vec3 rd, vec3 lp)
{
  float t = march(ro, rd);

  float svID = gID;

  vec3 svGlow = glow;

  vec3 col = vec3(0);

  if (t<MAXDIST)
  {
    vec3 sp = ro + rd*t;

    vec3 sn = nrm(sp); 

    vec3 ld = (lp - sp);
    float atten = max(length(ld), .001);
    ld /= atten;

    atten = 1./(1. + atten*.125);

    float diff = max(dot(ld, sn), 0.);

    float spec = pow(max(dot(reflect(rd, sn), ld), 0.), 8.);

    float ao = cAO(sp, sn);
    float sh = shadow(sp, lp, sn, 8.); 

    float sinF = dot(sin(sp - cos(sp.yzx)*1.57), vec3(1./6.)) + .5;

#if COLOR == 0
    svGlow = mix(svGlow, svGlow.zyx, sinF);
#else
    svGlow = mix(svGlow.zxy, svGlow.zyx, sinF);
#endif

    vec3 tx = tex3D(iChannel1, sp*2., sn);
    col = tx;

    if (svID==1.) col /= 8.;
    else if (svID==2.) col /= 4.;

    col = col*(diff*sh + .2) + spec*sh*.5;

    col += col*(svGlow)*32.;

    col *= atten*ao;
  }

  col = mix(col, vec3(0), smoothstep(.0, .8, t/MAXDIST));

  return vec4(col, t);
}

vec4
uTone (vec4 x)
{
  return ((x*(x*.6 + .1) + .004)/(x*(x*.6 + 1.)  + .06) - .0667)*1.933526;    
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec3 ro = vec3(0, 0, iTime*.25);
  vec3 lk = ro + vec3(0, -.05, .25);
  vec3 lp = ro + vec3(0, 0, .75);

  ro.xy += path(ro.z);
  lk.xy += path(lk.z);
  lp.xy += path(lp.z);

  float FOV = 3.14159/1.5;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt);

  vec3 rd = normalize(uv.x*rgt + uv.y*up + fwd/FOV);  

  vec2 sw = path(lk.z);
  rd.xy *= r2(-sw.x/24.);
  rd.yz *= r2(-sw.y/16.);    

  vec4 col4 = render(ro, rd, lp);

  col4.xyz = uTone(col4).xyz;

#if 1
  vec4 preCol = texelFetch(iChannel0, ivec2(gl_FragCoord.xy), 0);
  float blend = (iFrame < 2) ? 1. : 1./3.; 
  gl_FragColor = mix(preCol, vec4(clamp(col4.xyz, 0., 1.), col4.w), blend);
#else
  gl_FragColor = vec4(clamp(col4.xyz, 0., 1.), col4.w);
#endif
}
