#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define SAMPLE_COUNT 32
#define DIST_MAX 128.
#define MOUSEY (3.*iMouse.y/iResolution.y)
#define SAMPLES_ADAPTIVITY 0.2

bool useNewApproach = true;

vec3 lookDir = vec3(-1.,0.,0.);
vec3 camVel = vec3(-3.,0.,0.);
float zoom = 1.2; // 1.5;

float samplesCurvature = 0.;

vec3 sundir = normalize(vec3(-1.0,0.0,-1.));

float
noise (vec3 x)
{
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);
    
  vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
  vec2 rg = textureLod( iChannel0, (uv+ 0.5)/256.0, 0.0 ).yx;
  return mix( rg.x, rg.y, f.z );
}

vec4
map (vec3 p)
{
  float d = 0.1 + .8 * sin(0.6*p.z)*sin(0.5*p.x) - p.y;

  vec3 q = p;
  float f;

  f  = 0.5000*noise( q ); q = q*2.02;
  f += 0.2500*noise( q ); q = q*2.03;
  f += 0.1250*noise( q ); q = q*2.01;
  f += 0.0625*noise( q );
  d += 2.75 * f;

  d = clamp( d, 0.0, 1.0 );
    
  vec4 res = vec4( d );

  vec3 col = 1.15 * vec3(1.0,0.95,0.8);
  col += vec3(1.,0.,0.) * exp2(res.x*10.-10.);
  res.xyz = mix( col, vec3(0.7,0.7,0.7), res.x );

  return res;
}

float
spacing (float t)
{
  t = max(t,0.);

  float pdf = 1. / (SAMPLES_ADAPTIVITY*t + 1.);

  float norm = (1. / SAMPLES_ADAPTIVITY)*log(1. + SAMPLES_ADAPTIVITY*DIST_MAX);

  pdf /= norm;

  return 1. / (float(SAMPLE_COUNT) * pdf);
}

float
mov_mod (float x, float y)
{
  return mod(x + (useNewApproach ? dot(camVel*iTime,lookDir) : 0.), y) ;
}

bool
on_boundary (float x, float y)
{
  float numericalFixOffset = y*0.25;
  return mov_mod( x + numericalFixOffset, y ) < y*0.5;
}

void
firstT (out float t, out float dt, out float wt, out bool even)
{
  dt = exp2(floor(log2(spacing(0.))));
  t = dt - mov_mod(t,dt);
  even = on_boundary(t,2.*dt);
  wt = 1.;
}

void
nextT (inout float t, inout float dt, inout float wt, inout bool even)
{
  float s = spacing(t);
  if( s < dt ) { dt /= 2.; even = true; }
  else if( even && s > 2.*dt ) { dt *= 2.; wt = 1.; even = on_boundary(t,2.*dt); }

  if( even ) wt = clamp( 2. - s/dt,0.,1.);

  t += dt;
  even = !even;
}

float
sampleWt (float wt, bool even)
{
  return even ? (2.-wt) : wt;
}

vec4
raymarch (vec3 ro, vec3 rd)
{
  vec4 sum = vec4(0, 0, 0, 0);

  float t, dt, wt; bool even;
  firstT( t, dt, wt, even );

  for(int i=0; i<SAMPLE_COUNT; i++)
  {
    if( sum.a > 0.99 ) continue;

    vec3 pos = ro + t*rd;
    vec4 col = map( pos );

    float dif = clamp((col.w - map(pos+0.6*sundir).w)/0.6, 0.0, 1.0 );
    vec3 lin = vec3(0.51, 0.53, 0.63)*1.35 + 0.55*vec3(0.85, 0.57, 0.3)*dif;
    col.xyz *= lin;

    col.xyz *= col.xyz;

    col.a *= 0.35;
    col.rgb *= col.a;

    float fadeout = 1.-clamp((t/(DIST_MAX*.3)-.85)/.15,0.,1.);

    float thisDt = dt * sampleWt( wt, even);
    thisDt = sqrt(thisDt/5. )*5.;
    sum += thisDt * col * (1.0 - sum.a) * fadeout;

    nextT( t, dt, wt, even );
  }

  sum.xyz /= (0.001+sum.w);

  return clamp( sum, 0.0, 1.0 );
}

vec3
sky (vec3 rd)
{
  vec3 col = vec3(0.);

  float hort = 1. - clamp(abs(rd.y), 0., 1.);
  col += 0.5*vec3(.99,.5,.0)*exp2(hort*8.-8.);
  col += 0.1*vec3(.5,.9,1.)*exp2(hort*3.-3.);
  col += 0.55*vec3(.6,.6,.9);

  float sun = clamp( dot(sundir,rd), 0.0, 1.0 );
  col += .2*vec3(1.0,0.3,0.2)*pow( sun, 2.0 );
  col += .5*vec3(1.,.9,.9)*exp2(sun*650.-650.);
  col += .1*vec3(1.,1.,0.1)*exp2(sun*100.-100.);
  col += .3*vec3(1.,.7,0.)*exp2(sun*50.-50.);
  col += .5*vec3(1.,0.3,0.05)*exp2(sun*10.-10.); 

  float ax = atan(rd.y,length(rd.xz))/1.;
  float ay = atan(rd.z,rd.x)/2.;
  float st = texture( iChannel0, vec2(ax,ay) ).x;
  float st2 = texture( iChannel0, .25*vec2(ax,ay) ).x;
  st *= st2;
  st = smoothstep(0.65,.9,st);
  col = mix(col,col+1.8*st,clamp(1.-1.1*length(col),0.,1.));

  return col;
}

void
main ()
{
  if( iMouse.z > 0. )
    useNewApproach = false;

  vec2 q = gl_FragCoord.xy / iResolution;
  vec2 p = -1.0 + 2.0*q;
  p.x *= iResolution.x/ iResolution.y;
  vec2 mo = -1.0 + 2.0*iMouse.xy / iResolution;

  vec3 ro = vec3(0.,1.9,0.) + iTime*camVel;
  vec3 ta = ro + lookDir;
  vec3 ww = normalize( ta - ro);
  vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
  vec3 vv = normalize(cross(ww,uu));
  vec3 rd = normalize( p.x*uu + 1.2*p.y*vv + 1.5*ww );

  vec3 col = sky(rd);

  vec3 rd_layout = rd/mix(dot(rd,ww),1.0,samplesCurvature);
  vec4 clouds = raymarch( ro, rd_layout );

  col = mix( col, clouds.xyz, clouds.w );

  col = clamp(col, 0., 1.);
  col = smoothstep(0.,1.,col);
  col *= pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.12 );

  gl_FragColor = vec4( col, 1.0 );
}
