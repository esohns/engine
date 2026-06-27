#version 130

uniform vec2 iResolution;
uniform float iTime;

// supersampling, make make it 2 for 4x
#define AA 1

vec2
cylIntersect (vec3 ro, vec3 rd, float rad)
{
  float a = dot (rd.xz, rd.xz);
  float b = dot (ro.xz, rd.xz);
  float c = dot (ro.xz, ro.xz) - rad*rad;
  float h = b*b - a*c;
  if (h < 0.0)
    return vec2(-1.0);
  h = sqrt(h);
  return vec2 (-b - h, -b + h) / a;
}

float
sdBox (vec3 p, vec3 b)
{
  vec3  q = abs(p) - b;
  float g = max(q.x,max(q.y,q.z));
  return g<0.0 ? g : length(max(q,0.0));
}

float
sdBox (vec2 p, vec2 b)
{
  vec2  q = abs(p) - b;
  float g = max(q.x,q.y);
  return g<0.0 ? g : length(max(q,0.0));
}

float
sdTube (vec3 p, float r, float h, float w)
{
  vec2 q = vec2( length(p.xz)-r, p.y );
  return sdBox( q, vec2(w,h));
}

vec2
smin (float a, float b, float k)
{
  k *= 4.0;
  float h = max( k-abs(a-b), 0.0 )/k;
  return vec2 (min(a,b)-h*h*k/4.0,
               a<b ? h*0.5 : 1.0-h*0.5);
}

float
hash (vec3 p)
{
  p += 1000.0;
  return fract(123.0*sin(p.x*21.6)*sin(p.y*43.4)*sin(p.z*14.5));
}

float
cnoise (vec3 p)
{
  vec3 ip = floor(p);
  vec3 fp = fract(p);

  fp = fp*fp*(3.0-2.0*fp);

  float a = hash(ip+vec3(0,0,0));
  float b = hash(ip+vec3(1,0,0));
  float c = hash(ip+vec3(0,1,0));
  float d = hash(ip+vec3(1,1,0));
  float e = hash(ip+vec3(0,0,1));
  float f = hash(ip+vec3(1,0,1));
  float g = hash(ip+vec3(0,1,1));
  float h = hash(ip+vec3(1,1,1));

  return mix(mix(mix(a,b,fp.x),
                 mix(c,d,fp.x), fp.y ),
             mix(mix(e,f,fp.x),
                 mix(g,h,fp.x), fp.y ), fp.z );
}

float
fbm (vec3 p)
{
  float f = 0.0;
  float a = 0.5;
  for( int i=0; i<6; i++ )
  {
    f += a*cnoise(p);
    p = p.yxz*1.99 + 0.1;
    a *= 0.56;
  }
  return f;
}

vec4
map (vec3 p)
{
  {
    float an = 6.2831*iTime/40.0;
    p.xz = mat2(cos(an), -sin(an), sin(an), cos(an))*p.xz;
  }

  vec3 op = p;

  float sp = 0.40;
  float layerID = clamp( round(p.y/sp), -10.0, 1.0 );
  p.y = p.y - sp*layerID;

  {
    float rb = 123.0*sin(layerID * 1.3);
    p.xz = mat2(cos(rb), -sin(rb), sin(rb), cos(rb))*p.xz;
  }

  float an = 6.283185/12.0;
  float a = atan(p.z,p.x);
  float sectorID = round(a/an);
  float ra = sectorID * an;
  p.xz = mat2(cos(ra), -sin(ra), sin(ra), cos(ra))*p.xz;

  float h1 = sin(ra*123.0 + layerID*924.0);
  float h2 = sin(ra*462.0 + layerID*214.9);
  float h3 = sin(ra*754.0 + layerID*534.2);
  float h4 = sin(ra*445.0 + layerID*736.6);

  p.x -= 1.5 + 0.05*abs(h1);
  float d1 = sdBox( p, vec3(0.05,0.12+0.05*h3,0.3+0.10*h2) ) - 0.1;

  float f = fbm( op/0.15 );
  d1 += 0.07*f;

  float objectID = 0.0;

  float d2 = sdTube( op-vec3(0.0,-1.0,0.0), 1.45, 1.5, 0.15 );
  d2 += 0.02*f;

  vec2 re = smin(d1,d2,0.01);

  return vec4( re.x, re.y, h4, 0.0 );
}

#define ZERO min(iFrame,0)
vec3
calcNormal (vec3 pos)
{
  vec3 n = vec3(0.0);
  for( int i=0; i<4; i++ )
  {
    vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
    n += e*map(pos+0.0025*e).x;
  }
  return normalize(n);
}

float
calcAO (vec3 pos, vec3 nor, float time)
{
  float occ = 0.0;
  float sca = 1.0;
  for( int i=0; i<8; i++ )
  {
    float h = 0.005 + 0.2*float(i)/8.0;
    vec3 dir = normalize( nor + 0.85*sin(h*31.31+vec3(0,2,4)));
    dir *= sign(dot(dir,nor));
    float d = map( pos+h*dir ).x;
    occ += max(h-d,0.0)*sca;
    sca *= 0.95;
    if( occ>1.0/1.5 )
      break;
  }
  return clamp( 1.0 - 1.5*occ, 0.0, 1.0 );
}

float
calcSoftshadow (vec3 ro, vec3 rd, float k)
{
  float res = 1.0;
  float tmax = cylIntersect( ro, rd, 1.75 ).y;
  float t    = 0.001;
  for( int i=0; i<128; i++ )
  {
    float h = map( ro + rd*t ).x;
    res = min( res, k*h/t );
    t += clamp( h*0.5, 0.02, 0.25 );
    if( res<0.01 || t>tmax )
      break;
  }
  return clamp( res, 0.0, 1.0 );
}

vec4
intersect (vec3 ro, vec3 rd)
{
  vec4 res = vec4(-1.0);
  vec2 bb = cylIntersect( ro, rd, 1.73 );
  if( bb.y>0.0 )
  {
    float t = max(bb.x,0.0);
    float tmax = bb.y;
    for( int i=0; i<256 && t<tmax; i++ )
    {
      vec4 h = map(ro+t*rd);
      if( h.x<0.002 ) { res=vec4(t,h.yzw); break; }
      t += h.x*0.35;
    }
  }
  return res;
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

void
main()
{
  vec3 tot = vec3(0.0);

#if AA>1
  for( int m=0; m<AA; m++ )
  for( int n=0; n<AA; n++ )
  {
    // pixel coordinates
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (2.0*(gl_FragCoord.xy+o)-iResolution.xy)/iResolution.y;
    float d = 0.5*sin(gl_FragCoord.x*147.0)*sin(gl_FragCoord.y*131.0);
    float time = iTime;
#else
    vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/iResolution.y;
    float time = iTime;
#endif

    // camera
    vec3 ta = vec3( 0.0, 0.0, 0.0 );
    vec3 ro = vec3( 4.0, 1.2, 0.0 );

    // camera-to-world transformation
    mat3 ca = setCamera( ro, ta, 0.0 );

    // ray direction
    float fl = 2.0;
    vec3 rd = ca * normalize( vec3(p,fl) );

    // background
    vec3 col = vec3(1.0+rd.y)*0.03;

    // raymarch geometry
    vec4 tuvw = intersect( ro, rd );
    if( tuvw.x>0.0 )
    {
      // shading/lighting
      vec3 pos = ro + tuvw.x*rd;
      vec3 nor = calcNormal(pos);

      // color
      vec3 brickColor = vec3(0.2,0.04,0.02);
      brickColor *= 1.0 + 0.4*tuvw.z;
      brickColor *= 1.0 + 0.2*sin( 3.1415927*tuvw.z+vec3(0,2,4));

      vec3 mortarColor = vec3(0.2,0.15,0.13);

      vec3 mate = mix( brickColor, mortarColor, tuvw.y );

      // key light
      vec3  lig = normalize(vec3(0.3,0.4,-0.9));
      float dif = max(0.0,dot(nor,lig));
      float sha = calcSoftshadow( pos+nor*0.001, lig, 32.0 );
      vec3  hal = normalize( lig-rd );
      float spe = pow( clamp( dot( nor, hal ), 0.0, 1.0 ), 8.0);
      spe *= dif*sha;
      spe *= 0.04+0.96*pow(clamp(1.0-dot(hal,lig),0.0,1.0),5.0);
      col = dif*sha*mate*vec3(4.0,2.0,1.0);
      col += spe*vec3(5.0);

      // done light
      float occ = calcAO( pos, nor, time ) * (1.0+0.4*nor.y);
      col += mate*occ*vec3(0.5,0.7,1.5)*1.3;

      //col = vec3(occ); // debug fake ambient occlusion
    }

    // gain
    col = col*2.5/(2.0+col);

      // gamma
    tot += pow(col,vec3(0.45) );
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  // cheap dithering
  tot += sin(gl_FragCoord.x*114.0)*sin(gl_FragCoord.y*211.1)/512.0;

  gl_FragColor = vec4( tot, 1.0 );
}
