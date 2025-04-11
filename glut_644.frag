#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;

#define AA 1

int   seed = 1;
int   rand(void) { seed = seed*0x343fd+0x269ec3; return (seed>>16)&32767; }
float frand(void) { return float(rand())/32767.0; }
void
srand (ivec2 p, int frame)
{
  int n = 1376312589;
  n += frame; n = (n<<13)^n; n=n*(n*n*15731+789221); 
  n += p.y;   n = (n<<13)^n; n=n*(n*n*15731+789221);
  n += p.x;   n = (n<<13)^n; n=n*(n*n*15731+789221);
  seed = n;
}

vec4
quadIntersect (vec3 ro, vec3 rd, vec3 v0, vec3 v1, vec3 v2, vec3 v3, float tmin, float tmax, out float oDisSq)
{
  vec3 r1 = v1 - v0;
  vec3 r2 = v2 - v0;
  vec3 r3 = v3 - v0;
  vec3 rz = ro - v0;

  vec3 nor = cross(r1,r2);
  float t = -dot(rz,nor)/dot(rd,nor);

  if( t<tmin || t>tmax )
    return vec4(-1.0);

  vec3 rp = rz + t*rd;

  vec3 ww = normalize(nor);
  float l1 = length(r1);
  vec3 uu = r1/l1;
  vec3 vv = cross(uu,ww);

  vec2 k0 = vec2( 0.0, 0.0 );
  vec2 k1 = vec2( l1,  0.0 );
  vec2 k2 = vec2( dot(r2,uu), dot(r2,vv) );
  vec2 k3 = vec2( dot(r3,uu), dot(r3,vv) );
  vec2 kp = vec2( dot(rp,uu), dot(rp,vv) );

  vec2  e0 = k1 - k0, p0 = kp - k0; 
  vec2  e1 = k2 - k1, p1 = kp - k1;
  vec2  e2 = k3 - k2, p2 = kp - k2;
  vec2  e3 = k0 - k3, p3 = kp - k3;

  float c0 = e0.x*p0.y - e0.y*p0.x;
  float c1 = e1.x*p1.y - e1.y*p1.x;
  float c2 = e2.x*p2.y - e2.y*p2.x;
  float c3 = e3.x*p3.y - e3.y*p3.x;

  if( max(max(c0,c1),max(c2,c3))>0.0 )
    return vec4(-1.0);

  float d = min(min(c0*c0/dot(e0,e0),
                    c1*c1/dot(e1,e1)),
                min(c2*c2/dot(e2,e2),
                    c3*c3/dot(e3,e3)));

  if( d>0.3*0.3 )
    return vec4(-1.0);

  oDisSq = d; 
  return vec4(t,ww);
}

#define DF(f) (ivec4((f)>>12,(f)>>8,(f)>>4,(f))&15);
#define EF(a,b,c,d) (((a)<<12)|((b)<<8)|((c)<<4)|(d))
#define QF(x,y,a) EF(a,a^(1<<x),a^(1<<x)^(1<<y),a^(1<<y))
const int kFaces[24] = int[24](
    // xy
    QF(0,1,  0),  //  0, 1, 3, 2
    QF(0,1,  7),  //  7, 6, 4, 5
    QF(0,1, 11),  // 11,10, 8, 9
    QF(0,1, 12),  // 12,13,15,14
    // xz
    QF(0,2,  0),  //  0, 1, 5, 4
    QF(0,2,  7),  //  7, 6, 2, 3
    QF(0,2, 11),  // 11,15,14,10
    QF(0,2, 12),  // 12, 8, 9,13
    // xw
    QF(0,3,  0),  //  0, 1, 9, 8
    QF(0,3,  7),  //  7,15,14, 6
    QF(0,3, 11),  // 11,10, 2, 3
    QF(0,3, 12),  // 12, 4, 5,13
    // yz
    QF(1,2,  0),  //  0, 2, 6, 4
    QF(1,2,  7),  //  7, 5, 1, 3
    QF(1,2, 11),  //  11,15,13,9
    QF(1,2, 12),  //  12,8,10,14
    // yw
    QF(1,3,  0),  //  0, 2,10, 8
    QF(1,3,  7),  //  7,15,13, 5
    QF(1,3, 11),  // 11, 9, 1, 3
    QF(1,3, 12),  // 12, 4, 6,14
    // zw
    QF(2,3,  0),  //  0, 4,12, 8
    QF(2,3,  7),  //  7,15,11, 3
    QF(2,3,  9),  //  9, 1, 5,13
    QF(2,3, 14)); //  14,10,2, 6

vec4
intersectClosest (vec3 ro, vec3 rd, vec3 verts[16], out int oFace, out float oDisSq)
{
  vec4 res = vec4(1e10,0.0,0.0,0.0);
  oFace = -1;

  for( int i=0; i<kFaces.length(); i++ )
  {
    float tmpd;
    ivec4 idx = DF(kFaces[i]);
    vec4 tmp = quadIntersect( ro, rd, verts[idx.x], verts[idx.y], verts[idx.z], verts[idx.w], 0.0, res.x, tmpd );
    if( tmp.x>0.0 )
    {
      res = tmp;
      oFace = i;
      oDisSq = tmpd;
    }
  }

  res.yzw = (dot(res.yzw,rd)<0.0) ? res.yzw : -res.yzw;  // face camera

  return (res.x<1e9)?res:vec4(-1.0);
}

float
intersectAny (vec3 ro, vec3 rd, vec3 v[16], int obj)
{
  for( int i=0; i<kFaces.length(); i++ )
  {
    if( i!=obj )
    {
      float kk;
      ivec4 idx = DF(kFaces[i]);
      if( quadIntersect( ro, rd, v[idx.x], v[idx.y], v[idx.z], v[idx.w], 0.001, 10.0, kk ).x>0.0 )
        return 0.0;
    }
  }
  return 1.0;
}

float
calcOcclusion (vec3 pos, vec3 nor, vec3 verts[16], int obj)
{
  float occ = 0.0;
  const int num = 16;
  for( int j=0; j<num; j++ )
  {
    float u = frand();
    float v = frand();
    float a = 6.2831853*v; float b = 2.0*u-1.0;
    vec3 dir = vec3(sqrt(1.0-b*b)*vec2(cos(a),sin(a)),b);

    dir = normalize( nor + dir );

    if( dir.y>0.0 )
      occ += intersectAny( pos, dir, verts, obj );
  }

  return occ/float(num);
}

vec3
render (vec3 ro, vec3 rd, vec3 verts[16], vec2 px)
{
  vec3 col = mix(vec3(0.05,0.4,0.4),vec3(0.05,0.2,0.4),0.5+0.5*px.y)*0.45;
  col *= 1.0-0.4*length(px);

  if( abs(px.x)<1.0)
  {
    int   face;
    float disSq;
    vec4  tnor = intersectClosest(ro,rd,verts,face,disSq);
    float t = tnor.x;
    if( t>0.0 )
    {
      vec3 pos = ro + t*rd;
      vec3 nor = tnor.yzw;

      ivec4 idx = DF(kFaces[face]);
      float l = length( verts[idx.x]-verts[idx.y] );
      l = max(l,length( verts[idx.y]-verts[idx.z] ));
      l = max(l,length( verts[idx.z]-verts[idx.w] ));
      l = max(l,length( verts[idx.w]-verts[idx.x] ));
      l += nor.x*1.5; 
      vec3 mate = vec3(0.6,0.4,0.52) + 0.5*sin(0.26*l+vec3(0,1.5,2)+1.35);
      mate = max(mate,0.0);
      mate.z += 0.2*(1.0-exp2(-0.02*t*t));
      mate *= 1.2-1.2*vec3(0.5,0.5,0.1)*smoothstep(0.6,0.7,-cos(sqrt(disSq)*80.0));

      col = mate * calcOcclusion( pos, nor, verts, face );
    }
  }

  return col;
}

mat3
setCamera (vec3 ro, vec3 ta, float cr)
{
  vec3 cw = normalize(ta-ro);
  vec3 cp = vec3(sin(cr), cos(cr),0.0);
  vec3 cu = normalize( cross(cw,cp) );
  vec3 cv =          ( cross(cu,cw) );
  return mat3( cu, cv, cw );
}

mat2
rot (float a)
{
  float c = cos(a);
  float s = sin(a);
  return mat2(c,-s,s,c);
}

vec3
transform (vec4 p, float time)
{
  p.xy = rot(6.283185*time/18.0)*p.xy;
  p.zw = rot(6.283185*time/ 6.0)*p.zw;
  return 2.8*p.xyz/(3.0+p.w); 
}

void
main ()
{
  srand( ivec2(gl_FragCoord.xy), iFrame);

  vec3 ro = vec3( 3.8, 2.0, 3.3 );
  vec3 ta = vec3( 0.0, 0.0, 0.0 );
  mat3 ca = setCamera( ro, ta, 0.0 );

  vec3 tot = vec3(0.0);
#if AA>1
#define ZERO min(iFrame,0)
  for( int m=ZERO; m<AA; m++ )
  for( int n=ZERO; n<AA; n++ )
  {
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (2.0*(gl_FragCoord.xy+o)-iResolution.xy)/iResolution.y;
    float time = iTime + frand()/30.0;
#else
    vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;
    float time = iTime;
#endif

    vec3 v[] = vec3[]( transform(vec4(-1,-1,-1,-1),time),
                        transform(vec4(-1,-1,-1, 1),time),
                        transform(vec4(-1,-1, 1,-1),time),
                        transform(vec4(-1,-1, 1, 1),time),
                        transform(vec4(-1, 1,-1,-1),time),
                        transform(vec4(-1, 1,-1, 1),time),
                        transform(vec4(-1, 1, 1,-1),time),
                        transform(vec4(-1, 1, 1, 1),time),
                        transform(vec4( 1,-1,-1,-1),time),
                        transform(vec4( 1,-1,-1, 1),time),
                        transform(vec4( 1,-1, 1,-1),time),
                        transform(vec4( 1,-1, 1, 1),time),
                        transform(vec4( 1, 1,-1,-1),time),
                        transform(vec4( 1, 1,-1, 1),time),
                        transform(vec4( 1, 1, 1,-1),time),
                        transform(vec4( 1, 1, 1, 1),time));

    vec3 rd = ca * normalize( vec3(p.xy,2.0) );

    vec3 col = render( ro, rd, v, p );

    col = pow( col, vec3(0.4545) );

    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  tot += frand()/255.0;

  gl_FragColor = vec4( tot, 1.0 );
}
