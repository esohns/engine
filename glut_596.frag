#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 40.
//#define THREE_D 
//#define NO_RERACTION

float objID;
float svObjID;

mat2 r2 (float th) { vec2 a = sin(vec2(1.5707963, 0) + th); return mat2(a, -a.y, a.x); }

vec3
tex3D (sampler2D t, vec3 p, vec3 n)
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
  return mix(h.x, h.y, p.z);
}

const vec2 e = vec2(0, 1);

vec3
hash (vec3 p)
{
  p = fract(p * vec3(.1031, .1030, .0973));
  p += dot(p, p.yxz + 19.19);
  p = fract((p.xxy + p.yxx)*p.zyx)*2. - 1.;
  return p;
}

float
gradN3D (vec3 f)
{
  const vec2 e = vec2(0, 1);

  vec3 p = floor(f);
  f -= p;

  vec3 w = f*f*(3. - 2.*f);

  float c = mix(mix(mix(dot(hash(p + e.xxx), f - e.xxx), dot(hash(p + e.yxx), f - e.yxx), w.x),
                    mix(dot(hash(p + e.xyx), f - e.xyx), dot(hash(p + e.yyx), f - e.yyx), w.x), w.y),
                mix(mix(dot(hash(p + e.xxy), f - e.xxy), dot(hash(p + e.yxy), f - e.yxy), w.x),
                    mix(dot(hash(p + e.xyy), f - e.xyy), dot(hash(p + e.yyy), f - e.yyy), w.x), w.y), w.z);

  return c*.5 + .5;
}

vec2
path (float z)
{
  return vec2(sin(z*.15)*2.4, cos(z*.25)*1.7); 
}

vec3 tri (vec3 x) { return abs(x - floor(x) - .5); }
vec3 trap (vec3 x, float pf) { return (tri(x - pf*.125) + tri(x + pf*.125))*.5; }

float
surfFunc (vec3 p)
{
  return dot(trap(p*.384 + trap(p.yzx*.192, .75), .75), vec3(.666));
}

float
surfFunc2 (vec3 p)
{
  p += vec3(0, 0, -iTime*4.); // Flow it in the Z-direction - down the tunnel.
  return dot(sin(p + sin(p.yzx*2. + iTime*1.)), vec3(.1666)) + .5;
}

float
map (vec3 p)
{
  float sf = surfFunc(p);

  p.xy -= path(p.z);

  // Round tunnel: Euclidean distance: length(tun.xy)
  float n = 1. - length(p.xy*vec2(0.5, 0.7071)) + (0.5 - sf);

  // Square tunnel: Chebyshev(?) distance: max(abs(tun.x), abs(tun.y))
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.7071);
  //float n = 1.- max(tun.x, tun.y) + (0.5 - sf);

  // Rounded square tunnel.
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.7071);
  //float n = 1. - pow(dot(pow(tun, vec2(4)), vec2(1)), 1./4.) + (0.5 - sf);

  // Hexagonal tunnel.
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.75);
  //float n = 1. - max(tun.x*.866025 + tun.y*.5, tun.y) + (0.5 - sf); //1.133975

  sf = surfFunc2(p);
  p.y += (.5-sf)*.5 + .85;

  objID = step(n, p.y);

  return min(n, p.y);
}

float
mapRef (vec3 p)
{
  float sf = surfFunc(p);

  p.xy -= path(p.z);

  // Round tunnel: Euclidean distance: length(tun.xy)
  float n = 1. - length(p.xy*vec2(0.5, 0.7071)) + (0.5 - sf);

  // Square tunnel: Chebyshev(?) distance: max(abs(tun.x), abs(tun.y))
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.7071);
  //float n = 1.- max(tun.x, tun.y) + (0.5 - sf);

  // Rounded square tunnel.
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.7071);
  //float n = 1. - pow(dot(pow(tun, vec2(4)), vec2(1)), 1./4.) + (0.5 - sf);

  // Hexagonal tunnel.
  //vec2 tun = abs(p.xy)*vec2(0.5, 0.75);
  //float n = 1. - max(tun.x*.866025 + tun.y*.5, tun.y) + (0.5 - sf); //1.133975

  objID = 1.;

  return n;
}

float
bumpFunc (vec3 p)
{
  return gradN3D(p*8. + vec3(0, iTime, 0.));
}

vec3
bumpMap (vec3 p, vec3 n, float bumpfactor)
{
  const vec2 e = vec2(0.002, 0);
  float ref = bumpFunc(p);                 
  vec3 grad = (vec3(bumpFunc(p - e.xyy), bumpFunc(p - e.yxy), bumpFunc(p - e.yyx)) - ref)/e.x;

  grad -= n*dot(n, grad);

  return normalize( n + grad*bumpfactor );
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

    t += d*.86;
  }

  return min(t, FAR);
}

float
traceRef (vec3 ro, vec3 rd)
{
  float t = 0., d;

  for (int i=0; i<32; i++)
  {
    d = map(ro + rd*t);//*rDir;

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

  for (int i=0; i<32; i++)
  {
    d = mapRef(ro + rd*t);

    if((d<0. && abs(d)<0.002*(t*.25 + 1.)) || t>FAR)
      break;

    t += d;
  }

  return min(t, FAR);
}

float
softShadow (vec3 ro, vec3 lp, float k, float t)
{
  const int maxIterationsShad = 24; 

  vec3 rd = lp-ro;

  float shade = 1.;
  float dist = .0015*(t*.125 + 1.);
  float end = max(length(rd), 0.0001);
  rd /= end;

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));
    dist += clamp(h, .01, .2); 

    if (h<0. || dist > end)
      break;
  }

  return min(max(shade, 0.) + .2, 1.); 
}

vec3
getNormal (vec3 p, inout float edge, inout float crv)
{
  vec2 e = vec2(6./iResolution.y, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.0015, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

vec3
getNormalRefr (vec3 p, inout float edge, inout float crv)
{
  vec2 e = vec2(6./iResolution.y, 0);

  float d1 = mapRef(p + e.xyy), d2 = mapRef(p - e.xyy);
  float d3 = mapRef(p + e.yxy), d4 = mapRef(p - e.yxy);
  float d5 = mapRef(p + e.yyx), d6 = mapRef(p - e.yyx);
  float d = mapRef(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);
  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.003, 0); //iResolution.y - Depending how you want different resolutions to look.
  d1 = mapRef(p + e.xyy), d2 = mapRef(p - e.xyy);
  d3 = mapRef(p + e.yxy), d4 = mapRef(p - e.yxy);
  d5 = mapRef(p + e.yyx), d6 = mapRef(p - e.yyx);

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

vec3
getObjectColor (vec3 p, vec3 n)
{
  vec3 tx = tex3D(iChannel0, p/3., n );
  tx = smoothstep(.1, .5, tx);

  if(svObjID>.5)
  {
    tx *= vec3(1, .55, .35);

    tx *= smoothstep(.1, .6, surfFunc(p))*.6 + .4;

    // Alternative algae in the crevices.
    //float c = smoothstep(.1, .6, surfFunc(p));
    //tx *= vec3(c*c, c, c*c*c)*.6 + .4;
  }
  else
    tx *= vec3(.6, .8,  1);

  return tx;
}

vec3
doColor (vec3 sp, vec3 rd, vec3 sn, vec3 lp, float edge, float crv, float t)
{
  vec3 sceneCol = vec3(0);

  if(t<FAR)
  {
    vec3 ld = lp - sp;
    float lDist = max(length(ld), 0.001);
    ld /= lDist;

    float atten = 1.5/(1. + lDist*0.125 + lDist*lDist*0.025);

    float diff = max(dot(sn, ld), 0.);
    diff = pow(diff, 4.)*.66 + pow(diff, 8.)*.34;
    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.0);

    vec3 objCol = getObjectColor(sp, sn);

    sceneCol = (objCol*(diff + 0.7 + vec3(1, .6, .2)*spec*2.));

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

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5) / iResolution.y;

#ifdef THREE_D
  float sg = sign(gl_FragCoord.x - .5*iResolution.x);
  uv.x -= sg*.25*iResolution.x/iResolution.y;
#endif

  vec3 ro = vec3(0, 0, iTime*3.);
  vec3 lk = ro + vec3(0, 0, .25);

  vec3 lp = ro + vec3(0, .25, 3.);

  ro.xy += path(ro.z);
  lk.xy += path(lk.z);
  lp.xy += path(lp.z);

#ifdef THREE_D
  ro.x -= sg*.15; lk.x -= sg*.15; lp.x -= sg*.15;
#endif

  float FOV = 3.14159/3.;
  vec3 forward = normalize(lk-ro);
  vec3 right = normalize(vec3(forward.z, 0., -forward.x )); 
  vec3 up = cross(forward, right);

  vec3 rd = normalize(forward + (uv.x*right + uv.y*up)*FOV);

  float edge = 0., crv = 1.;

  float t = trace(ro, rd);
  svObjID = objID;
  float oSvObjID = svObjID;

  vec3 sp = ro + rd*t;

  vec3 sn = getNormal(sp, edge, crv);

  if(oSvObjID<0.5)
    sn = bumpMap(sp, sn, .02);

  float fr = clamp(1. + dot(rd, sn), 0., 1.);

  vec3 sceneColor = doColor(sp, rd, sn, lp, edge, crv, t);

  float sh = softShadow(sp + sn*.0011, lp, 16., t); // Set to "1.," if you can do without them.
  float ao = calculateAO(sp, sn);
  sh = (sh + ao*.3)*ao;

  float fog = smoothstep(0., .95, t/FAR);

  vec3 refl = reflect(rd, sn); // Standard reflection.
  vec3 refr = refract(rd, sn, 1./1.33); // Water refraction. Note the inverted index.

  vec3 refSp;

  t = traceRef(sp + sn*0.0015, refl);
  svObjID = objID;

  refSp = sp + refl*t;

  sn = getNormal(refSp, edge, crv);//*rDir;

  vec3 reflColor = doColor(refSp, refl, sn, lp, edge, crv, t);

#ifdef NO_RERACTION
  if(oSvObjID<0.5)
    sceneColor = sceneColor*.1 +  reflColor*(fr*fr);
  else
    sceneColor += reflColor*.5;
#else
  if(oSvObjID<0.5)
  {
    t = traceRefr(sp - sn*.0025, refr);
    svObjID = objID;

    refSp = sp + refr*t;

    sn = getNormalRefr(refSp, edge, crv);

    vec3 refrColor = doColor(refSp, refr, sn, lp, edge, crv, t);

    sceneColor = sceneColor*.2 + mix(refrColor, reflColor, pow(fr, 5.)*.8 + .2)*vec3(.6, .8, 1);
  }
  else
    sceneColor += reflColor*.5;
#endif

  sceneColor *= sh;

  vec3 fogCol = vec3(.7, .75, 1)*(rd.y*.25 + .75)*1.5;
  sceneColor = mix(sceneColor, fogCol, fog);

  sceneColor = pow(max(sceneColor, 0.), vec3(1.33))*1.66; // Adding a bit of contrast.

  uv = gl_FragCoord.xy/iResolution.xy;
  sceneColor *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .125)*.5 + .5;

  gl_FragColor = vec4(sqrt(clamp(sceneColor, 0.0, 1.0)), 1.0);
}
