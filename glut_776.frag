#version 140

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0;
uniform samplerCube iChannel1;
uniform sampler2D iChannel2;

#define G (.5+sqrt(5./4.))
#define PI2 6.28318531

#define XCHGV3(a,b) { vec3 d=a; a=b; b=d; }

void
sortXYZ (inout vec3 p1, inout vec3 p2, inout vec3 p3)
{
#define W(p) (p.x+p.y*.01+p.z*.0001)
  if(W(p3)>W(p2)) XCHGV3(p3,p2);
  if(W(p2)>W(p1)) XCHGV3(p2,p1);
  if(W(p3)>W(p2)) XCHGV3(p3,p2);
  if(W(p2)>W(p1)) XCHGV3(p2,p1);
}

void
getIcosaTriOld (vec3 pos, out vec3 p1, out vec3 p2, out vec3 p3)
{
  float dot1 = -1000.0;
  float dot2 = -1000.0;
  float dot3 = -1000.0;
  for(int s1=0;s1<2;s1++)
    for(int s2=0;s2<2;s2++)
      for(int perm=0;perm<3;perm++)
      {
        vec3 p0 = normalize(vec3(G,1,0))*vec3(s1*2-1,s2*2-1,0);
        if     (perm>1) p0 = p0.yzx;
        else if(perm>0) p0 = p0.zxy;
        float dot0 = dot(pos,p0);
        if(dot0>dot1)
        {
          dot3=dot2; p3=p2;
          dot2=dot1; p2=p1; 
          dot1=dot0; p1=p0;
        }
        else if(dot0>dot2)
        {
          dot3=dot2; p3=p2;
          dot2=dot0; p2=p0; 
        }
        else if(dot0>dot3)
          dot3=dot0; p3=p0;
      }
}

void
getIcosaTri (vec3 pos, out vec3 p1, out vec3 p2, out vec3 p3)
{
  mat2 rot=mat2(0.809016994374947, 0.587785252292473, -0.587785252292473, 0.809016994374947);

  float ph = atan(pos.y,pos.x);  ph=(ph<0.)?PI2+ph:ph;

  float dang=PI2/5.;
  float seg=ph/dang;
  float fseg=floor(seg);

  vec3 p1t=vec3(0,0,1.);
  vec3 p2t=vec3(cos(fseg*dang-vec2(0,PI2/4.))*.894427190999916,.447213595499958);
  vec3 p4t=vec3(rot*p2t.xy,-p2t.z);
  vec3 p3t=vec3(rot*p4t.xy, p2t.z);

  if (dot(pos,cross(p2t,p4t))<0. || dot(pos,cross(p4t,p3t))<0.)
  {
    p1t=vec3(0,0,-1.);
    p3t=vec3((seg-fseg<.5)?p2t.xy*rot:rot*p2t.xy,-p2t.z);
    p4t=vec3(rot*p3t.xy,-p3t.z);
    p2t=vec3(rot*p4t.xy, p3t.z);
  }

  bool top=(dot(pos,cross(p2t,p3t))>0.);
  p1=top?p1t:p4t; 
  p2=top?p2t:p3t; 
  p3=top?p3t:p2t;
}


bool
thruTriangle (vec3 pos, vec3 v1, vec3 v2, vec3 v3)
{
  vec3 n = cross(v2-v1,v3-v1);

  pos = pos*dot(v1,n)/dot(pos,n);
  v1-=pos; v2-=pos; v3-=pos;
  vec3 c1=cross(v1,v2);
  vec3 c2=cross(v2,v3);
  vec3 c3=cross(v3,v1);

  return dot(c1,c2)>0. && dot(c2,c3)>0. && dot(c3,c1)>0.;
}

void
getTriSubDivOld (vec3 pos, inout vec3 p1, inout vec3 p2, inout vec3 p3)
{
  vec3 p4 = normalize(p1+p2);
  vec3 p5 = normalize(p2+p3);
  vec3 p6 = normalize(p3+p1);

  if     (thruTriangle(pos,p1,p4,p6)) { p1=p1; p2=p4; p3=p6; }
  else if(thruTriangle(pos,p6,p5,p3)) { p1=p6; p2=p5; p3=p3; }
  else if(thruTriangle(pos,p6,p4,p5)) { p1=p6; p2=p4; p3=p5; }
  else if(thruTriangle(pos,p4,p2,p5)) { p1=p4; p2=p2; p3=p5; }
}

void
getTriSubDiv (vec3 pos, inout vec3 p1, inout vec3 p2, inout vec3 p3)
{
  mat3 m = mat3((p2-p1)*.5,(p3-p1)*.5,p1);
  vec3 q = inverse(m)*pos;
  q/=q.z;
  vec3 qf=floor(q);
  float f=step(1.,q.x-qf.x+q.y-qf.y);
  p1 = m*vec3( qf.xy+f,            1 );
  p2 = m*vec3( qf.xy+vec2(1.-f,f), 1 );
  p3 = m*vec3( qf.xy+vec2(f,1.-f), 1 );
  p1=normalize(p1);
  p2=normalize(p2);
  p3=normalize(p3);
}

vec4
getRand (vec3 pos)
{
  vec4 r = vec4(1.0);
  r*=texture(iChannel2,pos.xy)*2.-1.;
  r*=texture(iChannel2,pos.xz)*2.-1.;
  r*=texture(iChannel2,pos.zy)*2.-1.;
  return r;
}

float
distTorus (vec3 pos, float r1, float r2, vec3 z)
{
  float pz = dot(pos,normalize(z));
  return length(vec2(length(pos-z*pz)-r1,pz))-r2;
}

float
getRand01Sph (vec3 pos)
{
  return texture(iChannel0,pos.xy*123.+pos.z).x;
}

float
distSphere (vec3 pos, float r)
{
  return length(pos)-r;
}

float
calcAngle(vec3 v1, vec3 v2)
{
  return acos(dot(v1,v2)/length(v1)/length(v2));
}

float
distTruchet (vec3 pos, vec3 p1, vec3 p2, vec3 p3)
{
  float d = 10000.0;
  float rnd=getRand01Sph(p1+p2+p3);

  if      (rnd>.75) { vec3 d=p1; p1=p2; p2=d; }
  else if (rnd>.50) { vec3 d=p1; p1=p3; p3=d; }
  else if (rnd>.25) { vec3 d=p2; p2=p3; p3=d; }

  float r,r1,r2,fact,ang,fullAng;
  vec3 n = normalize(cross(p2-p1,p3-p1));

  vec3 pos2 = ((pos-p1)-dot(pos-p1,n)*n)+p1;

  r1 = .5*length(p2-p1);
  r2 = .5*length(p3-p1);
  ang = calcAngle(pos2-p1,p2-p1);
  fullAng = calcAngle(p3-p1,p2-p1);
  fact = ang/fullAng;
  r=mix(r1,r2,fact);
  d=min(d,distTorus(pos-p1*sqrt(1.0-r*r),r,.1*r,p1));

  r1 = .5*length(p3-p2);
  r2 = .5*length(p1-p2);
  ang = calcAngle(pos2-p2,p3-p2);
  fullAng = calcAngle(p1-p2,p3-p2);
  fact = ang/fullAng;
  r=mix(r1,r2,fact);
  d=min(d,distTorus(pos-p2*sqrt(1.0-r*r),r,.1*r,p2));

  return d;
}

float
dist (vec3 pos)
{
  pos+=.00015*getRand(pos*1.3).xyz*4.;
  pos+=.00006*getRand(pos*3.).xyz*4.;
  pos+=.00040*getRand(pos*.5).xyz*4.;
  vec3 p1,p2,p3;
  float d = 10000.;

  d=min(d,distSphere(pos,.88));

  getIcosaTri(pos, p1, p2, p3);
  getTriSubDiv(pos, p1, p2, p3);

  sortXYZ(p1,p2,p3);
  d=min(d,distTruchet(pos, p1,p2,p3));

  float sc = 1.;

  getTriSubDiv(pos,p1,p2,p3);
  sortXYZ(p1,p2,p3);
  sc = 1./.978;
  d=min(d,distTruchet(pos*sc, p1,p2,p3)/sc);

  getTriSubDiv(pos,p1,p2,p3);
  sortXYZ(p1,p2,p3);
  sc = 1./.965;
  d=min(d,distTruchet(pos*sc, p1,p2,p3)/sc);

  return d;
}

vec3
getGrad (vec3 pos, float eps)
{
  vec2 d=vec2(eps,0);
  float d0=dist(pos);
  return vec3(dist(pos+d.xyy)-d0,
              dist(pos+d.yxy)-d0,
              dist(pos+d.yyx)-d0)/eps;
}

vec4
march (inout vec3 pos, vec3 dir)
{
  if(length(pos-dir*dot(dir,pos))>1.05) 
    return vec4(0,0,0,1);

  float eps=0.001;
  float bg=1.0;
  for(int cnt=0;cnt<32;cnt++)
  {
    float d = dist(pos);
    pos+=d*dir;
    if(d<eps) { bg=0.0; break; }
  }
  vec3 n = getGrad(pos,.001);
  return vec4(n,bg);
}

mat3
rotX (float ang)
{
  float c=cos(ang), s=sin(ang);
  return mat3(1,0,0, 0,c,s, 0,-s,c);
}

mat3
rotZ (float ang)
{
  float c=cos(ang), s=sin(ang);
  return mat3(c,s,0, -s,c,0, 0,0,1);
}

void
main ()
{
  vec2 sc = (gl_FragCoord.xy/iResolution.xy)*2.-1.;

  vec3 pos = vec3(0,-3.5,0);
  vec3 dir = normalize(2.*normalize(-pos)+vec3(sc.x,0,sc.y*iResolution.y/iResolution.x));

  float phi = iMouse.x/iResolution.x*7.;
  float th  = iMouse.y/iResolution.y*7.;
  if (iMouse.x==0.) { phi=iTime*.5; th=.27*.5*iTime; }
  mat3 rx = rotX(th);
  mat3 rz = rotZ(phi);
  pos = rz*(rx*pos);
  dir = rz*(rx*dir);

  vec4 n=march(pos,dir);
  float bg=n.w;

  float ao=1.;
#if 0
  ao*=dist(pos*1.02)/.02;
  ao*=dist(pos*1.05)/.05;
  ao*=dist(pos*1.1)/.1;
#else
  ao*=dist(pos+n.xyz*.02)/.02;
  ao*=dist(pos+n.xyz*.05)/.05;
  ao*=dist(pos+n.xyz*.10)/.10;
#endif
  ao=pow(ao,.4);

  vec3 R = pos-2.0*dot(pos,n.xyz)*n.xyz;
  R = -((R*rz)*rx).yzx;

  vec3 c = vec3(1);
  c += n.xyz*.1+.1;

  c *= texture(iChannel1,R).xyz*1.5+.4;

  c*=clamp(-dot(dir,pos)*.7+.7, .2, 1.);

  c*=ao;

  if(bg>=.5)
    c=vec3(.5,.6,.75)-.17;

  float vign = (1.1-.3*length(sc.xy));

  gl_FragColor = vec4(c*vign,1);
}
