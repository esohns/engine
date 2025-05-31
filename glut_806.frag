#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

#define FAR 50.

#define BRACING

float objID;
float svObjID;

float hash (float n) { return fract(sin(n)*43758.5453); }
mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

vec2
path (float t)
{
  return vec2(0);
  //float s = sin(t/24.)*cos(t/12.);
  //return vec2(s*4., 0.);

  float a = sin(t*.11);
  float b = cos(t*.14);
  return vec2(a*4. -b*1.5, b*1.7 + a*1.5);

  //return vec2(sin(t*.15)*2.4, cos(t*.25)*1.7*.5); 
}

vec3
tex3D (sampler2D t, in vec3 p, in vec3 n)
{
  n = max(abs(n), 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(t, p.yz).xyz;
  vec3 ty = texture(t, p.zx).xyz;
  vec3 tz = texture(t, p.xy).xyz;
  return (tx*tx*n.x + ty*ty*n.y + tz*tz*n.z);
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7., 157., 113.);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p *= p*p*(p*(p * 6. - 15.) + 10.);
  h = mix(fract(sin(h)*43758.5453), fract(sin(h + s.x)*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);
  return mix(h.x, h.y, p.z); // Range: [0, 1].
}


float
smax (float a, float b, float s)
{
  float h = clamp(.5 + .5*(a - b)/s, 0., 1.);
  return mix(b, a, h) + h*(1. - h)*s;
}

float
map (vec3 q)
{
  q.xy -= path(q.z); // Wrap the object around the path.

  vec3 p = abs(fract(q/3.)*3. - 1.5);
  float d = min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1. + .03;

#ifdef BRACING
  float tb;

  float rp = 3.;
  p = abs(fract(q/rp + .5)*rp - rp/2.);
  float x1 = sqrt(min(dot(p.xy, p.xy),min(dot(p.yz, p.yz),dot(p.xz, p.xz))))-rp/2.*.475; // EQN 2

  rp = 3./15.;
  p = abs(fract(q/rp)*rp - rp/2.);
  p = abs(p - rp/2.);
  float x2 = min(p.x, min(p.y,p.z)) - .0125; // EQN 1

  tb = smax(abs(x1), abs(x2), .1) - .035; 
#endif

  p =  abs(fract(q) - .5);    
  d = smax(d, min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1./3. + .05, .05);

  p =  abs(fract(q*2.)*.5 - .25);
  d = max(d, min(smax(p.x, p.y, .125), min(smax(p.y, p.z, .125), smax(p.x, p.z, .125))) - .5/3. - .025); 

  p =  abs(fract(q*3./.5)*.5/3. - .5/6.);
  d = max(d, min(max(p.x, p.y), min(max(p.y, p.z), max(p.x, p.z))) - 1./18. - .015);

#ifdef BRACING
    objID = step(d, tb);
    return min(d, tb);
#else
    objID = 0.;
    return d;
#endif
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for (int i=0; i<96; i++)
  {
    d = map(ro + rd*t);
    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;

    t += d;
  }

  return min(t, FAR);
}

float
traceRef (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for (int i=0; i<48; i++)
  {
    d = map(ro + rd*t);
    if(abs(d)<0.001*(t*.125 + 1.) || t>FAR)
      break;

    t += d;
  }

  return min(t, FAR);
}

float
traceRefr (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for (int i=0; i<48; i++)
  {
    d = map(ro + rd*t);

    if((d<0. && abs(d)<.001*(t*.25 + 1.)) || t>FAR)
      break;

    t += -d;
  }

  return min(t, FAR);
}

float
softShadow (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 24; 

  vec3 rd = lp-ro;

  float shade = 1.;
  float dist = .0025;
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0., 1., k*h/dist));

    dist += clamp(h, .01, .2); 

    if (h<0. || dist > end)
      break; 
  }

  return min(max(shade, 0.) + .2, 1.); 
}

float
getEdge (vec3 p, vec2 e)
{ 
  float sgn = 1.;
  float mp[6];
  vec3[3] e6 = vec3[3](e.xyy, e.yxy, e.yyx);

  for(int i = min(0, iFrame); i<6; i++)
  {
    mp[i] = map(p + sgn*e6[i/2]);
    sgn = -sgn;
    if(sgn>2.)
      break;
  }

  float d = map(p)*2.;

  float edge = abs(mp[0] + mp[1] - d) + abs(mp[2] + mp[3] - d) + abs(mp[4] + mp[5] - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  return edge;
}

vec3
getNrm (vec3 p)
{
  float sgn = 1.;
  vec3 e = vec3(.0015, 0, 0), mp = e.zzz;
  for(int i = min(iFrame, 0); i<6; i++)
  {
    mp.x += map(p + sgn*e)*sgn;
    sgn = -sgn;
    if((i&1)==1)
    {
      mp = mp.yzx;
      e = e.zxy;
    }
  }

  return normalize(mp);
}

vec3
getNormal (vec3 p, inout float edge, inout float crv, float t)
{
  vec2 e = vec2(3./iResolution.y*(1. + t*.5), 0);

  edge = getEdge(p, e);

  return getNrm(p);
}

vec3
getObjectColor (vec3 p, vec3 n)
{
  vec3 txP = p - vec3(path(p.z), 0.);
  vec3 tx = tex3D(iChannel0, txP/1., n);
  tx = mix(tx, vec3(1)*dot(tx, vec3(.299, .587, .114)), .25);
  tx = smoothstep(0., 1., tx);

  vec3 lCol = vec3(1.25, 1, .75);

  vec3 q = abs(mod(txP, 3.) - 1.5);
  if (max(max(q.x, q.y), q.z) > 1.063) tx *= lCol;
  else tx *= vec3(1., .75, .5);

#ifdef BRACING
  if(svObjID<.5)
    tx *= 5./vec3(1.25, 1, .75);
#endif

  return tx;
}

vec3
doColor (vec3 sp, vec3 rd, vec3 sn, vec3 lp, float edge, float crv, float ao, float t)
{
  vec3 sceneCol = vec3(0);

  if(t<FAR)
  {
    vec3 ld = lp - sp;
    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float atten = 2./(1. + lDist*0.125 + lDist*lDist*0.025);

    float diff = max(dot(sn, ld), 0.);
    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.0);
    float fres = clamp(1. + dot(rd, sn), 0., 1.);

    vec3 objCol = getObjectColor(sp, sn);

    sceneCol = objCol*(diff + .6*ao + fres*fres) + vec3(1, .7, .5).zyx*spec*2.;

    sceneCol *= 1. - edge*.9;

    sceneCol *= atten;
  }

  return sceneCol;
}

float
calculateAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for( int i=0; i<5; i++ )
  {
    float hr = 0.01 + float(i)*0.5/4.0;        
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }
  return clamp( 1.0 - occ, 0.0, 1.0 );    
}

vec3
eMap (vec3 rd, vec3 sn)
{
  return getObjectColor(rd, sn);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5) / iResolution.y;

  vec3 ro = vec3(0, 0, iTime);
  vec3 lk = ro + vec3(0, 0, .25);

  vec3 lp = ro + vec3(0, 1, 0);

  ro.xy += path(ro.z);
  lk.xy += path(lk.z);
  lp.xy += path(lp.z);

  float FOV = 3.14159/2.;
  vec3 forward = normalize(lk-ro);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + FOV*uv.x*right + FOV*uv.y*up);

  rd.xy *= r2(iTime/4.);
  rd.xz *= r2(iTime/4.);

  float edge = 0., crv = 1.;

  float t = trace(ro, rd);
  svObjID = objID;
  float oSvObjID = svObjID;

  vec3 sp = ro + rd*t;
  vec3 sn = getNormal(sp, edge, crv, t);

  float sh = softShadow(sp + sn*.002, lp, 16., t);
  float ao = calculateAO(sp, sn);
  sh = (sh + ao*.3)*ao;

  vec3 sceneColor = doColor(sp, rd, sn, lp, edge, crv, ao, t);

  float fog = smoothstep(0., .95, t/FAR);

  vec3 refl = reflect(rd, sn);

  vec3 refSp; 
    
  t = traceRef(sp + sn*0.0015, refl);

  svObjID = objID;
  oSvObjID = svObjID; 

  refSp = sp + refl*t;
  sn = getNormal(refSp, edge, crv, t);

  vec3 reflColor = doColor(refSp, refl, sn, lp, edge, crv, 1., t);
  sceneColor = sceneColor + reflColor*.5 ;

  sceneColor *= sh;

  vec3 fogCol = vec3(.7, .8, 1.)*(rd.y*.5 + .5)*2.5;
  sceneColor = mix(sceneColor, fogCol, fog);

  sceneColor = mix(sceneColor, pow(min(vec3(1.5, 1, 1)*sceneColor, 1.), vec3(1, 2.5, 12.)), uv.y);

  uv = gl_FragCoord.xy/iResolution.xy;
  sceneColor *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .125)*.5 + .5;

  gl_FragColor = vec4(sqrt(clamp(sceneColor, 0.0, 1.0)), 1.0);
}
