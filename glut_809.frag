uniform vec2 iResolution;
uniform float iTime;

#define AA 2
#define HEIGHT 0.25
#define M_PI 3.1415926535897932384626433832795
#define LONGSTEP (M_PI*4.)

float gTime;

float gABPos;
float gDensA;
float gDensB;

float
sdCube (vec3 p, float b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

void
pR (inout vec2 p, float a)
{
  p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

float
sdSpike2D (vec2 p, float h)
{
  float d = p.y - (h*0.1)/(abs(p.x)+0.1);
  d = min(d, length(p - vec2(0, min(h, p.y))));
  float d2 = abs(p.x) - ((h*0.1)-0.1*p.y)/p.y;
  if (p.y<h && d>0.0)
    d = min(d, d2);
  return d;
}

vec3
tile (vec4 pin, out float density, out float cubsz)
{
  float r = pin.w;

  density = mix(gDensA, gDensB, smoothstep(0., 0.1, r-gABPos));

  vec3 p = vec3(log(r), (pin.y-HEIGHT*0.1/(r+0.1))/r, atan(pin.z, pin.x));

  p *= density;

  p.x -= gTime*2.0;

  pR(p.xz, 0.6435); // atan(3/4)

  p.xz = mod(p.xz, 2.0) - 1.0;

  float osc = sin(sqrt(r)-gTime*0.25-1.0);
  float cubrot = smoothstep(0.5, 0.8, osc);
  cubsz = sin(p.x*0.1)*0.29 + 0.5;
  cubsz = mix(cubsz, 0.96, smoothstep(0.7, 1.0, abs(osc)));
  pR(p.xy, cubrot);

  return p;
}

float
sdf (vec3 pin)
{
  float r = length(pin.xz);
  float cubsz, density;
  vec3 tiled = tile(vec4(pin, r), density, cubsz);
  float ret = sdCube(tiled, cubsz);
  ret *= r/density;

  float pkofs = r * cubsz / density;
  float pk = sdSpike2D(vec2(r, pin.y), HEIGHT) - pkofs;
  if (pk < 0.002) pk = ret;
  ret = min(ret, pk);

  float shorten = length(pin - vec3(0., 0.25, 0.));
  shorten = 1. + 1.5*(1.-smoothstep(0., 0.22, shorten));
  ret /= shorten;

  return ret;
}

vec3
colr (vec3 pin)
{
  float a = 0.26;
  float b = 0.65;
  float z = 0.19;
  float cubsz, density; // out
  vec3 p = tile(vec4(pin, length(pin.xz)), density, cubsz);
  if (p.x > abs(p.y) && p.x > abs(p.z)) return vec3(z,a,b);
  if (p.x < -abs(p.y) && p.x < -abs(p.z)) return vec3(z,b,a)*0.7;
  if (p.z > abs(p.x) && p.z > abs(p.y)) return vec3(z,a,a);
  if (p.z < -abs(p.x) && p.z < -abs(p.y)) return vec3(b*0.5,z,a);
  return vec3(b,b,a);
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
time2density (float x)
{
  float fullMod = fract(x/(LONGSTEP*3.))*3.;
  if (fullMod > 2.) return 45.;
  else if (fullMod > 1.) return 25.;
  else return 15.;
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy/iResolution.xy;

  gTime = iTime+1.8;
  float ltime = gTime + M_PI*6.3;
  gABPos = smoothstep(0.45, 0.6, fract(ltime/LONGSTEP))*2.2-0.2;
  gDensA = floor(time2density(ltime))/M_PI;
  gDensB = floor(time2density(ltime-LONGSTEP))/M_PI;

  float camera_y = pow(sin(gTime*0.2), 3.)*0.2+0.7;
  vec3 ro = vec3(0., camera_y, 1.);
  vec3 ta = vec3(0.0, 0.0, 0.0);

  vec3 ww = normalize(ta - ro);
  vec3 uu = normalize(cross(ww,vec3(0.0,1.0,0.0)));
  vec3 vv = normalize(cross(uu,ww));

  vec3 tot = vec3(0.0);
  
#if AA>1
  for(int m=0; m<AA; m++)
  for(int n=0; n<AA; n++)
  {
    vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
    vec2 p = (-iResolution.xy + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else    
    vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
#endif

    vec3 rd = normalize(p.x*uu + p.y*vv + 3.5*ww);

    const float tmax = 3.0;
    float t = 0.0;
    for(int i=0; i<256; i++)
    {
      vec3 pos = ro + t*rd;
      float h = sdf(pos);
      if( h<0.0001 || t>tmax )
        break;
      t += h;
    }
  
    vec3 bg = vec3(0.1, 0.15, 0.2)*0.3;
    vec3 col = bg;
    if(t<tmax)
    {
      vec3 pos = ro + t*rd;
      vec3 nor = calcNormal(pos);
      float dif = clamp( dot(nor,vec3(0.57703)), 0.0, 1.0 );
      float amb = 0.5 + 0.5*dot(nor,vec3(0.0,1.0,0.0));
      col = colr(pos)*amb + colr(pos)*dif;
    }

    col = mix(col, bg, smoothstep(2., 3., t));

    col = sqrt(col);
    tot += col;
#if AA>1
  }
  tot /= float(AA*AA);
#endif

  gl_FragColor = vec4(tot,1.0);
}
