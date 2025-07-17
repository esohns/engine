#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define FAR 40.

float objID;
float mObjID;

mat2 r2 (float th) { vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

float
smin (float a, float b , float s)
{
  float h = clamp( 0.5 + 0.5*(b-a)/s, 0. , 1.);
  return mix(b, a, h) - h*(1.0-h)*s;
}

float
smax (float a, float b, float s)
{
  float h = clamp( 0.5 + 0.5*(a-b)/s, 0., 1.);
  return mix(b, a, h) + h*(1.0-h)*s;
}

vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  return fract(vec2(262144, 32768)*n);
}

float
Voronoi (vec2 p)
{
  vec2 g = floor(p), o; p -= g;

  vec3 d = vec3(2.5);

  for(int y=-1; y<=1; y++)
    for(int x=-1; x<=1; x++)
    {
      o = vec2(x, y);
      o += hash22(g + o) - p;

      d.z = dot(o, o);
      d.y = max(d.x, min(d.y, d.z));
      d.x = min(d.x, d.z); 
    }

  return min((d.y - d.x)*.6, 1.);
}

vec3 tri (vec3 x) { return abs(fract(x)-.5); }

float
terrain (vec3 p)
{
  float n = dot(tri(p*0.3 + tri(p.yzx*0.15)), vec3(0.44));
  p = p*1.57;
  p.yz = mat2(.866025, .5, -.5, .866025)*p.yz;
  p.xz = mat2(.866025, .5, -.5, .866025)*p.xz;
  n += dot(tri(p*0.45 + tri(p.yzx*0.225)), vec3(0.222));

  return smoothstep(0.3, .95, n);
}

float
Mobius (vec3 q)
{
  const float toroidRadius = 1.25;
  const float polRot = 4./4.;
  const float ringNum = 32.;

  vec3 p = q;

  float a = atan(p.z, p.x);

  float ia = floor(ringNum*a/6.2831853);  
  ia = (ia + .5)/ringNum*6.2831853; 

  p.xz *= r2(a);
  p.x -= toroidRadius;
  p.xy *= r2(a*polRot);

  p = abs(abs(p) - .25);

  float rail = max(max(p.x, p.y) - .07, (max(p.y-p.x, p.y + p.x)*.7071 - .075));

  p = q;
  p.xz *= r2(ia);
  p.x -= toroidRadius;
  p.xy *= r2(a*polRot);

  p = abs(p);
  float ring = max(p.x, p.y);
  ring = max(max(ring - .275, p.z - .03), -(ring - .2));

  mObjID = step(ring, rail);

  return smin(ring, rail, .07); 
}

float
map (vec3 p)
{
  float obj = Mobius(p - vec3(0, .4, 0));

  float ter = terrain(p);

  float fl = p.y  - ter;

  fl =  smax(fl, -max(length(p - vec3(0, 2.5, 0)) - 3., - p.y - .0), .5);

  objID = step(obj, fl);

  return min(fl, obj);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0;

  for (int i=0; i<96; i++)
  {
    float d = map(ro + rd*t);

    if(abs(d)<0.001*(t*.125 + 1.) || t>FAR)
      break;

    t += d*.85;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p, inout float edge, inout float crv, float ef, float t)
{
  vec2 e = vec2(ef/iResolution.y, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.002, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx); 

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
softShadow (vec3 ro, vec3 lp, vec3 n, float k)
{
  const int maxIterationsShad = 64; 

  ro += n*.0015;
  vec3 rd = lp - ro;

  float shade = 1.;
  float t = 0.; 
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i = 0; i<maxIterationsShad; i++)
  {
    float d = map(ro + rd*t);

    shade = min(shade, k*d/t);

    t += clamp(d, .01, .25); 

    if (d<0. || t>end)
      break;
  }

  return max(shade, 0.); 
}

float
cAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for (int i=0; i<5; i++)
  {
    float hr = 0.01 + float(i)*0.5/4.0;        
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp( 1.0 - occ, 0.0, 1.0 );    
}

float
bumpFunction (vec3 p)
{
  return Voronoi(p.xz*6.);
}

vec3
doBumpMap (vec3 p, vec3 n, float bumpfactor, inout float edge, inout float crv)
{
  vec2 e = vec2(1.5/iResolution.y, 0); 

  float f = bumpFunction(p);

  float fx = bumpFunction(p - e.xyy);
  float fy = bumpFunction(p - e.yxy);
  float fz = bumpFunction(p - e.yyx);
    
  float fx2 = bumpFunction(p + e.xyy);
  float fy2 = bumpFunction(p + e.yxy);
  float fz2 = bumpFunction(p+ e.yyx);

  vec3 grad = vec3(fx - fx2, fy - fy2, fz - fz2)/(e.x*2.);  

  edge = abs(fx + fy + fz + fx2 + fy2 + fz2 - 6.*f);
  edge = smoothstep(0., 1., edge/e.x);

  grad -= n*dot(n, grad);          

  return normalize(n + grad*bumpfactor);
}

vec3
tex3D (sampler2D channel, vec3 p, vec3 n)
{
  n = max(abs(n) - .2, 0.001);
  n /= dot(n, vec3(1));
  vec3 tx = texture(channel, p.zy).xyz;
  vec3 ty = texture(channel, p.xz).xyz;
  vec3 tz = texture(channel, p.xy).xyz;

  return tx*tx*n.x + ty*ty*n.y + tz*tz*n.z;
}

vec3
texBump (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.002, 0);

  mat3 m = mat3( tex3D(tx, p - e.xyy, n), tex3D(tx, p - e.yxy, n), tex3D(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m;
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf );
}

float
curve (vec3 p, float w)
{
  vec2 e = vec2(-1., 1.)*w;

  float t1 = map(p + e.yxx), t2 = map(p + e.xxy);
  float t3 = map(p + e.xyx), t4 = map(p + e.yyy);

  return 0.125/(w*w) *(t1 + t2 + t3 + t4 - 4.*map(p));
}

float
n3D (vec3 p)
{
  const vec3 s = vec3(7, 157, 113);
  vec3 ip = floor(p); p -= ip; 
  vec4 h = vec4(0., s.yz, s.y + s.z) + dot(ip, s);
  p = p*p*(3. - 2.*p);
  h = mix(fract(sin(mod(h, 6.2831589))*43758.5453), 
          fract(sin(mod(h + s.x, 6.2831589))*43758.5453), p.x);
  h.xy = mix(h.xz, h.yw, p.y);

  return mix(h.x, h.y, p.z);
}

vec3
envMap (vec3 p)
{
  p *= 2.;
  p.xz += iTime*.5;

  float n3D2 = n3D(p*2.);

  float c = n3D(p)*.57 + n3D2*.28 + n3D(p*4.)*.15;
  c = smoothstep(0.5, 1., c);

  p = vec3(c*c*c*c, c*c, c);

  return mix(p.zxy, p, n3D2*.34 + .665);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution*.5)/iResolution.y;

  vec3 lk = vec3(0, .25, 0);
  vec3 ro = lk + vec3(cos(iTime/4.)*2.8, cos(iTime/2.)*sin(iTime/4.)*.25 + .75, sin(iTime/4.)*3.3); 
  vec3 lp = ro + vec3(.85, 1.75, -1);

  float FOV = 1.25;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt); 

  vec3 rd = normalize(fwd + (rgt*uv.x + up*uv.y)*FOV);

  vec2 ms = vec2(0);
  if (iMouse.z > 0.0) ms = (2.*iMouse.xy - iResolution)/iResolution;
  vec2 a = sin(vec2(1.5707963, 0) - ms.x);
  mat2 rM = mat2(a, -a.y, a.x);
  rd.xz = rd.xz*rM;
  a = sin(vec2(1.5707963, 0) - ms.y);
  rM = mat2(a, -a.y, a.x);
  rd.yz = rd.yz*rM;

  float t = trace(ro, rd);

  float svObjID = objID;
  float svMObjID = mObjID;

  vec3 sceneCol = vec3(0);

  if(t < FAR)
  {
    float edge, crv = 1., ef = 4.;

    float tSize0 = 1.;

    float bf = .005;

    if(svObjID<0.5)
    {
      bf = .02;
      tSize0 = 1./1.;
      ef = 1.5;
    }

    vec3 sp = ro + rd*t;
    vec3 sn = getNormal(sp, edge, crv, ef, t);

    sn = texBump(iChannel0, sp*tSize0, sn, bf);

    float edge2 = 0., crv2 = 1.;    
    if(svObjID<.5) sn = doBumpMap(sp, sn, .1/(1. + t/FAR), edge2, crv2);    

    vec3 texCol;

    if(svObjID<0.5)
    {
      texCol = tex3D(iChannel0, sp*tSize0, sn);
      texCol = smoothstep(0.05, .5, texCol)*vec3(1, .75, .5);
      texCol *= terrain(sp)*.5 + .5;
    }
    else
    {
      texCol = tex3D(iChannel0, sp*tSize0, sn);
      texCol = smoothstep(0.05, .5, texCol);

      texCol = mix(texCol, texCol*vec3(1, .4, .2)*2., svMObjID);
    }

    vec3 ld = lp-sp;

    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float sh = softShadow(sp, lp, sn, 8.);
    float ao = cAO(sp, sn);

    float atten = 1./(1. + lDist*lDist*0.05);

    float diff = max( dot(sn, ld), 0.0);
    diff = pow(diff, 4.)*1.5;

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 16.); 

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 4.);

    vec3 env = envMap(reflect(rd, sn))*2.;
    if(svObjID<.5)
    {
      env *= .25;
      spec *= .5;            
      fre *= .5;
    }

    sceneCol += (texCol*(diff*sh + 0.15 + vec3(.7, .9, 1)*fre*sh) + env*sh + vec3(.7, .9, 1)*spec*sh*2.);

    if(svObjID>.5) sceneCol *= (1. - edge*.8);
    else sceneCol *= (1. - edge2*.7);

    sceneCol *= ao*atten;
  }

  vec3 bg = mix(vec3(.6, .5, 1), vec3(.025, .05, .1), clamp(rd.y + .75, 0., 1.));
  sceneCol = mix(sceneCol, bg/48., smoothstep(0., .95, t/FAR));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
