#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;

#define SHADOW
//#define FAKE_VOXEL_AO
//#define GRID_LINES

#define PI2 (3.141592653*2.)
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
  return vec4(normalize(ax),1) * sin(vec2(ang*.5)+vec2(0,PI2*.25)).xxxy;
}

vec3 light = normalize(vec3(1,.6,.3));

float
torusDist (vec3 pos, float R, float r)
{
  return length(vec2(length(pos.xy)-R,pos.z))-r;
}

#define randSampler iChannel0
vec4 getRand(vec3 texc)
{
  texc+=.6*sin(texc*256.*PI2)/PI2/256.;
  float z=texc.z*256.+.5;
  float fz=floor(z);
  vec4 v1 = textureLod( randSampler, texc.xy+(fz     *vec2(17.0,7.0))/256.0, 0.0);
  vec4 v2 = textureLod( randSampler, texc.xy+((fz+1.)*vec2(17.0,7.0))/256.0, 0.0);
  return mix( v1, v2, fract(z) );
}

float distGradX;

float
distTor (vec3 pos)
{
  vec4 q = vec4(0,0,0,1);
  q=multQuat(q,axAng2Quat(vec3(1,0,0),PI2*.125));
  q=multQuat(q,axAng2Quat(vec3(0,0,1),iTime*.5+2.));
  pos=transformVecByQuat(pos,q);
    
  pos+=.100*(getRand(pos*.015).xyz-.5);
  pos+=.050*(getRand(pos*.030).xyz-.5);
  pos+=.025*(getRand(pos*.060).xyz-.5);
  float d=torusDist(pos+vec3(.33,0,0),.66,.22+/*(pos.x+.33)**/distGradX);
  d=min(d,torusDist((pos-vec3(.33,0,0)).xzy,.66,.22+/*(pos.x-.33)**/-distGradX));
  return d;
}

float
dist (vec3 pos)
{
  return distTor(pos);
}

vec3
grad (vec3 pos, float eps)
{
  vec3 d=vec3(eps,0,0);
  return vec3(
      dist(pos+d.xyz)-dist(pos-d.xyz),
      dist(pos+d.zxy)-dist(pos-d.zxy),
      dist(pos+d.yzx)-dist(pos-d.yzx)
      )/eps/2.;
}

bool
checkSolid (vec3 pos)
{
  return dist(pos)-.00<.0;
}

bool
gridStep (inout vec3 pos, inout vec3 n, vec3 grid, vec3 dir)
{
  float l,lmin=10000.;
  vec3 s = sign(dir);
  vec3 next=floor(pos/grid+s*(.5+.00001)+.5)*grid;
  l=(next.x-pos.x)/dir.x; if (l>0. && l<lmin) { lmin=l; n=-vec3(1,0,0)*s; }
  l=(next.y-pos.y)/dir.y; if (l>0. && l<lmin) { lmin=l; n=-vec3(0,1,0)*s; }
  l=(next.z-pos.z)/dir.z; if (l>0. && l<lmin) { lmin=l; n=-vec3(0,0,1)*s; }
    
  pos+=dir*lmin;
  return checkSolid((floor((pos-.5*n*grid)/grid)+.5)*grid);
}

bool
march (inout vec3 pos, vec3 dir, inout float dmin)
{
  bool rval=false;
  float eps=.001;
  float dtot=0.;
  dmin=10000.;
  float dp=dist(pos);
  for(int i=0;i<100;i++)
  {
      float d=dist(pos);
      if(d<dp) dmin=min(d,dmin);
      dp=d;
      d*=.8;
      pos+=d*dir;
      dtot+=d;
      if(d<eps) { rval=true; break; }
      if(dtot>4.) { pos-=(dtot-4.)*dir; break; }
  }
  return rval;
}

bool
march (inout vec3 pos, vec3 dir)
{
  float dmin;
  return march(pos,dir,dmin);
}

#define GRID vec3(.05)
int
stepAny (inout vec3 pos, vec3 dir, inout vec3 n)
{
  distGradX=0.;
  vec3 startPos=pos;
  vec3 posV = pos;
  vec3 nV = vec3(0,0,1);
  bool bgV = true;
  for(int i=0;i<100;i++)
  {
      if(gridStep(posV,nV,GRID,dir)) { bgV=false; break; }
  }
  if (bgV) { n=light; posV=startPos+4.*dir; }
    
  distGradX=.1;
  vec3 posN = startPos;
  vec3 nN = vec3(0,0,1);
  bool bgN=!march(posN,dir);
  nN=grad(posN,.01);
  if (bgN) { nN=light; posN=startPos+4.*dir; }
    
  pos=posN; n=nN;
  if (bgV && bgN) return 0;
  if (length(posV-startPos)>length(posN-startPos)) return 1;
  pos=posV; n=nV; 
  return 2;
}

vec4
getRand (int idx)
{
  ivec2 res=textureSize(iChannel0,0);
  return texelFetch(iChannel0,ivec2(idx%res.x,(idx/res.x)%res.y),0);
}

vec4
getRand (vec2 pos)
{
  vec2 res=vec2(textureSize(iChannel0,0));
  return texture(iChannel0,pos/res);
}

#define iMouseData vec4(0)
void
main ()
{
  float ph=iMouse.x/iResolution.x*7.;
  float th=-(iMouse.y/iResolution.y-.5)*3.;
  if(iMouse.x<1.) th=-0.1;
  if(iMouse.y<1.) ph=2.3;
  vec3 fwd = vec3(vec2(cos(ph),sin(ph))*cos(th),sin(th));
  vec3 right=normalize(vec3(fwd.yx*vec2(1,-1),0));
  vec3 up=cross(right,fwd);
  vec3 startPos=-fwd*2.5*(1.-iMouseData.z*.001);
  vec3 pos=startPos;
  vec2 sc=(gl_FragCoord.xy/iResolution.xy-.5)*2.*vec2(1,iResolution.y/iResolution.x);
  vec3 dir=normalize(fwd*1.6+right*sc.x+up*sc.y);
  vec3 n=vec3(0,0,1);
  float ao=1.;
  float sh=1.;
  float br=1.;
  float ao1,sh1,ao2,sh2;
  vec3 posS,nS;

  int m = stepAny(pos,dir,n);
#ifdef SHADOW
  posS=pos+n*.005;
  int s = stepAny(posS,light,nS);
  sh = (s==0)?1.:0.;
#endif

#ifdef GRID_LINES
  if(m==2){
      vec3 s=sin(pos/GRID*PI2*.5);
      br*=1.-.15*(dot(exp(-s*s/.05),vec3(1))-1.);
      br*=1.-.075*(dot(exp(-s*s/.5),vec3(1))-1.);
  }
#endif

  ao=1.;
#ifndef FAKE_VOXEL_AO
  ao=0.;
#define AONUM 24
  for(int i=0;i<AONUM;i++)
  {
      vec3 r=2.*(getRand(i+int(getRand(gl_FragCoord.xy).x*147.)+int(123.*pos.x+37.*pos.y+17.*pos.y)).xyz-.5);
      r-=dot(r,n)*n*.5;
      vec3 posC=pos+n*.02+r*.04;
      distGradX=0.;
      float solid=0.;
      if(dist((floor(posC/GRID)+.5)*GRID)<0.) solid=1.;
      distGradX=.1;
      if(dist(posC)<0.) solid=1.;
      ao+=solid;
  }
  ao=clamp(1.-ao/float(AONUM),0.,1.);
#else
  if(m==2) { distGradX=.0; ao*=clamp(.8+.25*(dist(pos)+.03)/GRID.x,0.,1.); }
  if(m==1)
  {
    ao-=.5*max(.15-dist(pos+n*.1),0.);
    ao-=.5*max(.30-dist(pos+n*.2),0.);
    ao-=.5*max(.60-dist(pos+n*.4),0.);
  }
#endif
  ao*=1.-5.*max(.1-dist(pos+n*.2),0.);
  ao*=1.-8.*max(.05-dist(pos+n*.1),0.);

  float amb=.25;
  float fog=clamp(1.5-.5*length(pos),0.,1.);
  float diff=clamp(dot(normalize(n),normalize(light)),0.,1.)*.6;
  gl_FragColor.xyz=vec3(mix(diff*sh,ao,amb))*br;
  gl_FragColor.w=1.;
}
