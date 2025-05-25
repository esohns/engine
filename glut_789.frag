uniform vec2 iResolution;
uniform float iTime;

#define HW_PERFORMANCE 1

float
sdHelix (vec3 p, float fr, float r1, float r2)
{
  vec2  nline = vec2(fr, 6.283185*r1 );
  vec2  pline = vec2(nline.y, -nline.x);
  float repeat = nline.x*nline.y;

  vec2  pc = vec2(p.x,r1*atan(p.y,p.z));

  vec2  pp = vec2( dot(pc,pline),
                   dot(pc,nline));

  pp.x = round(pp.x/repeat)*repeat;

  vec2 qc = (nline*pp.y+pline*pp.x)/dot(nline,nline);
  qc.y /= r1;

  vec3 q = vec3(qc.x, sin(qc.y)*r1, cos(qc.y)*r1 );

  return length(p-q)-r2;
}

float
map (vec3 pos)
{
  float fr = 1.0 - 0.5*cos(6.283185*iTime/6.0);
  float r1 = 0.4;
  float r2 = 0.1;

  return sdHelix(pos,fr,r1,r2) - 0.0001;
}

vec3
calcNormal (vec3 pos)
{
  vec2 e = vec2(1.0,-1.0)*0.5773;
  const float eps = 0.0005;
  return normalize( e.xyy*map( pos + e.xyy*eps ) + 
                    e.yyx*map( pos + e.yyx*eps ) + 
                    e.yxy*map( pos + e.yxy*eps ) + 
                    e.xxx*map( pos + e.xxx*eps ) );
}
   
#if HW_PERFORMANCE==0
#define AA 1
#else
#define AA 2
#endif

void
main ()
{
  float an = 1.0 + 0.0*iTime;
  vec3 ro = vec3( 1.5*cos(an), 0.4, 1.5*sin(an) );
  vec3 ta = vec3( 0.0, 0.0, 0.0 );

  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
  vec3 vv = normalize( cross(uu,ww));

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

    vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );

    const float tmax = 13.0;
    float t = 0.0;
    for( int i=0; i<256; i++ )
    {
      vec3 pos = ro + t*rd;
      float h = map(pos);
      if( abs(h)<0.0001 || t>tmax )
        break;
      t += h*0.75;
    }

    vec3 col = vec3(0.0);
    if( t<tmax )
    {
      vec3 pos = ro + t*rd;
      vec3 nor = calcNormal(pos);
      float dif = clamp( dot(nor,vec3(0.7,0.6,0.4)), 0.0, 1.0 );
      float amb = 0.5 + 0.5*dot(nor,vec3(0.0,0.8,0.6));
      col = vec3(0.2,0.3,0.4)*amb + vec3(0.8,0.7,0.5)*dif;
    }

    col = sqrt( col );
    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  gl_FragColor = vec4( tot, 1.0 );
}
