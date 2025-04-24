#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform sampler2D iChannel0;

#define FAR 80.

float
hash31 (vec3 p)
{
  float n = dot(p, vec3(13.163, 157.247, 7.951)); 
  return fract(sin(n)*43758.5453); 
}

vec3 hash13 (float n) { return fract(vec3(2097152, 262144, 32768)*sin(n)); }

mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, s, -s, c); }

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

vec2
path (float z)
{
  vec2 a = vec2(sin(z*.055), cos(z*.07));
  return vec2(a.x - a.y*.75, a.y*1.275 + a.x*1.125); 
}

float
sBox (vec3 p, vec3 b)
{
  return length(max(abs(p) - b, 0.));
}

const float gSc = 1.5;

float
blocks (vec3 p, float rndZ)
{
  const float sc = gSc;

  p += sc*.5;

  float d = 1e5;

  for (int i=0; i<=1; i++)
    for (int j=0; j<=1; j++)
    {
      vec2 ip = floor(p.xy/sc - vec2(i, j)/2.)*sc + vec2(i, j)/2.*sc;

      vec3 q = vec3((mod(p.xy + vec2(i, j)/2.*sc, sc) - sc/2.), p.z);

      float rnd = (hash31(vec3(ip, rndZ))*15. + 1.)/64.;

      q -= vec3(0, 0, -rnd);

      float obj = sBox(q, vec3(sc/4. - .02, sc/4. - .02, rnd));

      d = min(d, obj);
    }

  return d;
    
}

float
map (vec3 p)
{
  p.xy -= path(p.z); 

  const float sc = 32.; // Section Z spacing.
  float ipZ = floor(p.z/sc); // Unique section ID.
  vec3 rnd3 = floor(vec3(4, 2, 4)*hash13(ipZ)); // Variable sized rooms.

  vec3 sq = sign(p);

  vec3 q = vec3(p.xy, mod(p.z, sc) - sc/2.);

  vec3 walls = abs(q - vec3(2, .5, 0)) - vec3(5, 3, 6) - rnd3;
  float s2 = walls.z;

  float d = -max(max(walls.x, walls.y), walls.z); 

  walls -= gSc/2.;

  float blX = blocks(vec3(q.yz, walls.x), sq.x);
  float blY = blocks(vec3(q.xz - gSc/4., walls.y), sq.y);
  float blZ = blocks(vec3(q.xy, walls.z), ipZ);

  d = min(d, min(min(blX, blY), blZ));

  q = p;
  float blXSm = blocks(vec3(q.yz - gSc/4., abs(q.x) - 1.5 - gSc/2.), sq.x);
  float blYSm = blocks(vec3(q.xz, abs(q.y) - 1.5 - gSc/2.), sq.y);

  q = abs(q);

  d = max(d, min(max(min(blXSm, blYSm), -s2), -max(q.x - 1.5, q.y - 1.5)));

  return d*.8;
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., d;
  for(int i = min(0, iFrame); i<96; i++)
  {
    d = map(ro+rd*t);
    if(abs(d)<.001*(t*.125 + 1.) || t>FAR)
      break;
    t += d;
  }

  return min(t, FAR);
}

vec3
getNormal (vec3 p)
{
  const vec2 e = vec2(.005, 0);

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

  return normalize(vec3(mp[0] - mp[1], mp[2] - mp[3], mp[4] - mp[5]));
}

float
getShad (vec3 ro, vec3 n, vec3 lp)
{
  const float eps = .001;

  float t = 0., sh = 1., dt;

  ro += n*eps*1.1;

  vec3 ld = (lp - ro);
  float lDist = length(ld);
  ld /= lDist;

  for(int i = min(0, iFrame); i<24; i++)
  {
    dt = map(ro + ld*t);

    sh = min(sh, 12.*dt/t);

    t += clamp(dt, .02, .5);
    if(dt<0. || t>lDist)
      break;
  }

  return max(sh, 0.);
}

float
getAO (vec3 p, vec3 n)
{
  float sca = 2., occ = 0.;

  for(int i = min(0, iFrame); i<5; i++)
  {
      float hr = .01 + float(i)*.5/4.;        
      float dd = map(p + n*hr);
      occ += (hr - dd)*sca;
      sca *= .7;
      if(occ>1e5)
        break;
  }

  return clamp(1. - occ, 0., 1.);    
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  vec3 ro = vec3(0, 0, iTime*4.);
  vec3 lk = ro + vec3(0, 0, .25);
 
  vec3 lp = ro + vec3(0., .25, 6);
  vec3 lp2 = ro + vec3(0., .125, 16);

  lk.xy += path(lk.z);
  ro.xy += path(ro.z);
  lp.xy += path(lp.z);
  lp2.xy += path(lp2.z);

  float FOV = 3.14159265/3.;
  vec3 fwd = normalize(lk - ro);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt);

  vec3 rd = normalize(fwd + (uv.x*rgt + uv.y*up)*FOV);
    
  // Fish eye lens.
  //vec3 rd = normalize(forward + (uv.x*right + uv.y*up)*FOV);
  //rd = normalize(vec3(rd.xy, rd.z - dot(rd.xy, rd.xy)*.15));    
    
  rd.xy = r2( path(lk.z).x/32. )*rd.xy;
    
  float t = trace(ro, rd);
    
  vec3 col = vec3(0);
  
  if(t<FAR)
  {
    vec3 sp = ro + rd*t;
    vec3 sn = getNormal(sp);

    const float txSc0 = .25;
    vec3 txP = vec3(sp.xy - path(sp.z), sp.z);

    float ao = getAO(sp, sn);
    float sh = getShad(sp, sn, lp2);
    sh = min(sh + ao*.3, 1.);

    vec3 ld = lp - sp;
    vec3 ld2 = lp2 - sp;

    float distlpsp = max(length(ld), .001);
    float distlpsp2 = max(length(ld2), .001);
    ld /= distlpsp;
    ld2 /= distlpsp2;

    float atten = 1.5/(1. + distlpsp*distlpsp*.25); // + distlpsp*distlpsp*0.025
    float atten2 = 3./(1. + distlpsp2*distlpsp2*.25); // + distlpsp*distlpsp*0.025

    float amb = ao*.35;

    float diff = max( dot(sn, ld), 0.);
    float diff2 = max( dot(sn, ld2), 0.);        

    float spec = pow(max( dot( reflect(-ld, sn), -rd ), 0.0 ), 32.);
    float spec2 = pow(max( dot( reflect(-ld2, sn), -rd ), 0.0 ), 32.);

    vec3 oCol = tex3D(iChannel0, txP*txSc0, sn);
    oCol = smoothstep(0., .5, oCol);

    col = (oCol*(diff*vec3(.4, .6, 1) + amb + vec3(.4, .6, 1)*spec*4.))*atten;
    col += (oCol*(diff2*vec3(1, .4, .2) + amb + vec3(1, .4, .2)*spec2*4.))*atten2;

    col *= ao*sh;
  }
  //col = mix(col, vec3(1.8, 1, .9), smoothstep(.2, .99, t/FAR));
    
  uv = gl_FragCoord.xy/iResolution.xy;
  col = mix(col, vec3(0), (1. - pow(16.*uv.x*uv.y*(1.-uv.x)*(1.-uv.y), .0625)));
  
  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.0);
}
