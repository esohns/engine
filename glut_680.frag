uniform vec2 iResolution;
uniform float iTime;

#define AA 1

const int   kNum = 256;
const float kRad = 0.06;

vec3
knot (float t)
{
  t *= 6.283185;

  const float e = 0.16;
  const float h = 0.25;
  float a = e*sin(4.0*t);
  float b = 1.0-a*a;
  vec4 q = vec4 ( 
    b*(h*cos(t)+(1.0-h)*cos(3.0*t)),
    b*(2.0*sqrt(h-h*h)*sin(2.0*t)),
    a*(2.0),
    b*(h*sin(t)-(1.0-h)*sin(3.0*t))
    )
    / (1.0+a*a);

  float a1 = iTime*6.283185/10.0;
  q.xw *= mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
    
  vec3 p = q.xyz/(1.0-q.w);

  return p * 0.25;
}

vec4
iCylinder (vec3 ro, vec3 rd, vec3 pa, vec3 pb, float ra, out float v)
{
  vec4 res = vec4(-1.0);

  v = 0.0;
  vec3 ba = pb-pa;
  vec3 oc = ro-pa;

  float baba = dot(ba,ba);
  float bard = dot(ba,rd);
  float baoc = dot(ba,oc);
  float ocrd = dot(oc,rd);
  float ococ = dot(oc,oc);

  float a = baba - bard*bard;
  float b = baba*ocrd - baoc*bard;
  float c = baba*ococ - baoc*baoc - ra*ra*baba;
  float h = b*b - a*c;
  if( h>0.0 )
  {
    float t = (-b-sqrt(h))/a;

    float y = baoc + t*bard;
    if( y>0.0 && y<baba )
    {
      v = y/baba;
      res = vec4(t,(oc+t*rd-ba*v)/ra);
    }
    else 
    {
      h = ocrd*ocrd - ococ + ra*ra;
      if( h>0.0 )
      {
        t = -ocrd - sqrt(h);
        res = vec4(t,(oc+t*rd)/ra);
      }
    }
  }

  return res;
}

bool
sCylinder (vec3 ro, vec3 rd, vec3 pa, vec3 pb, float ra)
{
  vec3 ba = pb-pa;
  vec3 oc = ro-pa;

  float baba = dot(ba,ba);
  float bard = dot(ba,rd);
  float baoc = dot(ba,oc);
  float ocrd = dot(oc,rd);
  float ococ = dot(oc,oc);

  float a = baba - bard*bard;
  float b = baba*ocrd - baoc*bard;
  float c = baba*ococ - baoc*baoc - ra*ra*baba;
  float h = b*b - a*c;
  if( h>0.0 )
  {
    float t = (-b-sqrt(h))/a;

    float y = baoc + t*bard;
    if( t>0.0 && y>0.0 && y<baba )
      return true;
    h = ocrd*ocrd - ococ + ra*ra;
    if( h>0.0 )
    {
      t = -ocrd - sqrt(h);
      if( t>0.0 )
        return true;
    }
  }

  return false;
}

vec4
intersect (vec3 ro, vec3 rd, out float resV)
{
  float   v = 0.0;
  vec4 tnor = vec4(1e20);
  vec3   op = knot(0.0);
  for( int i=1; i<=kNum; i++ )
  {
    float t = float(i)/float(kNum);

    vec3 p = knot(t);

    float tmpv;
    vec4 tmp = iCylinder( ro, rd, op, p, kRad, tmpv );
    if( tmp.x>0.0 && tmp.x<tnor.x )
    {
      tnor = tmp;
      v=t+(tmpv-1.0)/float(kNum);
    }

    op = p;
  }

  resV = v;

  return tnor;
}

float
shadow (vec3 ro, vec3 rd)
{
  vec3 op = knot(0.0);
  for( int i=1; i<=kNum; i++ )
  {
    float t = float(i)/float(kNum);

    vec3 p = knot(t);

    if( sCylinder( ro, rd, op, p, kRad ) )
      return 0.0;

    op = p;
  }

  return 1.0;
}

vec3
shade (vec3 pos, vec3 nor, vec3 rd, float hm)
{
  vec3 mate = 0.5 + 0.5*cos(hm*6.283185+vec3(0.0,2.0,4.0));

  vec3 cen = knot(hm); 
  vec3 w = normalize(knot(hm+0.001)-cen);
  vec3 v = vec3(w.y,-w.x,0.0)/length(w.xy);
  vec3 u = normalize(cross(v,w));
  float an = atan( dot(pos-cen,u), dot(pos-cen,v) );
  float ar = an - 30.0*hm + iTime;
  mate += 1.5*smoothstep(-0.3,0.8,sin(6.283185*ar));

  vec3 ref = reflect(rd,nor);
  float dif = 0.5+0.5*nor.y;
  float spe = smoothstep(0.1,0.2,ref.y);
  spe *= dif;
  spe *= 0.04 + 0.96*pow( clamp(1.0+dot(rd,nor), 0.0, 1.0), 5.0 );
  if( spe>0.001 ) spe *= shadow(pos+nor*0.001, ref);
  vec3 col = 0.6*mate*vec3(1.0)*dif + spe*6.0;

  float fre = clamp(1.0+dot(rd,nor),0.0,1.0);
  col += fre*fre*(0.5+0.5*mate)*(0.2+0.8*dif);

  float occ = 0.0;
  for( int i=1; i<=kNum/4; i++ )
  {
    float h = float(i)/float(kNum/4);
    vec3  d = knot(h) - pos;
    float l2 = dot(d,d);
    float l = sqrt(l2);
    float f = dot(d/l,nor);
    occ = max(occ, f*exp2(-l2*8.0) );
    occ = max(occ, f*1.5*kRad*kRad/l2 );
  }
  col *= 1.0-occ;

  return col;
}

void
main ()
{
  float an = 0.0*iTime;
  vec3 ro = vec3( 1.0*sin(6.283185*an), 0.0, 1.0*cos(6.283185*an) );
  vec3 ta = vec3( 0.0, 0.02, 0.0 );

  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
  vec3 vv = normalize( cross(uu,ww));

  vec3 tot = vec3(0.0);

#if AA>1
  for( int m=0; m<AA; m++ )
  for( int n=0; n<AA; n++ )
  {
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (2.0*(gl_FragCoord.xy+o)-iResolution.xy)/iResolution.y;
#else    
    vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;
#endif

    vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );

    vec3 col = vec3(0.17*(1.0-0.15*dot(p,p))*smoothstep(-1.0,1.0,rd.y));

    float hm;
    vec4 tnor = intersect( ro, rd, hm );
    if( tnor.x<1e19 )
      col = shade( ro+tnor.x*rd, tnor.yzw, rd, hm );

    col *= 1.4/(1.0+col);
    col = pow( col, vec3(0.8,0.95,1.0) );

    col = pow( col, vec3(0.4545) );
    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif
 
  tot += (1.0/255.0)*fract(sin(gl_FragCoord.x*7.0+17.0*gl_FragCoord.y)*1.317);

  gl_FragColor = vec4( tot, 1.0 );
}
