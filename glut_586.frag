#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

#define FAR 20.

float hash (float n) { return fract(cos(n)*45758.5453); }

mat2 rot2 (float a) { vec2 v = sin(vec2(1.570796, 0) + a);	return mat2(v, -v.y, v.x); }

vec3 camPath (float t) { return vec3(sin(t * 0.45)*.75, cos(t * 0.75)*.75, t); }

vec3
tpl (sampler2D t, vec3 p, vec3 n)
{
  n = max(abs(n), 0.001);
  n /= (n.x + n.y + n.z );  
  vec3 tx = (texture(t, p.yz)*n.x + texture(t, p.zx)*n.y + texture(t, p.xy)*n.z).xyz;

  return tx*tx;
}

#define t2D texture
vec3
tpl4 (sampler2D a, sampler2D b, sampler2D c, vec3 p, vec3 n )
{
  n = max(abs(n), 0.001);
  n /= (n.x + n.y + n.z );

  float h = dot(cos(p*6.), sin(p.yzx*6.));

  vec3 tx  = (t2D(a, p.yz)*n.x + t2D(a, p.zx)*n.y + t2D(a, p.xy)*n.z).xyz; // Pink sandstone.
  vec3 tx2 = (t2D(b, p.yz)*n.x + t2D(b, p.zx)*n.y + t2D(b, p.xy)*n.z).xyz; // Sandstone.
  vec3 tx3 = 1.-(t2D(c, p.yz)*n.x + t2D(c, p.zx)*n.y + t2D(c, p.xy)*n.z).zyx; // Pink coral.

  tx = mix(tx*tx, tx2*tx2, h*.5 + .5);

  h = dot(sin(p*5.), cos(p.zxy*5.));

  tx2 = mix(tx3*tx3, tx2*tx2, h*.5 + .5);

  return mix(tx, tx2, dot(sin(p*2.), sin(p.zxy*2.))*.5 + .5);
}

float
drawObject (vec3 p)
{
  p = cos(p*6.2831853) + 1.;
  return dot(p, p);
}

float
cellTile (vec3 p)
{
  vec3 d;

  d.x = drawObject(p - vec3(.81, .62, .53));
  p.xy = vec2(p.y-p.x, p.y + p.x)*.7071;
  d.y = drawObject(p - vec3(.2, .82, .64));
  p.yz = vec2(p.z-p.y, p.z + p.y)*.7071;
  d.z = drawObject(p - vec3(.41, .06, .70));

  return 1.- min(min(d.x, d.y), d.z)*.1666;
}

float
map (vec3 p)
{
  float b = cellTile(p*3.); 
  //float b = cellTile(p + iTime/16.);

  p.xy -= camPath(p.z).xy; 

  p += (sin(p*3.14159 - sin(p.zyx*3.14159)*3.14159*1.25))*.1;

  float n = abs(dot(cos(p*3.14159), sin(p.yzx*3.14159)));

  return .45 - n*.33  - b*.1;
}
 
float
cao (vec3 pos, vec3 nor)
{
  float sca = 1.5, occ = 0.0;
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
nr (vec3 p)
{
  vec2 e = vec2(-1, 1)*.001;   
  return normalize(e.yxx*map(p + e.yxx) + e.xxy*map(p + e.xxy) + 
                   e.xyx*map(p + e.xyx) + e.yyy*map(p + e.yyy) );   
}

float
trace (vec3 ro, vec3 rd)
{
  float t = 0., h;
  for(int i = 0; i < 128; i++)
  {
    h = map(ro+rd*t);
    if(abs(h)<0.0015*(t*.125 + 1.) || t>FAR)
      break;
    t += (step(1., h)*.25 + .5)*h;
  }

  return min(t, FAR);
}

float
sha (vec3 ro, vec3 rd, float start, float end, float k)
{
  float shade = 1.0;
  const int shadIter = 24; 

  float dist = start;

  for (int i=0; i<shadIter; i++)
  {
    float h = map(ro + rd*dist);
    shade = min(shade, smoothstep(0.0, 1.0, k*h/dist)); // Subtle difference. Thanks to IQ for this tidbit.

    dist += clamp(h, 0.02, 0.16);
        
    if ((h)<0.001 || dist > end)
      break; 
  }
    
  return min(max(shade, 0.) + 0.3, 1.0); 
}

float
bumpSurf3D (vec3 p)
{
  float vor = cellTile(p*27.);

  return pow(max(vor, 0.), 18.);
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

  vec3 g = vec3(0.299, 0.587, 0.114)*m; // Converting to greyscale.
  g = (g - dot(tpl(tx,  p , n), vec3(0.299, 0.587, 0.114)) )/e.x; g -= n*dot(n, g);

  return normalize( n + g*bf ); // Bumped normal. "bf" - bump factor.
}

void
main ()
{
  vec2 u = (gl_FragCoord.xy - iResolution.xy*0.5)/iResolution.y;

  u += sin(u*32. + cos(u.yx*16. + iTime*4.))*.0035;

  vec3 o = camPath(iTime*.5);
  vec3 lk = camPath(iTime*.5 + .1);
  vec3 l = camPath(iTime*.5 + 1.5) + vec3(.0, .0, 0.);

  float FOV = 3.14159/3.;
  vec3 fwd = normalize(lk-o);
  vec3 rgt = normalize(vec3(fwd.z, 0., -fwd.x )); 
  vec3 up = cross(fwd, rgt);

  vec3 r = normalize(fwd + FOV*(u.x*rgt + u.y*up));

  r.xy = rot2( camPath(lk.z).x/16. )*r.xy;

  float t = trace(o, r);

  vec3 col = vec3(0);

  if(t<FAR)
  {
    vec3 p = o + r*t, n = nr(p);

    float sz = 1./1.;
    n = db(iChannel0, p*sz, n, .02/(1. + t/FAR));

    n = dbF(p*sz, n, .01);

    l -= p;
    float d = max(length(l), 0.001);
    l /= d;

    float ao =  cao(p, n);
    float sh = sha(p, l, 0.04, d, 4.);

    float di = max(dot(l, n), 0.);
    float sp = pow(max( dot( reflect(r, n), l ), 0.0 ), 8.);
    float fr = clamp(1.0 + dot(r, n), 0.0, 1.0);

    vec3 tx = tpl(iChannel0, p*sz, n);
    //vec3 tx = tpl4(iChannel0, iChannel1, iChannel2, p*sz, n);

    float c = dot(tx, vec3(0.299, 0.587, 0.114));

    tx += vec3(c*c*.8, c, c*c*.5)*fr;

    col = tx*(di + .1 + sp)+ tx*fr*2.;
    col *= 1./(1. + d*.125 + d*d*.025)*ao*sh;
  }

  vec3 bg = vec3(.5, .7, 1);
  col = mix(clamp(col, 0., 1.), bg, smoothstep(0., FAR-5., t));

  gl_FragColor = vec4(sqrt(clamp(col, 0., 1.)), 1.);
}
