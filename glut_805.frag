uniform vec2 iResolution;
uniform float iTime;

#define ANTIALIASING

vec2 sincos( float x ) { return vec2( sin(x), cos(x) ); }

vec2
sdSegment (vec3 p, vec3 a, vec3 b)
{
  vec3 pa = p-a, ba = b-a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return vec2( length( pa-ba*h ), h );
}

vec3 opU (vec3 d1, vec3 d2) { return (d1.x<d2.x) ? d1 : d2; }

vec3
map (vec3 p)
{
  vec2 id = floor( (p.xz+1.0)/2.0);
  p.xz = mod( p.xz+1.0, 2.0 ) - 1.0;

  float ph = sin(0.5 + 3.1*id.x + sin(7.1*id.y));

  p.xz += 0.5*sincos(1.0+0.5*iTime+(p.y+11.0*ph)*0.8);

  vec3 p1 = p; p1.xz += 0.15*sincos(1.0*p.y-1.0*iTime+0.0);
  vec3 p2 = p; p2.xz += 0.15*sincos(1.0*p.y-1.0*iTime+2.0);
  vec3 p3 = p; p3.xz += 0.15*sincos(1.0*p.y-1.0*iTime+4.0);

  vec2 h1 = sdSegment(p1, vec3(0.0,-50.0, 0.0), vec3(0.0, 50.0, 0.0) );
  vec2 h2 = sdSegment(p2, vec3(0.0,-50.0, 0.0), vec3(0.0, 50.0, 0.0) );
  vec2 h3 = sdSegment(p3, vec3(0.0,-50.0, 0.0), vec3(0.0, 50.0, 0.0) );

  return opU( opU( vec3(h1.x-0.12,                                   ph + 0.0/3.0, h1.y), 
                   vec3(h2.x-0.12-0.05*cos( 500.0*h2.y - iTime*4.0), ph + 1.0/3.0, h2.y) ), 
                   vec3(h3.x-0.12-0.02*cos(2000.0*h3.y - iTime*4.0), ph + 2.0/3.0, h3.y) );
}

vec3
calcNormal (vec3 pos, float dt)
{
  vec2 e = vec2(1.0,-1.0)*dt;
  return normalize( e.xyy*map( pos + e.xyy ).x + 
                    e.yyx*map( pos + e.yyx ).x + 
                    e.yxy*map( pos + e.yxy ).x + 
                    e.xxx*map( pos + e.xxx ).x );
}

float
calcOcc (vec3 pos, vec3 nor)
{
  const float h = 0.15;
  float ao = 0.0;
  for( int i=0; i<8; i++ )
  {
    vec3 dir = sin( float(i)*vec3(1.0,7.13,13.71)+vec3(0.0,2.0,4.0) );
    dir = dir + 2.5*nor*max(0.0,-dot(nor,dir));            
    float d = map( pos + h*dir ).x;
    ao += max(0.0,h-d);
  }
  return clamp( 1.0 - 0.7*ao, 0.0, 1.0 );
}

vec3
shade (float t, float m, float v, vec3 ro, vec3 rd)
{
  float px = 0.0001;
  float eps = px*t;

  vec3  pos = ro + t*rd;
  vec3  nor = calcNormal( pos, eps );
  float occ = calcOcc( pos, nor );

  vec3 col = 0.5 + 0.5*cos( m*vec3(1.4,1.2,1.0) + vec3(0.0,1.0,2.0) );
  col += 0.05*nor;
  col = clamp( col, 0.0, 1.0 );
  col *= 1.0 + 0.5*nor.x;
  col += 0.2*clamp(1.0+dot(rd,nor),0.0,1.0);
  col *= 1.4;
  col *= occ;
  col *= exp( -0.15*t );
  col *= 1.0 - smoothstep( 15.0, 35.0, t );

  return col;
}

void
main ()
{
  vec2 p = (-iResolution.xy+2.0*gl_FragCoord.xy)/iResolution.y;

  vec3 ro = 0.6*vec3(2.0,-3.0, 4.0);
  vec3 ta = 0.5*vec3(0.0, 4.0,-4.0);

  float fl = 1.0;
  vec3 ww = normalize( ta - ro);
  vec3 uu = normalize( cross( vec3(1.0,0.0,0.0), ww ) );
  vec3 vv = normalize( cross(ww,uu) );
  vec3 rd = normalize( p.x*uu + p.y*vv + fl*ww );

  float px = (2.0/iResolution.y)*(1.0/fl);

  vec3 col = vec3(0.0);

  const float maxdist = 32.0;

  vec3 res = vec3(-1.0);
  float t = 0.0;
#ifdef ANTIALIASING
  vec3 oh = vec3(0.0);
  vec4 tmp = vec4(0.0);
#endif

  for( int i=0; i<128; i++ )
  {
    vec3 h = map( ro + t*rd );
    float th1 = px*t;
#ifndef ANTIALIASING
    th1 *= 1.5;
#endif
    res = vec3( t, h.yz );
    if( h.x<th1 || t>maxdist )
      break;

#ifdef ANTIALIASING
    float th2 = px*t*2.0;
    if( (h.x<th2) && (h.x>oh.x) )
    {
      float lalp = 1.0 - (h.x-th1)/(th2-th1);
      vec3  lcol = shade( t, oh.y, oh.z, ro, rd );
      tmp.xyz += (1.0-tmp.w)*lalp*lcol;
      tmp.w   += (1.0-tmp.w)*lalp;
      if( tmp.w>0.99 )
        break;
    }
    oh = h;
#endif

    t += min( h.x, 0.5 )*0.5;
  }

  if( t < maxdist )
    col = shade( res.x, res.y, res.z, ro, rd );

#ifdef ANTIALIASING
  col = mix( col, tmp.xyz/(0.001+tmp.w), tmp.w );
#endif

  col = pow( col, vec3(0.5,0.7,0.5) );

  vec2 q = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.1);

  gl_FragColor = vec4( col, 1.0 );
}
