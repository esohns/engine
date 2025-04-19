#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define RENDER_SKY
#define RENDER_OCEAN
#define RENDER_SPRAY
#define RENDER_WATERBOMB

const float PI = 3.1415926;
const float PI2 = PI *2.0; 

vec3 sundir = normalize(vec3 ( -1.0,0.1,0.0));
vec3 haloclr1 = vec3(254.,201.,59.)/255.;
vec3 haloclr2 = vec3(253.,158.,45.)/255.;

vec2 rot (vec2 p, float a) { float s = sin(a); float c = cos(a); return mat2(c,s,-s,c) * p; }

float
noise (vec3 v)
{
  vec3 p = floor(v);
  vec3 f = fract(v);
  f = f*f*(3.-2.*f);
  vec2 uv = (p.xy+vec2(37.,17.)*p.z) + f.xy;
  vec2 rg = textureLod( iChannel0, (uv+.5)/256., 0.).yx;

  return mix(rg.x, rg.y, f.z);
}

vec3
random3f (vec3 p)
{
  return textureLod( iChannel0, (p.xy + vec2(3.0,1.0)*p.z+0.5)/256.0, 0.0 ).xyz;
}

vec3
voronoi (vec3 x)
{
  vec3 p = floor( x );
  vec3 f = fract( x );

  float id = 0.0;
  vec2 res = vec2( 100.0 );
  for( int k=-1; k<=1; k++ )
  for( int j=-1; j<=1; j++ )
  for( int i=-1; i<=1; i++ )
  {
    vec3 b = vec3( float(i), float(j), float(k) );
    vec3 r = vec3( b ) - f + random3f( p + b );
    float d = dot( r, r );

    if (d < res.x)
    {
      id = dot( p+b, vec3(1.0,57.0,113.0 ) );
      res = vec2( d, res.x );
    }
    else if (d < res.y)
      res.y = d;
  }

  return vec3( sqrt( res ), abs(id) );
}

float
grow (float x, float bias, float k)
{
  return 1.0- exp(-abs(x-bias)*k);
}

float
fbmCloud ( vec3 p ) 
{
  float a = 0.5;
  p *= 0.5;
  p.x += iTime*0.5;
  p.y += iTime*0.2;
  float f = 1.0;
  float v = 0.0;
  for ( int i = 0 ; i < 5 ; ++i)
  {
    v += a* noise ( p*f );
    a *= 0.5 ;
    f *= 2.;
  }
  v = max(0.0, v - 0.5);
  return v;
}

float
calcDomeRay (vec3 ro, vec3 rd, out vec3 domePos)
{
  float r = 500.0;
  float h = 30.0;
  vec3 o = ro;
  vec3 p = o;
  p.y -= r - h;
  float a = dot(rd,rd);
  vec3 op = 2.0*o-p;
  float b = dot(op, rd);
  float c = dot(op,op) - r*r;
  float bac = b*b-a*c;
  float t = -1.0;
  if ( bac > 0.0 )
    t = (-b +sqrt(bac)) / a;
  if ( t < 0.0 )
    t = -1.0;
  domePos = ro + rd*t;
  return t;
}

vec3
renderCloud ( vec3 ro, vec3 rd, vec3 bg)
{
  float sundot = clamp(dot(sundir, rd),0.,1.);
  vec3 halo1 = haloclr1 * pow ( sundot, 50.0);
  vec3 halo2 = haloclr2 * pow ( sundot, 20.0);

  vec4 sum = vec4(bg, 0.0);
  vec3 domePos;
  float domeT;
  if ( rd.y > -0.1 )
  {
    float domeT = calcDomeRay(ro,rd,domePos);
    if ( domeT > -0.5 ) 
    {
      float t = 0.0;
      for ( int i = 0 ;i < 4 ; ++i)
      {
        vec3 pos = domePos + rd * t ;
        float ratio = 0.2;
        float d1 = fbmCloud ( pos*ratio);;
        float d2 = fbmCloud ( (pos*ratio + sundir*1.0 ) ) ;

        float dif = clamp(d1-d2,0.0,1.0);

        vec4 clr = vec4(vec3(0.3),0.0) + vec4(haloclr2 *dif *5.0, d1);
        clr.rgb += halo2*5.0 + halo1 *2.0 ;
        clr.w *=  exp ( -distance(domePos, ro)*.025);
        clr.rgb = clr.rgb * clr.a;
        sum = sum + clr * ( 1.0-sum.a);

        t += 1.0;
      }
    }
  }

  return sum.rgb;
}

vec3
renderAtmosphere (vec3 rd)
{
  vec3 atm = vec3(0.0);
  float ry = max(0.0,rd.y);
  atm.r = mix(0.25, 0., grow(ry, 0.0, 5.0));
  atm.g = mix(0.06,0., grow(ry, 0.1, 0.5));
  atm.b = mix(0.,0.3, grow(ry, 0., 0.5));
  return atm;
}

vec3
renderSky( vec3 rd)
{
  float sundot = clamp(dot(sundir, rd),0.,1.);
  vec3 core = vec3(1.) * pow(sundot,250.0);

  vec3 halo1 = haloclr1 * pow ( sundot, 50.0);
  vec3 halo2 = haloclr2 * pow ( sundot, 20.0);
  vec3 sun = core + halo1 *0.5 + halo2 *0.9 ;

  vec3 atm = renderAtmosphere ( rd);
  return sun + atm;
}


vec3
renderSkyCloudDome ( vec3 ro, vec3 rd )
{
  vec3 sky = renderSky(rd);
  vec3 cloud = renderCloud( ro, rd, sky );
  return cloud;
}

mat2 octave_m = mat2(1.6,1.2,-1.2,1.6);

float
sea_octave (vec2 uv, float choppy)
{
  uv += noise(uv.xyx + uv.xyy);
  vec2 wv = 1.0-abs(sin(uv));
  vec2 swv = abs(cos(uv));    
  wv = mix(wv,swv,wv);
  return pow(1.0-pow(wv.x * wv.y,0.65),choppy);
}

float
sea_height (vec3 p)
{
  float freq = 0.4;
  float amp = 0.1;
  float choppy = 5.0;
  vec2 uv = p.xz; uv.x *= 0.75;
  float sea_time = iTime * 0.8;
  float d, h = 0.0;    
  for(int i = 0; i < 3; i++)
  {
    d = sea_octave((uv+sea_time)*freq,choppy);
    d += sea_octave((uv-sea_time)*freq,choppy);
    h += d * amp;
    uv *= octave_m; freq *= 1.9; amp *= 0.22;
    choppy = mix(choppy,1.0,0.2);
  }

  return h;
}

float
impulse (float x, float c, float w)
{
  float d = abs(x-c);
  if ( d > w )
    return 0.0;
  return 1.0 - smoothstep(0.0,1.0, d/w);
}

float
sdCylinder (vec3 p, vec2 h)
{
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float
hfWave (float x, vec3 kSpike)
{
  float cos_h = kSpike.x * ( cos ( PI * x  ) *0.5 +0.5 ) ;
  float spike_h = kSpike.y *pow(abs(1.0-abs(x)), 5.5* kSpike.z ) ;
  return cos_h + spike_h;
}

vec2
dfWave (vec3 q, vec2 o, float kRot, vec3 kSpike)
{
  vec2 p = q.xy;
  if ( dot(p,p) > 1.0) return vec2(p.y,1.0);
  float r = length(p); 
  p = rot ( p-o, kRot * 0.5 *PI *r  ) + o;

  return  vec2(p.y - (hfWave(p.x, kSpike) ),p.x);    
}

vec2
calcLip (vec2 o, float kRot, vec3 kSpike) 
{
  vec2 lip = vec2(0.0, hfWave(0.0, kSpike )); 
  float r = length(lip);
  return rot ( lip -o , -kRot * 0.5 *PI * r ) + o; 
}

void
bwaveParams ( float t, float life, out vec2 o, out vec3 kSpike, out float kRot )
{
  t *= 5.0 * life; 
  o = vec2(0.0);
  if ( t < 1.0 )
  {
    kSpike = vec3(0.2*t, 0.0,1.0);
    kRot = 0.0;
  }
  else if ( t < 2.0 )
  {
    t = t - 1.0;
    kSpike = vec3(0.2, 0.8*pow(t,1.5),1.0);
    kRot = pow(t,0.5);
  }
  else if ( t < 5.0 )
  {        
    t = t - 2.0; t /= 3.0;
    o = vec2(-.8,-1.) *t;
    float t2 = 1.0- t;
    kSpike = vec3(0.2*t2, 0.8*t2, t2) ;
    kRot = 0.9 + 0.1*(1.0-t);
    if ( t > 0.5 ) kRot = 0.95 * (1.0- ( t -0.5) *2.0); 
  }
}

vec2
calcLipAnim (vec2 ph)
{
  vec2 o; vec3 kSpike; float kRot;
  bwaveParams ( ph.x, ph.y, o, kSpike, kRot);
  return calcLip ( o, kRot, kSpike  );
}

vec3
dfWaveAnim (vec3 q, float sea_h, vec2 phase)
{
  q.y -= sea_h;
  vec2 o; vec3 kSpike; float kRot;
  float spikePhase = 1.0;

  bwaveParams ( phase.x, phase.y, o, kSpike, kRot);    
  vec2 d1 = dfWave(q, o, kRot*spikePhase, kSpike*spikePhase);
  vec2 d2 = dfWave(q, o, 0.0, vec3(kSpike.x*spikePhase,.0,.0));
  float d = mix( max ( d1.x,d2.x), min(d1.x,d2.x), smoothstep(-1.0,1.0, q.x ) ) ;    
  float ws = d1.y;

  q.y *= 2.5;
  float bound = sdCylinder(q.xzy,vec2(1.5,4.0));

  return vec3(d, ws, bound);
}

const float WaveTimeMax = 6.0;
const vec3 WaveOffset = vec3(2.8,0.0,0.0);
float
getWaveTime()
{
  return iTime;
}

const float d1 = 4.0;
const float d2 = 2.0;
const float d3 = 3.0;
const float totalPhaseTime = d1 + d2 + d3;

vec3
getPhaseDispInX (float t)
{
  float px;
  float dx;
  float d1 = 4.0;
  float d2 = 2.0;
  float d3 = 3.0;
  float t1 = d1;
  float t2 = t1 + d2;
  float t3 = t2 + d3;

  float tmod = mod(t, totalPhaseTime ) ;
  if ( tmod <= t1 )
  {
    px = tmod / d1 ;
    dx = tmod;
  }
  else if ( tmod <= t2)
  {
    px =  1.0 - (tmod - t1) / d2 ;
    dx = tmod;
  }
  else if ( tmod <= t3)
  {
    px = 0.0;
    dx = mix( t2, 0.0 , (tmod - t2) / d3 ); 
  }

  return vec3(px,dx*0.8, t3);
}

vec2
getPhase (vec3 p, float t)
{
  float w = 10.0;  // wave front length 
  vec3 params = getPhaseDispInX(t);

  float zt = mod(t, params.z) *1.5;

  float zDirPh = clamp(p.z + zt, 0.0, w) / w ; 

  float xDirPh = params.x;

  return vec2(zDirPh, xDirPh);
}

void
waveFrontDistort (inout vec3 p, float inSign)
{
  p.y *= 0.65;
    
  p.x -= inSign * (sin(2.0*p.z)*0.2 + cos(10.0*p.z)* 0.05);
    
  p += inSign * WaveOffset;
}

void
waveMoveForward (inout vec3 p, float t, float inSign)
{
  float shake = max(0.0,sin(t / WaveTimeMax * PI*0.5))*3.0;

  float xDisp = getPhaseDispInX(t).y;
  p.x -= inSign * xDisp ;  // moving for1ward
}

vec3
getLipPos (vec3 p, out vec2 phase)
{
  vec3 lip = vec3(0.0);
  float t = getWaveTime();
  waveFrontDistort(lip, -1.0);
  waveMoveForward(lip, t, -1.0f);

  phase = getPhase(p,t) ;

  lip.xy  += calcLipAnim ( phase ) ;

  return lip;
}

const float kSeaHeight = 1.0;
vec4
mapWave (vec3 p, out vec2 ph)
{
  float t = getWaveTime();

  waveFrontDistort(p, 1.0);

  ph = getPhase(p,t) ;

  float sea_h = sea_height ( p ) *  kSeaHeight;

  waveMoveForward( p, t, 1.0 );

  vec3 df = dfWaveAnim ( p,sea_h, ph);

  return vec4(df.x, df.y, df.z, ph); 
}

vec4
mapWave (vec3 p)
{
  vec2 ph;
  return mapWave(p, ph);
}

float
mapWaveCheap (vec3 p)
{
  float t = getWaveTime();
  waveFrontDistort(p, 1.0);
  vec2 ph = getPhase(p,t) ;
  float sea_h = 0.0;
  waveMoveForward( p, t, 1.0 );
  vec3 df = dfWaveAnim ( p,sea_h, ph);
  return df.z;
}

vec3
normalWave (vec3 pos)
{
  vec3 eps = vec3( 0.01, 0.0, 0.0 );
  vec3 nor = vec3(
    mapWave(pos+eps.xyy).x - mapWave(pos-eps.xyy).x,
    mapWave(pos+eps.yxy).x - mapWave(pos-eps.yxy).x,
    mapWave(pos+eps.yyx).x - mapWave(pos-eps.yyx).x );
  return normalize(nor);
}

float
traceWaveBound (vec3 ro, vec3 rd)
{
  float tmin = 1.0 ;
  float tmax = 50.0;

  float precis = 0.01;
  float t = tmin;
  float d = 0.0;

  for( int i=0; i<16; i++ )
  {
    d = mapWaveCheap( ro+rd*t );

    if( d <precis || t>tmax )
      break;
    t += d;
  }
  if( t>tmax )
    t=-1.0;

  return t;
}

vec4
traceWave (vec3 ro, vec3 rd)
{
  float tmin = 1.0 ;
  float tmax = 50.0;

  float precis = 0.01;
  float t = tmin;
  float d = 0.0;
  float ws = 0.0; // wave coordinates
  vec2 ph = vec2(0,0); // wave phase
  float bd = 0.0; // wave bounding cylinder df

  float tb = traceWaveBound(ro,rd);

  for( int i=0; i<128; i++ )
  {
    vec4 hit = mapWave( ro+rd*t, ph );
    d = hit.x; ws = hit.y; bd = hit.z;

    if( d <precis || t>tmax ) break;
#if 0
    t += (bd > 0.0 )? d *0.3 : d*0.1;
#else
    if ( tb > -0.5) 
      t += ( bd > 0.0 )? d *0.5 : d *0.1;
    else
      t += d;
#endif 
  }

  if( t>tmax )
    t=-1.0;
  return vec4( t, ws, ph.x, ph.y );
}

vec3
renderOcean (vec3 p, vec3 nml, vec3 lig, vec3 rd, float d, float ws, vec2 phase, vec3 sky)
{
  vec3 ligclr = haloclr1 *0.1;
  vec3 rfl = reflect ( rd, nml ) ;
  float dif = clamp( dot(nml,lig), 0.0, 1.0);
  float spe = pow(clamp( dot(rfl,lig), 0.0, 1.0), 16.0);
  float fre = pow(clamp(1.0+dot(nml,rd),0.0,1.0), 5.0 ) ;
  float fre2 = pow(clamp(1.0+dot(nml,rd),0.0,1.0), 3.0) *0.7;
  float ligdot = pow(clamp(dot(rfl,sundir),0.0,1.0),50.0);
  float t = phase.x;
    
  vec3 sss = sky * pow(1.0- clamp(abs(ws),0.0,1.0), 8.0 )  ;
  float k = pow(impulse(t, 0.8, 1.0),1.5);
  sss = sss * smoothstep(1.0,0.5, t) * k;
  sss *= phase.y;

  vec3 rflclr = (renderSky ( rfl ) *0.2) +  (ligdot * haloclr1 *2.0) ;
  vec3 rfrclr = (dif + spe + fre ) * ligclr * vec3(0.0,0.25,0.3) + sss;

  vec3 ocean = mix(rfrclr, rflclr, fre2);
  vec3 fog = sky;
  return mix( ocean, fog , 1.0-exp( -0.002*d*d ) );
}

float
sdBox (vec3 p, vec3 b)
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

float
fbmSpray (vec3 p)
{
  float ret = 0.0;
  float amp = 1.0;
  float frq = 1.0;
  p.y -= iTime*2.0;
  p.x -= iTime*1.0;
  ret +=  abs(amp * noise((p*frq ))) ; frq *= 2.7; amp *= 0.51;
  ret +=  abs(amp * noise((p*frq ))) ; frq *= 2.0; amp *= 0.49;    
  ret +=  abs(amp * noise((p*frq ))) ; frq *= 2.0; amp *= 0.49;    
  return ret;
}

float
sdSprayBound (vec3 p)
{
  float w = 0.3;
  float h = 1.0;

  vec3 bias = vec3(fbmSpray( p+vec3(128.0,17.0,13.0)),
                   fbmSpray(p),
                   fbmSpray(p+vec3(17.0,37.0,43.0)));    

  vec2 phase;
  vec3 lip = getLipPos (p,phase);
  float k = p.y-lip.y; 
  lip.y += 0.15;
  p -= lip;
  p.y -= h - 0.05 ;
  p += bias * smoothstep(0.0,2.0,k)*2.; 

  return sdBox ( p, vec3 ( w, h, 10.0));
}

float
sprayVol (vec3 p)
{
  if ( sdSprayBound(p) < 0.0 ) 
  {
    float d = smoothstep ( 1.0,0.0, abs(p.z));
    vec2 phase;

    vec3 lip = getLipPos (p, phase);
    float zPhase = phase.x;

    lip.y += 0.2;
    float lipk = smoothstep(0.1,1.0,lip.y);
    float k = exp( -abs(p.y - lip.y) *5.0);
    float v = mix(0.0,20.0,k) ;

    zPhase = clamp(zPhase,0.0,1.0) * phase.y;

    float s = smoothstep( 1.0,0.0, abs(zPhase - 0.38));

    s = pow (s , 40.0);

    float opa = v * s ;

    return opa;
  }

  return 0.0;
}

float
traceSprayVol (vec3 ro, vec3 rd, float depth)
{
  const int nSteps = 8;
  float dt = depth / float(nSteps);
  float t = 0.0;
  float opa = 0.0;
  float sum = 0.0;
  for ( int i = 0 ;i < nSteps ; ++i)
  {
    float d = sprayVol ( ro + rd *t ) ;
    sum += -t *d* dt;
    t += dt;
  }
  opa = 1.0 - exp(sum);
  return opa;
}

float
traceSpray (vec3 ro, vec3 rd)
{
  const float eps = 0.05;
  const float tmin = 0.0;
  const float tmax = 100.0;
  float t = tmin;
  float d = tmax;
  const int steps = 16;
  for ( int i = 0 ; i < steps ; ++i)
  {
    d = sdSprayBound(ro + t * rd);
    if ( d < eps || t > tmax)
      break;
    t += d * mix ( 0.5, 1.0, float(i)/float(steps));
  }
  if ( t < tmax)
    return t;
  return -1.0;
}

vec3
renderSpray (vec3 ro, vec3 rd, float spray_t, float ocean_t)
{
  vec3 clr = vec3(0.0);
  float t = spray_t;
  if ( t > -0.5)
  {
    vec3 fp = ro + rd *t ;
    vec3 ro2 = fp + rd* 5.0;
    float t2 = traceSpray ( ro2 , -rd ) ;

    t2 = t + 5.0 -t2;

    vec3 bp = ro + rd *t2;
    if ( ocean_t < t2 && ocean_t > -0.5) 
    {
      bp = ro + rd * ocean_t;
    }

    float depth = length( fp -bp);

    if ( t2 > -0.5)
    {
      float op = traceSprayVol ( ro+rd*t, rd, depth);
      clr = vec3(op);
    }
  }
  return clr;
}

float
fbm (vec3 p)
{
  float f;
  vec3 q = p;                           - vec3(0.0,1.0,0.0)*iTime;
    f  = 0.50000*noise( q ); q = q*2.02 - vec3(0.0,1.0,0.0)*iTime;
    f += 0.25000*noise( q ); q = q*2.03 - vec3(0.0,1.0,0.0)*iTime;
    f += 0.12500*noise( q ); q = q*2.01 - vec3(0.0,1.0,0.0)*iTime;
    f += 0.06250*noise( q ); q = q*2.02 - vec3(0.0,1.0,0.0)*iTime;
    f += 0.03125*noise( q );
    return f;
}

float
sdCapsule (vec3 p, vec3 a, vec3 b, float r)
{
  vec3 pa = p - a, ba = b - a;
  float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
  return length( pa - ba*h ) - r;
}

float
waterBombZBegin ()
{
  return 3.2 - mod(getWaveTime(), totalPhaseTime);
}

float
sdWaterBomb (vec3 p)
{
  vec2 phase;
  vec3 lip = getLipPos (p,phase); 
  lip.y = max(lip.y,0.0);
  p -= lip;
  float pz = waterBombZBegin(); // begin explosion

  float len = 8.0;
  float r =  0.8* sqrt(smoothstep(len+pz, pz, p.z));
  p.y -= 0.5;
  vec3 q = p;
  q.x += iTime*2.0;
  r -= voronoi ( q * 2.0 ).x *0.3;
  p.y += fbm(q) *0.5;

  return sdCapsule (p, vec3(0.0,0.0,pz), vec3(0.0,0.0,len + pz), r * phase.y * phase.y);
}

float
sdWaterBombCheap (vec3 p)
{
  vec2 phase;
  vec3 lip = getLipPos (p,phase); 
  p -= lip;

  float len = 8.0 * phase.y;
  float r = 1. *.8;
  p.y -= 0.5;
  vec3 q = p;
  q.x += iTime*2.0;
  p.y += fbm(q) *0.5;
  p.x += pow ( clamp(p.y,0.0,1.0),2.0)*1.0;

  float pz = waterBombZBegin() ;

  return sdCapsule (p, vec3(0.0,0.0,pz), vec3(0.0,0.0,len + pz), r );
}

float
waterBombVol (vec3 o)
{
  return smoothstep(1.5,0.0,o.y)*20.0;  
}

vec3
waterBombNormal (vec3 p)
{
  vec3 e = vec3( 0.001, 0.0, 0.0 );
  vec3 n = vec3(sdWaterBombCheap(p+e.xyy) - sdWaterBombCheap(p-e.xyy),
                sdWaterBombCheap(p+e.yxy) - sdWaterBombCheap(p-e.yxy), 
                sdWaterBombCheap(p+e.yyx) - sdWaterBombCheap(p-e.yyx) );
  return normalize(n);
}

vec4
traceWaterBombVol (vec3 ro, vec3 rd, float depth)
{
  const int nSteps = 4;
  float dt = depth / float(nSteps);
  float t = 0.0;
  vec3 clr = vec3(0.0);
  float sigma = 0.0;

  for ( int i = 0 ;i < nSteps ; ++i)
  {
    vec3 p = ro + rd * t ;
    float d = waterBombVol ( p ) ;
    sigma += -t * d * dt ;
    t += dt;
  }

  float opacity = 1.0 - exp(sigma);
  vec3 sunlgt = normalize(vec3(1.0));
  vec3 n = waterBombNormal ( ro ) ;
  float nl = clamp ( dot(n,sunlgt),0.0,1.0);
  opacity = mix ( opacity * nl , opacity, 0.8);

  float pre = fbm (100.0*(ro + vec3(0.0,-iTime*2.0,0.0)))*2.0;
  pre -= fbm ( 1.0*(ro + vec3(0.0,-iTime*1.0,0.0)));

  float splash =  mix( pre, 1.0, opacity);
  splash = clamp ( splash - 0.1, 0.0,1.0) *1.2;    
  opacity *= splash;

  return vec4(vec3(opacity), opacity);
}

float
traceWaterBomb (vec3 ro, vec3 rd)
{
  const float eps = 0.01;
  const float tmin = 0.0;
  const float tmax = 100.0;
  float t = tmin;
  float d = tmax;
  const int steps = 128;

  for ( int i = 0 ; i < steps ; ++i)
  {
    d = sdWaterBomb(ro + t * rd);
    if ( d < eps || t > tmax) break; 
    t += d * mix ( 0.5, 1.0, float(i)/float(steps));
  }
  if ( t < tmax) return t;
  return -1.0;
}

vec4
renderWaterBomb (vec3 ro, vec3 rd, float ocean_t)
{
  vec4 clr = vec4(0.0);
  float t = traceWaterBomb(ro,rd) ;
  if ( t > -0.5)
  {
    vec3 fp = ro + rd *t ;
    vec3 ro2 = fp + rd*10.0;
    float t2 = traceWaterBomb ( ro2 , -rd ) ;
    t2 = t + 10.0 - t2;
    vec3 bp = ro + rd *t2;

    if ( t2 > ocean_t && ocean_t > 0.0 )
      bp = ro + rd * ocean_t;
    float depth = length( fp -bp);

    if ( t2 > -0.5)
      clr = traceWaterBombVol ( ro+rd*t, rd, depth);
  }

  return clr;
}

vec3
render (vec3 ro, vec3 rd)
{
  vec3 clr = vec3(0.0);

#ifdef RENDER_SKY
  clr = renderSkyCloudDome(ro,rd);
#endif 

  float ot = -0.5;
  float ws = 0.0;  
  vec2 ph = vec2(0.0);
  float t = 0.0;

#ifdef RENDER_OCEAN    
  {
    vec4 hit = traceWave(ro,rd);
    ot = hit.x;
    ws = hit.y;
    ph = hit.zw;
    if( ot > -0.5 )
    {
      vec3 pos = ro + ot *rd;
      vec3 nor = normalWave( pos );
      clr = renderOcean (pos, nor, sundir, rd, ot, ws, ph, clr );   
    }
  }
#endif

#ifdef RENDER_SPRAY
  t = traceSpray( ro, rd) ;
  if ( t > -0.5 && (t < ot || ot <0.0))
  {
    vec3 tho = renderSpray ( ro, rd, t, ot);
    if ( ws < 0.0 || ws >= 1.0) 
      clr = mix(clr, vec3(0.5)+haloclr1, tho) ;
  }
#endif

#ifdef RENDER_WATERBOMB
  t = traceWaterBomb ( ro, rd ) ;
  if ( t > -0.5 && (t < ot || ot < 0.0) )
  {
    vec4 col4 = renderWaterBomb(ro,rd, ot);
    clr = mix ( clr, vec3(0.15)+haloclr1, min(col4.a,1.0));
  }
#endif

  return clr;
}

mat3
camera (vec3 pos, vec3 lookat, float roll)
{
  vec3 z = normalize(lookat- pos);
  vec3 x = vec3(sin(roll),cos(roll),0.0);  x = cross( z, x ) ;
  vec3 y = cross(x,z);
  return mat3(x, y, z ) ;
}

void
main ()
{
  vec2 q = gl_FragCoord.xy/iResolution.xy;
  vec2 p = -1.0+2.0*q;
  p.x *= iResolution.x/iResolution.y;
  vec2 mo = iMouse.xy/iResolution.xy;
  vec3 off = vec3(0.0,0.0,0.0);
  float my = 0.6;
  float mx = 0.95;
  if ( iMouse.z > 0.0 )
  {
    my = mix(0.35,0.75, mo.y);
    mx = mix(0.9, 1.0,  mo.x);
  }
  vec3 ro = off + vec3( -0.5+3.5*cos(PI2 *mx), 0.0 + 2.0*my, 0.5 + 3.5*sin(PI2 *mx) );
  vec3 ta = off + vec3( 0.0, 1.0,0.0 );
  mat3 ca = camera( ro, ta, 0.0 );
  vec3 rd = ca * normalize( vec3(p.xy,2.0) );
  vec3 col = render( ro, rd );
  col = pow( col, vec3(0.4545) );
  gl_FragColor=vec4( col, 1.0 );
}
