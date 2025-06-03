#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;

#define DOLLY_ZOOM

vec3 lightCol = vec3(1.1,1.,.9);
vec3 ambCol   = vec3(.2,.6,1.)*.25;
vec3 diffCol  = vec3(1.,.85,.7);
vec3 lightDir = normalize(vec3(.2,.5,1));

#define RandTex iChannel0
#define Res0 vec2(textureSize(iChannel0,0))
#define PI2 6.28318530718
#define GEAR_W .27
#define RaytraceMaxStepNum 100

float
gear (vec3 p, int numTeeth, float w)
{
  float lpxy=length(p.xy);
  float d=10000.;
  float ang=atan(p.y,p.x);
  d=min(d,length(p+vec3(p.xy/lpxy,0)*.1*sin(ang*float(numTeeth)))-1.);
  d=max(d,abs(p.z)-GEAR_W*w);
  d=max(d,.75-lpxy);
  return d;
}

vec4
inverseQuat (vec4 q)
{
  return vec4(-q.xyz,q.w);
}

vec4
multQuat (vec4 a, vec4 b)
{
  return vec4(cross(a.xyz,b.xyz) + a.xyz*b.w + b.xyz*a.w, a.w*b.w - dot(a.xyz,b.xyz));
}

vec3
transformVecByQuat (vec3 v, vec4 q)
{
  return v + 2.0 * cross( q.xyz, cross( q.xyz, v ) + q.w*v );
}

vec4
axAng2Quat (vec3 ax, float ang)
{
  return vec4(normalize(ax),1)*sin(vec2(ang*.5)+vec2(0,PI2*.25)).xxxy;
}

float
allGears (vec3 p, float rot, float tilt)
{
  float r0=.205;
  vec4 quat=vec4(0,0,0,1);
  float d=1000.;
  int N=2;
  int numTeeth=16;
  vec3 p0=p;
  for(int i=0;i<N+1;i++)
  {
    float ang=PI2/4.*float(i)/float(N);
    float c2=-cos(ang*4.)*.5+.5;
    vec3 pos0=vec3(cos(ang-vec2(0,PI2/4.)),0.)*.5;
    pos0-=normalize(pos0)*c2*.05*(.5+.5*cos(tilt)*cos(tilt));
    pos0+=vec3(0,0,1)*c2*.02*sin(tilt)*sin(tilt);
    int nteeth=numTeeth-3*(i%2);
    float r=r0*float(nteeth)/float(numTeeth);
    float gearAng=iTime*(float(i%2)*2.-1.)*r0/r-c2*.25;
    quat=axAng2Quat(vec3(0,0,1),rot*gearAng);
    quat=multQuat(quat,axAng2Quat(vec3(0,1,0),tilt));
    quat=multQuat(quat,axAng2Quat(vec3(0,0,1),-ang));
    p=fract(p0)-vec3(0,0,.5);
    d=min(d,gear(transformVecByQuat(p-pos0,quat)/r,nteeth,r/r0)*r);
    p=fract(p0.zxy*vec3( 1,-1, 1))-vec3(0,0,.5);
    d=min(d,gear(transformVecByQuat(p-pos0,quat)/r,nteeth,r/r0)*r);
    p=fract(p0.yzx*vec3(-1,-1, 1))-vec3(0,0,.5);
    d=min(d,gear(transformVecByQuat(p-pos0,quat)/r,nteeth,r/r0)*r);
  }
  return d;
}

vec4
myenv (vec3 pos, vec3 dir, float period)
{
#ifndef SHADEROO
  return texture(iChannel1,dir.xzy);
#else
  float azim = atan(dir.y,dir.x);
  float thr  = .5*.5*(.7*sin(2.*azim*5.)+.3*sin(2.*azim*7.));
  float thr2 = .5*.125*(.7*sin(2.*azim*13.)+.3*sin(2.*azim*27.));
  float thr3 = .5*.05*(.7*sin(2.*azim*32.)+.3*sin(2.*azim*47.));
  float br  = smoothstep(thr-.2, thr+.2, dir.z+.25);
  float br2 = smoothstep(thr2-.2,thr2+.2,dir.z+.15);
  float br3 = smoothstep(thr3-.2,thr3+.2,dir.z);
  vec4 r1 = .5*(texture(RandTex,dir.xy*.01)-texture(RandTex,dir.xy*.017+.33));
  vec3 skyCol=vec3(.9,1,1.1)+.3*(r1.xxx*.5+r1.xyz*.5);
  vec4 r2 = .5*(texture(RandTex,dir.xy*.1)-texture(RandTex,dir.xy*.07-.33));
  vec3 floorCol = vec3(.9,1.1,1.)*.8+.5*(r2.xxx*.7+r2.xyz*.3);
  vec3 col=mix(floorCol.zyx,skyCol,br3);
  col=mix(floorCol.yzx*.7,col,br2);
  col=mix(floorCol.xyz*.7*.7,col,br);
  vec3 r=texture(RandTex,vec2(azim/PI2*.125,.5)).xyz;
  col*= 1.-clamp(((r.xxx*.7+r.xzz*.3)*2.-1.)*clamp(1.-abs(dir.z*1.6),0.,1.),0.,1.);
  return vec4(col*col*vec3(1.1,1,.9)/**clamp(1.+dir.x*.3,.9,1.2)*/,1);
#endif
}

float
truchetDist (vec3 pos)
{
  vec3 mp=floor(mod(pos,2.))*2.-1.;
  float fact=mp.x*mp.y*mp.z;
  return allGears(pos*fact*mp.x,1.,PI2*.25*(mp.x*.5+.5));
}

float
getTime ()
{
  return texelFetch(iChannel0,ivec2(0,0),0).x;
}

float
getDistance (vec3 pos)
{
  return truchetDist(pos);
}

vec3
getGrad (vec3 pos, float delta)
{
  delta*=2.0;
  vec3 eps=vec3(delta,0,0);
  float d=getDistance(pos);
  return vec3(getDistance(pos+eps.xyy)-d, getDistance(pos+eps.yxy)-d, getDistance(pos+eps.yyx)-d)/delta;
}

float
march (inout vec3 pos, vec3 dir)
{
  float eps=0.002;
  for( int i=0 ; i<RaytraceMaxStepNum ; i++ )
  {
    float d=getDistance(pos);
    pos+=d*dir*.7;
    if(d<eps)
      return 1.;
  }
  return 0.;
}

void
calcDollyZoom (inout vec3 pos, vec3 dir0, inout vec3 dir, float fact)
{
  dir=mix(dir,dir0,fact);
  pos-=fact*dir0;
  pos+=fact*dir;
}

void
main ()
{
  float motion=1.;
  vec2 myRes=iResolution.xy;
  vec3 pos=vec3(iTime*.4*motion-.5,0,0);
  vec2 scoord=(gl_FragCoord.xy-myRes*.5)/myRes.x;
  float phi = iTime*.333*.3*motion;
  float th = cos(iTime*.23423*.3)*motion;
  if(iMouse.x>.5) 
  {
    phi+=-iMouse.x/iResolution.x*PI2/2.*3.;
    th+=PI2/4.-iMouse.y/iResolution.y*PI2/2.*3.;
  }

  vec4 quat=vec4(0,0,0,1);
  quat=multQuat(quat,axAng2Quat(vec3(0,0,1),phi));
  quat=multQuat(quat,axAng2Quat(vec3(1,0,0),th));
  vec3 dir0 = transformVecByQuat(vec3( 0,1,0),quat);
  vec3 left = transformVecByQuat(vec3(-1,0,0),quat);
  vec3 up   = transformVecByQuat(vec3( 0,0,1),quat);
  vec3 dir=left*scoord.x*2.+up*scoord.y*2.;
  dir=normalize(dir0+dir);
#ifdef DOLLY_ZOOM
  calcDollyZoom(pos,dir0,dir,.6*clamp(-cos(iTime*.1)*5.+.5,0.,1.));
#endif

  vec3 pos0=pos;
  march(pos,dir);

  vec3 bg=mix(ambCol,lightCol,clamp(dot(pos-pos0,lightDir)*.2,-.5,1.5));

  vec3 n=normalize(getGrad(pos,0.0015));
  vec3 R=normalize(reflect(pos-pos0,n));
  vec4 refl=myenv(pos,R,1.);

  vec3 col=vec3(1);

  float ao=1.;
  ao=min(ao,getDistance(pos+n*.08)/.08);
  ao=min(ao,getDistance(pos+n*.04)/.04);
  ao=min(ao,getDistance(pos+n*.02)/.02);
  ao=ao*.8+.2;

  float diff=clamp(dot(n,lightDir),0.,1.);

  col *= diffCol-.1+.1*(n*.5+.5);
  col *= ao;
  col *= lightCol*diff*(1.-ambCol)+ambCol;
  col+=.2*refl.xyz;

  col = mix(bg,col,exp(-length(pos-pos0)/7.));

  gl_FragColor = vec4(col, 1.);
}
