#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

//#define USE_GRADIENTS

vec3
mod289 (vec3 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4
mod289 (vec4 x)
{
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4
permute (vec4 x)
{
  return mod289(((x*34.0)+1.0)*x);
}

vec4
taylorInvSqrt (vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

vec3
fade (vec3 t)
{
  return t*t*t*(t*(t*6.0-15.0)+10.0);
}

float
cnoise (vec3 P)
{
  vec3 Pi0 = floor(P);
  vec3 Pi1 = Pi0 + vec3(1.0);
  Pi0 = mod289(Pi0);
  Pi1 = mod289(Pi1);
  vec3 Pf0 = fract(P);
  vec3 Pf1 = Pf0 - vec3(1.0);
  vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
  vec4 iy = vec4(Pi0.yy, Pi1.yy);
  vec4 iz0 = Pi0.zzzz;
  vec4 iz1 = Pi1.zzzz;

  vec4 ixy = permute(permute(ix) + iy);
  vec4 ixy0 = permute(ixy + iz0);
  vec4 ixy1 = permute(ixy + iz1);

  vec4 gx0 = ixy0 * (1.0 / 7.0);
  vec4 gy0 = fract(floor(gx0) * (1.0 / 7.0)) - 0.5;
  gx0 = fract(gx0);
  vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
  vec4 sz0 = step(gz0, vec4(0.0));
  gx0 -= sz0 * (step(0.0, gx0) - 0.5);
  gy0 -= sz0 * (step(0.0, gy0) - 0.5);

  vec4 gx1 = ixy1 * (1.0 / 7.0);
  vec4 gy1 = fract(floor(gx1) * (1.0 / 7.0)) - 0.5;
  gx1 = fract(gx1);
  vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
  vec4 sz1 = step(gz1, vec4(0.0));
  gx1 -= sz1 * (step(0.0, gx1) - 0.5);
  gy1 -= sz1 * (step(0.0, gy1) - 0.5);

  vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
  vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
  vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
  vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
  vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
  vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
  vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
  vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

  float n000 = dot(g000, Pf0);
  float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
  float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
  float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
  float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
  float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
  float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
  float n111 = dot(g111, Pf1);

  vec3 fade_xyz = fade(Pf0);
  vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
  vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
  float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x); 
  return 2.2 * n_xyz;
}

vec4
texCube (sampler2D sam, vec3 p, vec3 n, float k)
{
  vec4 x = texture( sam, p.yz );
  vec4 y = texture( sam, p.zx );
  vec4 z = texture( sam, p.xy );
  vec3 w = pow( abs(n), vec3(k) );
  return (x*w.x + y*w.y + z*w.z) / (w.x+w.y+w.z);
}

float
rand2 (vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float
rand3 (vec3 co)
{
  return fract(sin(dot(vec2(rand2(co.xy), rand2(co.xz)) ,vec2(12.9897,78.243))) * 44758.5453);
}

float
sqr (float x)
{
  return x*x;
}

vec4
map2 (vec3 p)
{
  float v = 2.0;

  float wiggle = iTime * 0.05;

  float wiggle1 = wiggle * 2.0;
  float wiggle2 = wiggle * 3.0;
  float wiggle3 = wiggle * 5.0;

  p.x += 0.5*sin( v*p.y + wiggle1 );
  p.y += 0.5*sin( v*p.z + wiggle2 );
  p.z += 0.5*sin( v*p.x + wiggle3 );
  p.x += 0.5*sin( v*p.y + wiggle );
  p.y += 0.5*sin( v*p.z + wiggle );
  p.z += pow(cnoise(p*10.0),3.0)*4.0;
  p.z += 0.5*sin( v*p.x + wiggle );
  p.x += 0.5*sin( v*p.y + wiggle );
  p.y += 0.5*sin( v*p.z + wiggle );
  p.z += 0.5*sin( v*p.x + wiggle );
  p.x += 0.5*sin( v*p.y + wiggle );
  p.y += 0.5*sin( v*p.z + wiggle );
  p.z += 0.5*sin( v*p.x + wiggle );

  float d1 = length(p) - 1.0*smoothstep(0.0,2.0,iTime);;
  d1 *= 0.02;	

  return vec4( d1, p );
}

vec4
map (vec3 p)
{
  vec4 res = map2(p);

  float d2 = p.y - 0.5;
  if( d2<res.x ) res = mix(res, vec4( d2, 0.0, 0.0, 0.0 ), smoothstep(0.0, 1.0, -d2*0.1));

  return res;
}

vec4
intersect (vec3 ro, vec3 rd, float maxd)
{
  vec3 res = vec3(-1.0);
  float precis = 0.00005;
  float t = 1.0;
  for( int i=0; i<1024; i++ )
  {
    vec4 tmp = map( ro+rd*t );
    res = tmp.yzw;
    float h = tmp.x;
    if( h<precis||t>maxd )
      break;
    t += h;
  }

  return vec4( t, res );
}

vec3
calcNormal (vec3 pos)
{
#ifdef USE_GRADIENTS    
  return normalize( cross(dFdx(pos),dFdy(pos)) );
#else    
  vec2 e = vec2(1.0,-1.0)*0.001;
  return normalize( e.xyy*map( pos + e.xyy ).x + 
                    e.yyx*map( pos + e.yyx ).x + 
                    e.yxy*map( pos + e.yxy ).x + 
                    e.xxx*map( pos + e.xxx ).x );
#endif    
}

float
softshadow (vec3 ro, vec3 rd, float mint, float k)
{
  float res = 1.0;
  float t = mint;
  float h = 1.0;
  for( int i=0; i<128; i++ )
  {
    h = map(ro + rd*t).x;
    res = min( res, k*h/t );
    if( res<0.0001 )
      break;
    t += clamp( h, 0.01, 0.05 );
  }
  return clamp(res,0.0,1.0);
}

float
calcOcc (vec3 pos, vec3 nor)
{
  const float h = 0.2;
  float ao = 0.0;
  for( int i=0; i<8; i++ )
  {
    vec3 dir = sin( float(i)*vec3(1.0,7.13,13.71)+vec3(0.0,2.0,4.0) );
    dir *= sign(dot(dir,nor));
    float d = map2( pos + h*dir ).x;
    ao += max(0.0,h-d*2.0);
  }
  return clamp( 4.0 - 2.5*ao, 0.0, 1.0 )*(0.5+0.5*nor.y);
}

vec3 lig = normalize(vec3(1.0,0.7,0.9));

void
main ()
{
  vec2 q = gl_FragCoord.xy / iResolution.xy;
  vec2 p = -1.0 + 2.0 * q;
  p.x *= iResolution.x/iResolution.y;
  vec2 m = vec2(0.5);
  if( iMouse.z>0.0 ) m = iMouse.xy/iResolution.xy;

  float an = 0.005*iTime + 7.5 - 5.0*m.x;

  vec3 ro = vec3(4.5*sin(an),0.5,4.5*cos(an));
  vec3 ta = vec3(0.0,0.5,0.0);

  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
  vec3 vv = normalize( cross(uu,ww));

  vec3 rd = normalize( p.x*uu + p.y*vv + 2.0*ww );

  vec3 col = vec3(0.0);
  const float maxd = 9.0;
  vec4  inn = intersect(ro,rd,maxd);
  float t = inn.x;
  if( t<maxd )
  {
    vec3 tra = inn.yzw;

    vec3 pos = ro + t*rd;
    vec3 nor = calcNormal(pos);
    vec3 ref = reflect( rd, nor );

    col = vec3(0.3,0.3,0.3);
    if(true || pos.y>-0.99)
      col += 0.2*tra;
    vec3 pat = texCube( iChannel0, 0.5*pos, nor, 4.0 ).xyz;
    col *= pat;
    col *= 0.5;

    float occ = calcOcc( pos, nor );

    float amb = 0.5 + 0.5*nor.y;
    float dif = max(dot(nor,lig),0.0);
    float bou = max(0.0,-nor.y);
    float bac = max(0.2 + 0.8*dot(nor,-lig),0.0);
    float sha = 0.0; if( dif>0.01 ) sha=softshadow( pos+0.01*nor, lig, 0.0005, 128.0 );
    float fre = pow( clamp( 1.0 + dot(nor,rd), 0.0, 1.0 ), 3.0 );
    float spe = 15.0*pat.x*max( 0.0, pow( clamp( dot(lig,reflect(rd,nor)), 0.0, 1.0), 16.0 ) )*dif*sha*(0.04+0.96*fre);

    vec3 lin = vec3(0.0);
    lin += 3.5*dif*vec3(6.00,4.00,3.00)*pow(vec3(sha),vec3(1.0,1.2,1.5));
    lin += 1.0*amb*vec3(0.30,0.30,0.30)*occ;
    lin += 1.0*bac*vec3(0.80,0.50,0.20)*occ;
    lin += 1.0*bou*vec3(1.00,0.30,0.20)*occ;
    lin += 4.0*fre*vec3(1.00,0.80,0.70)*(0.3+0.7*dif*sha)*occ;
    lin += spe*2.0;

    col = col*lin + spe;

    col *= min(200.0*exp(-1.5*t),1.0);
    col *= 1.0-smoothstep( 1.0,6.0,length(pos.xz) );
  }

  col = pow( clamp(col,0.0,1.0), vec3(0.4545) );

  col = pow( col, vec3(0.6,1.0,1.0) );

  col *= pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.1 );

  gl_FragColor = vec4( col, 1.0 );
}
