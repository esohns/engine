#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 40.

//#define SMOOTH_QUAD

float objID;

mat2 r2 (float th) { vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

float
hm (vec2 p)
{
  p = p/2. + iTime/4.;

  float n = dot(sin(p.xy*3.14159 - cos(p.yx*3.14159)*3.14159/2.), vec2(0.25) + .5)*.66;
  p = p*1.5;

  p.xy = mat2(.866025, .5, -.5, .866025)*p.xy;

  n += dot(sin(p.xy*3.14159 - cos(p.yx*3.14159)*3.14159/2.), vec2(0.25) + .5)*.34;

  return n;
}

#define scale 2. 
vec3 hVal;

float
triTerrain (vec2 p)
{
  vec2 ip = floor(p);
  p -= ip;

  float s = step(1., p.x + p.y);

  hVal = vec3(hm(ip+s), hm(ip + vec2(1, 0)), hm(ip+vec2(0, 1)));

#ifdef SMOOTH_QUAD
  return mix(mix(hm(ip), hVal.y, p.x), mix(hVal.z, hm(ip+1.), p.x), p.y);
#else
  vec3 b = abs(vec3(1.0 - p.x - p.y, p.x - (p.x - p.y + 1.)*s, p.y - (p.y - p.x + 1.)*s));

  return dot(b, hVal);
#endif
}

float
map (vec3 p)
{
  float ter = triTerrain(p.xz*scale)/scale; 

  const float hPert = .25;
  float fl = p.y  + (.5 - ter)*hPert;

  hVal = hVal*hPert - .175;

  vec3 q = p*scale;
  q.xz = fract(q.xz);

  float ln = length(vec2(q.x, abs(q.y - hVal.x -(hVal.z - hVal.x)*q.z - .25) -.25));
  ln = min(ln, length(vec2(q.x - 1., abs(q.y - hVal.y - (hVal.x - hVal.y)*q.z - .25) - .25)));

  ln = min(ln, length(vec2(abs(q.y - hVal.x - (hVal.y - hVal.x)*q.x - .25) -.25, q.z)));
  ln = min(ln, length(vec2(abs(q.y - hVal.z - (hVal.x - hVal.z)*q.x - .25) -.25, q.z - 1.)));

  float hgt = hm(floor(p.xz*scale + .5))*hPert - .175;
  vec2 offXZ = fract(p.xz*scale + .5) - .5;
  ln = min(ln, max(length(offXZ), abs(q.y - hgt) - .5));

  float sp = length(vec3(offXZ.x, abs(abs(q.y - hgt - .25) - .25), offXZ.y));

  ln -= .04;
  sp -= .125;
  ln = min(ln, sp);

  ln /= scale;
  sp /= scale;

  objID = step(fl, ln);

  return min(fl, ln);
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.;

  for (int i=0; i<96; i++)
  {
    float d = map(ro + rd*t);

    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;

    t += d*.8;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p, float t)
{
  const vec2 e = vec2(0.002, 0);
  return normalize(vec3(map(p + e.xyy) - map(p - e.xyy), map(p + e.yxy) - map(p - e.yxy),	map(p + e.yyx) - map(p - e.yyx)));
}

float
softShadow (vec3 ro, vec3 lp, float k)
{
  const int maxIterationsShad = 20; 

  vec3 rd = (lp-ro);

  float shade = 1.0;
  float dist = 0.05;    
  float end = max(length(rd), 0.001);

  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));
    dist += clamp(h, 0.01, .5);

    if (h<0.001 || dist > end)
      break;
  }

  return min(max(shade, 0.) + 0.2, 1.0); 
}

float
cAO (vec3 pos, vec3 nor)
{
  float sca = 2.0, occ = 0.0;
  for (int i=0; i<5; i++ )
  {
    float hr = 0.01 + float(i)*0.5/4.0;
    float dd = map(nor * hr + pos);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }

  return clamp( 1.0 - occ, 0.0, 1.0 );
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
  g = (g - dot(tex3D(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x;
  g -= n*dot(n, g);

  return normalize( n + g*bf );
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

vec3
envMap (vec3 p)
{
  p *= 2.;
  p.xz += iTime*.5;

  float n3D2 = n3D(p*2.);

  float c = n3D(p)*.57 + n3D2*.28 + n3D(p*4.)*.15;
  c = smoothstep(0.5, 1., c);

  p = vec3(c*.8, c*.9, c);//vec3(c*c, c*sqrt(c), c);

  return mix(p.zxy, p, n3D2*.34 + .665);
}

vec2
path (float z)
{
  float s = sin(z/36.)*cos(z/18.);
  return vec2(s*16., 0.);
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/iResolution.y;

  vec3 lk = vec3(0, -.5 - .125*.5, iTime + 1.);
  vec3 ro = lk + vec3(0, 2.5, -2.);

  vec3 lp = ro + vec3(0, .75, 2);// Put it a bit in front of the camera.

  lk.xy += path(lk.z);
  ro.xy += path(ro.z);
  lp.xy += path(lp.z);

  float FOV = 1.; // FOV - Field of view.
  vec3 fwd = normalize(lk-ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt); 

  vec3 rd = normalize(fwd + FOV*uv.x*rgt + FOV*uv.y*up);

  rd.xy *= r2( path(lk.z).x/128. );

/*
  // Mouse controls
  vec2 ms = vec2(0);
  if (iMouse.z >= 1.0) ms = (2.*iMouse.xy - iResolution.xy)/iResolution.xy;
  vec2 a = sin(vec2(1.5707963, 0) - ms.x); 
  mat2 rM = mat2(a, -a.y, a.x);
  rd.xz = rd.xz*rM; 
  a = sin(vec2(1.5707963, 0) - ms.y); 
  rM = mat2(a, -a.y, a.x);
  rd.yz = rd.yz*rM;
*/

  float t = trace(ro, rd);

  float svObjID = objID;

  vec3 sceneCol = vec3(0);

  if(t < FAR)
  {
    float tSize0 = 1./2.;

    vec3 sp = ro + rd*t;
    vec3 sn = getNormal(sp, t);

    vec3 texCol;

    if(svObjID>0.5)
    {
      texCol = tex3D(iChannel0, sp*tSize0, sn);
      texCol = smoothstep(0.0, .5, texCol);//*vec3(1, .9, .8);//

      if(mod(dot(floor(sp.xz*scale), vec2(1, -1)), 5.)>=2.) texCol *= vec3(1, .5, .25);
    }
    else
    {
      texCol = tex3D(iChannel0, sp*tSize0, sn)*.25;
    }

    vec3 ld = lp-sp;

    float lDist = max(length(ld), 0.001);

    ld /= lDist;

    float sh = softShadow(sp, lp, 8.);
    float ao = cAO(sp, sn);

    float atten = 1./(1. + lDist*lDist*0.05);

    float diff = max( dot(sn, ld), 0.0);
    diff = pow(diff, 4.)*1.5;

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 8.); 

    float fre = pow( clamp(dot(sn, rd) + 1., .0, 1.), 4.);

    vec3 env = envMap(reflect(rd, sn))*2.;
    if (svObjID>.5)
    {
      env *= .25;
      spec *= .5;            
      fre *= .5;
    }

    sceneCol += (texCol*(diff + 0.25 + vec3(1, .9, .7)*fre) + env + vec3(1, .95, .8)*spec);

    sceneCol *= ao*atten*sh;
  }

  vec3 bg = mix(vec3(.6, .5, 1), vec3(.025, .05, .1), clamp(rd.y + .75, 0., 1.));
  sceneCol = mix(sceneCol, bg, smoothstep(0., .95, t/FAR));

  gl_FragColor = vec4(sqrt(clamp(sceneCol, 0., 1.)), 1.0);
}
