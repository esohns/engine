#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define NUMPASSES 2

vec3
sphNormal (vec3 pos, vec4 sph)
{
  return normalize(pos-sph.xyz);
}

float
sphIntersect (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - sph.w*sph.w;
  float h = b*b - c;
  if( h<0.0 )
    return -1.0;
  return -b - sqrt( h );
}

float
sphShadow (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - sph.w*sph.w;
  return step( min( -b, min( c, b*b - c ) ), 0.0 );
}

vec2
sphDistances (vec3 ro, vec3 rd, vec4 sph)
{
  vec3 oc = ro - sph.xyz;
  float b = dot( oc, rd );
  float c = dot( oc, oc ) - sph.w*sph.w;
  float h = b*b - c;
  float d = sqrt( max(0.0,sph.w*sph.w-h)) - sph.w;
  return vec2( d, -b-sqrt(max(h,0.0)) );
}

float
sphSoftShadow (vec3 ro, vec3 rd, vec4 sph)
{
  float s = 1.0;
  vec2 r = sphDistances( ro, rd, sph );
  if( r.y>0.0 )
    s = max(r.x,0.0)/r.y;
  return s;
}

float
sphOcclusion (vec3 pos, vec3 nor, vec4 sph)
{
  vec3  r = sph.xyz - pos;
  float l = length(r);
  float d = dot(nor,r);
  float res = d;

  if( d<sph.w )
    res = pow(clamp((d+sph.w)/(2.0*sph.w),0.0,1.0),1.5)*sph.w;

  return clamp( res*(sph.w*sph.w)/(l*l*l), 0.0, 1.0 );
}

#define NUMSPHERES 15

vec4 sphere[NUMSPHERES];

float
shadow (vec3 ro, vec3 rd)
{
  float res = 1.0;
  for( int i=0; i<NUMSPHERES; i++ )
    res = min( res, 8.0*sphSoftShadow(ro,rd,sphere[i]) );
  return res;
}

float
occlusion (vec3 pos, vec3 nor)
{
  float res = 1.0;
  for( int i=0; i<NUMSPHERES; i++ )
    res *= 1.0 - sphOcclusion( pos, nor, sphere[i] ); 
  return res;
}

vec3 hash3 (float n) { return fract(sin(vec3(n,n+1.0,n+2.0))*43758.5453123); }

vec3
textureBox (sampler2D sam, vec3 pos, vec3 nor)
{
  vec3 w = abs(nor);
  return (w.x*texture( sam, pos.yz ).xyz + 
          w.y*texture( sam, pos.zx ).xyz + 
          w.z*texture( sam, pos.xy ).xyz ) / (w.x+w.y+w.z);
}

vec3 lig = normalize( vec3( -0.8, 0.3, -0.5 ) );

vec3
shade (vec3 rd, vec3 pos, vec3 nor, float id, vec3 uvw, float dis)
{
  vec3 ref = reflect(rd,nor);
  float occ = occlusion( pos, nor );
  float fre = clamp(1.0+dot(rd,nor),0.0,1.0);

  occ = occ*0.5 + 0.5*occ*occ;

  vec3 lin = vec3(0.0);
  lin += 1.0*vec3(0.6,0.6,0.6)*occ;
  lin += 0.5*vec3(0.3,0.3,0.3)*(0.2+0.8*occ);
  lin += 0.3*vec3(0.5,0.4,0.3)*pow( fre, 2.0 )*occ;
  lin += 0.1*nor.y + 0.1*nor;

  float dif = clamp( nor.y, 0.0, 1.0 ) * shadow( pos, vec3(0.0,1.0,0.0) );

  lin = lin*0.7 + 0.8*dif;

  vec3 mate = 0.6 + 0.4*cos( 10.0*sin(id) + vec3(0.0,0.5,1.0) + 2. );
  vec3 te = textureBox( iChannel0, 0.25*uvw, nor );
  vec3 qe = te;
  te = 0.1 + 0.9*te;
  mate *= te*1.7;

  float h = id / float(NUMSPHERES);
  mate *= 1.0-smoothstep(0.5,0.6,sin(50.0*uvw.x*(1.0-0.95*h))*
                                  sin(50.0*uvw.y*(1.0-0.95*h))*
                                  sin(50.0*uvw.z*(1.0-0.95*h)) );

  vec3 col = mate*lin;

  float r = clamp(qe.x,0.0,1.0);
  col += 0.2 * r * pow( clamp(dot(-rd,nor), 0.0, 1.0 ), 4.0 ) * occ;
  col += 0.4 * r * pow( clamp(reflect(rd,nor).y, 0.0, 1.0 ), 8.0 ) * dif;

  return col*0.8;
}

vec3
trace (vec3 ro, vec3 rd, vec3 col, float px)
{
  float tmin = 1e20;
  float t = tmin;
  float id  = -1.0;
  vec4  obj = vec4(0.0);

  for( int i=0; i<NUMSPHERES; i++ )
  {
    vec4 sph = sphere[i];
    float h = sphIntersect( ro, rd, sph ); 
    if( h>0.0 && h<t ) 
    {
      t = h;
      obj = sph;
      id = float(i);
    }
  }

  if( id>-0.5 )
  {
    vec3 pos = ro + t*rd;
    vec3 nor = sphNormal( pos, obj );
    col = shade( rd, pos, nor, float(NUMSPHERES-1)-id, pos-obj.xyz, t );
  }

  return col;
}

vec3
animate (float time)
{
  vec3 cen = vec3(0.0);

  for( int i=0; i<NUMSPHERES; i++ )
  {
    float id  = float(NUMSPHERES-1-i);
    float ra = pow(id/float(NUMSPHERES-1),3.0);
    vec3  pos = 1.0*cos( 6.2831*hash3(id*16.0+2.0*0.0) + 1.5*(1.0-0.7*ra)*sin(id)*time*2.0 );
    ra = 0.2 + 0.8*ra;

#if NUMPASSES>0
#if NUMPASSES>1
    for( int k=min(0,iFrame); k<NUMPASSES; k++ )
#endif
    for( int j=min(0,iFrame); j<i; j++ )
    {
      vec3  di = pos.xyz - sphere[j].xyz;
      float rr = ra + sphere[j].w;
      float di2 = dot(di,di);
      if( di2 < rr*rr )
      {
        float l = sqrt(di2);
        pos += di*(rr-l)/l;
      }
    }
#endif

    sphere[i] = vec4( pos, ra );
    cen += pos;
  }

  cen /= float(NUMSPHERES);
  return cen;
}

void
main ()
{
  vec2 q = gl_FragCoord.xy / iResolution;
  vec2 p = (2.0*gl_FragCoord.xy-iResolution)/iResolution.y;
  vec2 m = step(0.0001,iMouse.z) * iMouse.xy/iResolution;

  float time = iTime*0.5;
  vec3 cen = animate( time );

  float an = 0.3*time - 7.0*m.x - 3.5;

  float le = 2.5;
  vec3 ro = cen + vec3(4.0*sin(an),1.0,4.0*cos(an));
  vec3 ta = cen;
  vec3 ww = normalize( ta - ro );
  vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
  vec3 vv = normalize( cross(uu,ww));
  vec3 rd = normalize( p.x*uu + p.y*vv + le*ww );

  float px = 1.0*(2.0/iResolution.y)*(1.0/le);

  vec3 col = vec3(0.2)*clamp(1.0-0.3*length(p),0.0,1.0);

  col = trace( ro, rd, col, px );

  col = pow( col, vec3(0.44,0.5,0.55) );

  col = mix( col, smoothstep( 0.0, 1.0, col ), 0.5 );

  col *= 0.2 + 0.8*pow(16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.2);

  col += (1.0/255.0)*hash3(q.x+13.0*q.y);

  gl_FragColor = vec4( col, 1.0 );
}
