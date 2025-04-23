#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;

#define FAR 150. // Far plane, or maximum distance.

float objID = 0.;

mat2 rot2 (float a) { vec2 v = sin(vec2(1.570796, 0) - a);	return mat2(v, -v.y, v.x); }

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
  p.xy = vec2(p.y-p.x, p.y + p.x)*.7071;
  d.y = drawObject(p - vec3(.39, .2, .11));
  p.yz = vec2(p.z-p.y, p.z + p.y)*.7071;
  d.z = drawObject(p - vec3(.62, .24, .06));
  p.xz = vec2(p.z-p.x, p.z + p.x)*.7071;
  d.w = drawObject(p - vec3(.2, .82, .64));

  v.xy = min(d.xz, d.yw);//, v.z = min(max(d.x, d.y), max(d.z, d.w)), v.w = max(v.x, v.y); 

  d.x =  min(v.x, v.y); // First order.

  return d.x*2.66;
}

float
smax (float a, float b, float k)
{
  float f = max(0., 1. - abs(b - a)/k);
  return max(a, b) + k*.25*f*f;
}

vec3 tri (vec3 x) { return abs(fract(x)-.5); }
vec3 trap (vec3 x, float pf) { return (tri(x - pf*.125) + tri(x + pf*.125))*.5; }

float
surfFunc (vec3 p)
{
  p /= 2.75;
  return dot(trap(p + trap(p.zxy, .666), .666), vec3(0.666));
}

float
surfFunc2 (vec3 p)
{
  return dot(sin(p + sin(p.yzx*2. + iTime*2.)), vec3(.1666)) + .5; 
}

vec3
tpl (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n), 0.001);
  n /= (n.x + n.y + n.z );  
  p = (texture(t, p.yz)*n.x + texture(t, p.zx)*n.y + texture(t, p.xy)*n.z).xyz;
  return p*p;
}

vec3
camPath (float t)
{
  float a = sin(t * 0.11);
  float b = cos(t * 0.14);
  return vec3(a*4. -b*1.5, b*1.2 + a*1., t);
}

float
map (vec3 p)
{
  float sea = p.y + 3.5;
  float sf = surfFunc(p);

  float cav = abs(dot(cos(p*3.14159/6.), sin(p.yzx*3.14159/6.)) + 1.5);

  p.xy -= camPath(p.z).xy;

  float tun = 2. - length(p.xy);

  tun = smax(tun, 1.-cav, 1.) + .35 + (.5-sf);

  sf = surfFunc2(p);
  sea += (.5-sf)*.5;

  objID = step(sea, tun);

  return min(sea, tun);
}

float
cao (vec3 p, vec3 n)
{
  float sca = 1.5, occ = 0.;
  for(float i=0.; i<5.; i++)
  {
    float hr = .01 + i*.5/4.;        
    float dd = map(n * hr + p);
    occ += (hr - dd)*sca;
    sca *= 0.7;
  }
  return clamp(1.0 - occ, 0., 1.);    
}

vec3
nr (vec3 p, inout float edge)
{
  vec2 e = vec2(.015, 0);

  float d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  float d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  float d5 = map(p + e.yyx), d6 = map(p - e.yyx);
  float d = map(p)*2.;

  edge = abs(d1 + d2 - d) + abs(d3 + d4 - d) + abs(d5 + d6 - d);

  edge = smoothstep(0., 1., sqrt(edge/e.x*2.));

  e = vec2(.0025, 0);
  d1 = map(p + e.xyy), d2 = map(p - e.xyy);
  d3 = map(p + e.yxy), d4 = map(p - e.yxy);
  d5 = map(p + e.yyx), d6 = map(p - e.yyx); 

  return normalize(vec3(d1 - d2, d3 - d4, d5 - d6));
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0.0, h;
  for(int i = 0; i < 128; i++)
  {
    h = map(ro+rd*t);
    if(abs(h)<0.002*(t*.25 + 1.) || t>FAR)
      break;
    t += h*.85;
  }

  return min(t, FAR);
}

float
sha (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.0;
  const int maxIterationsShad = 24; 

  float dist = start;
  float stepDist = end/float(maxIterationsShad);

  for (int i=0; i<maxIterationsShad; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist));

    dist += clamp(abs(h), 0.01, 0.25);

    if (h<0.001 || dist > end) break; 
  }

  return min(max(shade, 0.) + 0.2, 1.0); 
}

float
bumpSurf3D (vec3 p)
{
  return cellTile(p*.5)*.7 + cellTile(p)*.3;
}

vec3
dbF (vec3 p, vec3 nor, float bumpfactor)
{
  const vec2 e = vec2(0.001, 0);
  float ref = bumpSurf3D(p);
  vec3 grad = (vec3(bumpSurf3D(p - e.xyy),
                    bumpSurf3D(p - e.yxy),
                    bumpSurf3D(p - e.yyx) )-ref)/e.x;

  grad -= nor*dot(nor, grad);          

  return normalize( nor + grad*bumpfactor );
}

vec3
db (sampler2D tx, vec3 p, vec3 n, float bf)
{
  const vec2 e = vec2(0.001, 0);

  mat3 m = mat3( tpl(tx, p - e.xyy, n), tpl(tx, p - e.yxy, n), tpl(tx, p - e.yyx, n));

  vec3 g = vec3(0.299, 0.587, 0.114)*m;
  g = (g - dot(tpl(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

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
eMap (vec3 rd, vec3 sn)
{
  vec3 sRd = rd;

  rd.xy -= iTime*.25;
  rd *= 3.;

  float c = n3D(rd)*.57 + n3D(rd*2.)*.28 + n3D(rd*4.)*.15;
  c = smoothstep(0.4, 1., c);

  vec3 col = vec3(c, c*c, c*c*c*c).zyx;

  return mix(col, col.yzx, sRd*.25+.25); 
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  float speed = 6.;
  vec3 o = camPath(iTime*speed);
  vec3 lk = camPath(iTime*speed + .1);
  vec3 l = camPath(iTime*speed + 4.);

  vec3 loffs =  vec3(0, .25, 0);
  vec2 a = sin(vec2(1.57, 0) - l.z*1.57/10.);
  l += loffs;

  float FOV = 3.14159/3.;
  vec3 fwd = normalize(lk-o);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt);

  vec3 r = normalize(fwd + FOV*(u.x*rgt + u.y*up));

  r.xy = rot2(-camPath(lk.z).x/16. )*r.xy;

  float t = trace(o, r);

  float sObjID = objID;

  vec3 col = vec3(0);

  if(t<FAR)
  {
    vec3 p = o + r*t;

    float ed;
    vec3 n = nr(p, ed);

    vec3 svn = n;

    float sz = .5;
    if(sObjID>.5)
    {
      sz = .25;
      n = dbF(p, n, .25);
      n = db(iChannel1, p*sz, n, .005/(1. + t/FAR));
    }
    else
      n = db(iChannel0, p*sz, n, .02/(1. + t/FAR));

    l -= p;
    float d = max(length(l), 0.001);
    l /= d;

    float at = 1./(1. + d*.25 + d*d*.05);

    float ao =  cao(p, n);
    float sh = sha(p, l, 0.04, d, 16.);

    float di = max(dot(l, n), 0.);
    float sp = pow(max( dot( reflect(r, n), l ), 0.0 ), 16.);
    float fr = pow(clamp(1.0 + dot(r, n), 0.0, 1.0), 2.);

    vec3 tx;
    if(sObjID<.5)
    {
      tx = tpl(iChannel0, p*sz, n)*1.35;
      tx *= mix(vec3(1), vec3(1.35, 1, .65), abs(n));
    }
    else
      tx = tpl(iChannel1, p*sz, n)*vec3(2, 2.4, 2.8); // Sea water texturing.

    float sf;
    if(sObjID<.5)
      sf = surfFunc(p); 
    else
    {
      vec3 txp = p;
      txp.xy -= camPath(txp.z).xy;
      sf = surfFunc2(txp)*.8 + .2;
      sf *= bumpSurf3D(p)*.8 + .2;
    }

    tx *= sf;

    col = tx*(di + vec3(.75, .75, 1)) + vec3(.5, .7, 1)*sp;
    col += (tx*.5 + .125)*vec3(.5, .7, 1)*fr;

    col *= 1. - ed*.75;

    vec3 ref, refr;
    vec3 em; 

    if(sObjID>.5)
    {
      ref = reflect(r, svn*.5 + n*.5);
      em = eMap(ref, n);
      ref = tpl(iChannel0, ref, n)*em*4.;
      refr = refract(r, svn*.5 + n*.5, 1./1.33);
      em = eMap(refr, n); 
      refr = tpl(iChannel0, refr, n)*em*2.;
      col += mix(refr, ref, fr*fr)*8.;
    }
    else
    {
      ref = reflect(r, svn*.75 + n*.25);
      em = eMap(ref, n);
      col += col*em*3.;
    }

    col *= ao*sh*at;
  }

  if(t>=FAR)
    l = normalize(l - o - r*FAR);

  vec3 bg = mix(vec3(.5, .7, 1), vec3(1, .5, .6), l.y*.5 + .5);
  col = mix(clamp(col, 0., 1.), bg.yzx, smoothstep(0., FAR-2., t));

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
