#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#if HW_PERFORMANCE==0
#define AA 1
#else
#define AA 2   // make this 2 is you have a fast computer
#endif

vec3
hexagon_pattern (vec2 p)
{
  vec2 q = vec2( p.x*2.0*0.5773503, p.y + p.x*0.5773503 );
  
  vec2 pi = floor(q);
  vec2 pf = fract(q);

  float v = mod(pi.x + pi.y, 3.0);

  float ca = step(1.0,v);
  float cb = step(2.0,v);
  vec2  ma = step(pf.xy,pf.yx);
  
  return vec3( pi + ca - cb*ma, dot( ma, 1.0-pf.yx + ca*(pf.x+pf.y-1.0) + cb*(pf.yx-2.0*pf.xy) ) );
}

const vec2 torus = vec2(0.5,0.2);

#define NumTiles 3

float
map (vec3 p)
{
  float d = length( vec2(length(p.xz)-torus.x,p.y) )-torus.y;

  vec2 uv = vec2(atan(p.z,p.x),atan(length(p.xz)-torus.x,p.y) )*vec2(3.0*sqrt(3.0),2.0)/3.14159;
  uv = uv*float(NumTiles) + vec2(0.0,0.5*iTime);
  vec3 h = hexagon_pattern( uv );
  float f = mod(h.x+2.0*h.y,3.0)/2.0;
  d += f*0.04;

  return d;
}

vec2
castRay (vec3 ro, vec3 rd)
{
  float tmax = (-torus.y-ro.y)/rd.y;

  float t = 0.7;
  float m = 2.0;
  for( int i=0; i<256; i++ )
  {
    float precis = 0.0004*t;
    float res = map( ro+rd*t );
    if( res<precis || t>tmax )
      break;
    t += res*0.25;
  }

  if( t>tmax ) { t=tmax; m=1.0; }
  return vec2( t, m );
}

float
calcSoftshadow (vec3 ro, vec3 rd)
{
  float res = 1.0;
  float t = 0.02;
  for( int i=0; i<128; i++ )
  {
    float h = map( ro + rd*t )*0.2;
    res = min( res,24.0*h/t );
    t += clamp( h, 0.005, 0.02 );
    if( res<0.001 || t>0.8 )
      break;
  }

  return smoothstep( 0.0, 1.0, res );
}

vec3
calcNormal (vec3 pos)
{
  vec2 e = vec2(1.0,-1.0)*0.5773*0.0005;
  return normalize( e.xyy*map( pos + e.xyy ) + 
                    e.yyx*map( pos + e.yyx ) + 
                    e.yxy*map( pos + e.yxy ) + 
                    e.xxx*map( pos + e.xxx ) );
}

vec3
render (vec3 ro, vec3 rd)
{ 
  vec2  res = castRay(ro,rd);
  float t   = res.x;
  vec3  pos = ro + t*rd;
  vec3  nor = vec3(0.0,1.0,0.0);
  float occ = 1.0;
  float ks  = 0.0;
  vec3  col = vec3(0.0,0.0,0.0);

  if( res.y<1.5 )
  {
    vec3 te = texture(iChannel0,pos.xz*0.5).xyz;
    col = 0.02 + te*0.2;
    ks = te.x;
    occ = smoothstep(0.06,0.4, abs(length(pos.xz)-torus.x) );
  }
  else
  {
    nor = calcNormal( pos );
    occ = 0.5+0.5*nor.y;

    vec2 uv = vec2( atan(pos.z,pos.x), atan(length(pos.xz)-torus.x,pos.y) )*
              vec2(3.0*sqrt(3.0), 2.0)/3.14159;
    uv = uv*float(NumTiles) + vec2(0.0,0.5*iTime);
    vec3 h = hexagon_pattern( uv );
    float f = mod(h.x+2.0*h.y,3.0)/2.0;

    col = vec3(1.0-f);
    float hh = abs(sin(10.0*mod(h.x+h.y*2.0,24.0)));
    if( f<0.1 ) col = 0.12+0.12*cos(hh+vec3(0.0,2.0,2.0));

    col *= 0.7+h.z;

    vec3 te = texture( iChannel0, 0.2*uv ).xyz;
    col *= 0.08+0.72*te;
    ks = te.x;
  }

  vec3  lig = normalize( vec3(0.4, 0.5, -0.6) );
  vec3  hal = normalize( lig-rd );
  float amb = clamp( 0.5+0.5*nor.y, 0.0, 1.0 );
  float dif = clamp( dot( nor, lig ), 0.0, 1.0 );
  float bac = clamp( dot( nor, normalize(vec3(-lig.x,0.0,-lig.z))), 0.0, 1.0 )*clamp( 1.0-pos.y,0.0,1.0);

  dif *= calcSoftshadow( pos, lig );

  float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ),32.0)*
                   dif *
                   (0.04 + 0.96*pow( clamp(1.0+dot(hal,rd),0.0,1.0), 5.0 ));

  vec3 lin = vec3(0.0);
  lin += 3.00*dif*vec3(1.10,0.90,0.60);
  lin += 0.40*amb*vec3(0.30,0.60,1.50)*occ;
  lin += 0.30*bac*vec3(0.40,0.30,0.25)*occ;
  col = col*lin;
  col += 5.00*spe*vec3(1.15,0.90,0.70)*ks*ks*4.0;

  return col;
}

void
main ()
{
  vec2 mo = iMouse.xy/iResolution.xy;

  vec3 ro = vec3( 1.1*cos(0.05*iTime + 6.0*mo.x), 0.9, 1.1*sin(0.05*iTime + 6.0*mo.x) );
  vec3 ta = vec3( 0.0, -0.2, 0.0 );
  vec3 cw = normalize(ta-ro);
  vec3 cu = normalize( cross(cw,vec3(0.0, 1.0,0.0)) );
  vec3 cv = normalize( cross(cu,cw) );

  vec3 tot = vec3(0.0);
#if AA>1
  for( int m=0; m<AA; m++ )
  for( int n=0; n<AA; n++ )
  {
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (-iResolution.xy + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else
    vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
#endif

    vec3 rd = normalize( p.x*cu + p.y*cv + 2.0*cw );

    vec3 col = render( ro, rd );

    col = pow( col, vec3(0.4545) );

    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  tot = 1.25*pow(tot,vec3(0.65,0.9,1.0) );

  vec2 q = gl_FragCoord.xy/iResolution.xy;
  tot *= 0.3 + 0.7*pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.25);

  gl_FragColor = vec4( tot, 1.0 );
}
