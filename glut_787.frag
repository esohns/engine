uniform vec2 iResolution;
uniform float iTime;

#define AA 2
#define M_PI 3.1415926535897932384626433832795

float density;
float height;
float fov;
float camera_y;
float gTime;
float vcut;
float lpscale;

float camera_ty = -0.17;
float interpos = -0.5;
float shorten = 1.;
float line_width = 0.017;
float rot_XY = 0.;
float rot_YZ = 0.785;
float radius = 0.05;
float rho_offset = 0.;

float
sdCone ( vec3 p, vec2 c )
{
  float q = length(p.xz);
  return dot(c,vec2(q,p.y));
}

void
pR (inout vec2 p, float a)
{
  p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

void
tile (vec3 p, out vec3 sp, out vec3 tp, out vec3 rp, out float mul)
{
  float r = length(p);
  p = vec3(log(r), acos(p.y / length(p)), atan(p.z, p.x));

  float xshrink = 1.0/(abs(p.y-M_PI)) + 1.0/(abs(p.y)) - 1.0/M_PI;
  p.y += height;
  p.z += p.x * 0.3;
  mul = r/lpscale/xshrink;
  p *= lpscale;
  sp = p;

  p.x -= rho_offset + gTime;
  
  p = fract(p*0.5) * 2.0 - 1.0;
  p.x *= xshrink;
  tp = p;
  pR(p.xy, rot_XY);
  pR(p.yz, rot_YZ);
  rp = p;
}

float
sdf (vec3 p)
{
  vec3 sp, tp, rp;
  float mul;
  tile(p, sp, tp, rp, mul);

  float spheres = abs(rp.x) - 0.012;
  float leaves = max(spheres, max(-rp.y, rp.z));
  leaves = max(leaves, vcut-sp.y);
  spheres = max(spheres, vcut-sp.y+1.07);
  float ret = min(leaves, spheres);

  vec3 pi = rp;
  pi.x += interpos;
  float interS = abs(pi.x) - 0.02;
  float interL = max(interS, max(-rp.y, rp.z));
  interL = max(interL, vcut-sp.y+2.);
  interS = max(interS, vcut-sp.y+3.);
  ret = min(ret, min(interL, interS));

  float ol = abs(rp.y) - radius*0.8;
  ol = min(ol, abs(rp.z) - radius*0.8);

  ret = max(ret, -ol);

  return ret * mul / shorten;
}

vec3
colr (vec3 p)
{
  vec3 sp, tp, rp, ret;
  float mul;
  tile(p, sp, tp, rp, mul);

  float ol = abs(rp.y) - radius;
  ol = min(ol, abs(rp.z) - radius);

  vec3 pi = rp;
  pi.x += interpos;
  float inter = abs(pi.x) - 0.02;
  inter = max(inter, vcut-sp.y+2.);

  float dark = smoothstep(density*0.25, density*0.5, density - sp.y);
  dark *= dark;
  
  if (ol < line_width)
    ret = vec3 (0.6, 0.6, 0.8)*dark;
  else if (inter < 0.02)
    ret = vec3 (0.1, 0.35, 0.05)*dark;
  else
    ret = vec3 (0.1, 0.15, 0.25)*dark;
  return ret;
}

vec3
calcNormal (vec3 pos)
{
  vec2 e = vec2(1.0,-1.0)*0.5773;
  const float eps = 0.0005;
  return normalize(
    e.xyy*sdf(pos + e.xyy*eps) + 
    e.yyx*sdf(pos + e.yyx*eps) + 
    e.yxy*sdf(pos + e.yxy*eps) + 
    e.xxx*sdf(pos + e.xxx*eps)
  );
}

float
gain (float x, float k) 
{
  float a = 0.5*pow(2.0*((x<0.5)?x:1.0-x), k);
  return (x<0.5)?a:1.0-a;
}

vec3
gain (vec3 v, float k)
{
  return vec3(gain(v.x, k), gain(v.y, k), gain(v.z, k));
}

float osc(float v1, float v2)
{
  return (sin(iTime*0.25)*0.5+0.5)*(v2-v1)+v1;
}

void
main ()
{
  vec2 iUV = gl_FragCoord.xy/iResolution.xy;

  gTime = iTime + osc(0., 4.);
  density = 26.;
  height = osc(0., 0.41);
  fov = osc(1.5, 1.07);
  camera_y = osc(0.4, 1.07);
  
  vcut = floor(density*0.25)*2.+0.9;
  lpscale = floor(density)/M_PI;

  float an = 0.002*gTime + 7.0;
  vec3 ro = vec3(1.0*cos(an), camera_y, 1.0*sin(an));
  vec3 ta = vec3( 0.0, camera_ty, 0.0 );

  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(ww,vec3(0.0,1.0,0.0)));
  vec3 vv = normalize(cross(uu,ww));

  vec3 bg = vec3(0.06, 0.08, 0.11)*0.3;
  bg *= 1.-smoothstep(0.1, 2., length(iUV*2.-1.));
  vec3 tot = bg;

#if AA>1
  for(int m=0; m<AA; m++)
  for(int n=0; n<AA; n++)
  {
    // pixel coordinates
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (-iResolution.xy + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else    
    vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
#endif

    vec3 rd = normalize(p.x*uu + p.y*vv + fov*ww);

    const float tmax = 3.5;
    float t = 0.0;
    vec3 pos;
    int iout;
    for( int i=0; i<64; i++ )
    {
      pos = ro + t*rd;
      float h = sdf(pos);
      if( h<0.0001 || t>tmax )
        break;
      t += h;
      iout = i;
    }
    float fSteps = float(iout) / 64.;
  
    vec3 col = vec3(0.0);
    if( t<tmax )
    {
      vec3 nor = calcNormal(pos);
      float dif = clamp( dot(nor,vec3(0.57703)), 0.0, 1.0 );
      float amb = 0.5 + 0.5*dot(nor,vec3(0.0,1.0,0.0));
      col = colr(pos)*amb + colr(pos)*dif;
    }

    float gloamt = smoothstep(0.04, 0.5, length(pos));
    float gain_pre = 1. - gloamt*0.6;
    float gain_k = 1.5 + gloamt*2.5;
    col += gain(fSteps*vec3(0.7, 0.8, 0.9)*gain_pre, gain_k);

    col = mix(col, bg, smoothstep(0.2+camera_y, 1.6+camera_y, t));

    col = sqrt( col );
    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  gl_FragColor = vec4(tot, 1.0);
}
