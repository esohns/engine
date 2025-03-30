uniform vec2 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;

#define AA 1
#define PATH_LENGTH 3
#define MAX_DIST 60.
#define MIN_DIST .001
#define ZERO (min(iFrame,0))

float time;
vec2 activeSpheres[2];
vec2 joints[3];
float joint0Rot;
float jointYRot;

vec2
hash22 (vec2 p)
{
  vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
  p3 += dot(p3, p3.yzx+33.33);
  return fract((p3.xx+p3.yz)*p3.zy);
}

float dot2 (vec3 v) { return dot(v,v); }

float
iPlane (vec3 ro, vec3 rd, vec2 distBound, out vec3 normal, vec3 planeNormal, float planeDist)
{
  float a = dot(rd, planeNormal);
  float d = -(dot(ro, planeNormal)+planeDist)/a;
  if (a > 0. || d < distBound.x || d > distBound.y)
    return MAX_DIST;
  normal = planeNormal;
  return d;
}

float
iSphere (vec3 ro, vec3 rd, vec2 distBound, out vec3 normal, float sphereRadius)
{
  float b = dot(ro, rd);
  float c = dot(ro, ro) - sphereRadius*sphereRadius;
  float h = b*b - c;
  if (h < 0.)
    return MAX_DIST;

  h = sqrt(h);
  float d1 = -b-h;
  float d2 = -b+h;
  if (d1 >= distBound.x && d1 <= distBound.y)
  {
    normal = normalize(ro + rd*d1);
    return d1;
  }

  return MAX_DIST;
}

float
iCylinder (vec3 oc, vec3 rd, vec2 distBound, out vec3 normal, vec3 ca, float ra, bool traceCaps)
{
  float caca = dot(ca,ca);
  float card = dot(ca,rd);
  float caoc = dot(ca,oc);

  float a = caca - card*card;
  float b = caca*dot( oc, rd) - caoc*card;
  float c = caca*dot( oc, oc) - caoc*caoc - ra*ra*caca;
  float h = b*b - a*c;

  if (h < 0.)
    return MAX_DIST;

  h = sqrt(h);
  float d = (-b-h)/a;

  float y = caoc + d*card;
  if (y >= 0. && y <= caca && d >= distBound.x && d <= distBound.y)
  {
    normal = (oc+d*rd-ca*y/caca)/ra;
    return d;
  }
  else if(!traceCaps)
  {
    return MAX_DIST;
  }
  d = ((y < 0. ? 0. : caca) - caoc)/card;

  if( abs(b+a*d) < h && d >= distBound.x && d <= distBound.y)
  {
    normal = normalize(ca*sign(y)/caca);
    return d;
  }

  return MAX_DIST;
}

float
iCone (vec3 oa, vec3 rd, vec2 distBound, out vec3 normal, vec3 pb, float ra, float rb)
{
  vec3  ba = pb;
  vec3  ob = oa - pb;

  float m0 = dot(ba,ba);
  float m1 = dot(oa,ba);
  float m2 = dot(ob,ba); 
  float m3 = dot(rd,ba);

  if (m2 > 0. && dot2(ob*m3-rd*m2) < (rb*rb*m3*m3) )
  {
    float d = -m2 / m3;
    if (d > distBound.x && d < distBound.y)
    {
      normal = ba*inversesqrt(m0);
      return d;
    }
  }

  float m4 = dot(rd,oa);
  float m5 = dot(oa,oa);
  float rr = ra - rb;
  float hy = m0 + rr*rr;

  float k2 = m0*m0    - m3*m3*hy;
  float k1 = m0*m0*m4 - m1*m3*hy + m0*ra*(rr*m3*1.0        );
  float k0 = m0*m0*m5 - m1*m1*hy + m0*ra*(rr*m1*2.0 - m0*ra);

  float h = k1*k1 - k2*k0;
  if( h < 0. )
    return MAX_DIST;

  float t = (-k1-sqrt(h))/k2;

  float y = m1 + t*m3;
  if (y > 0. && y < m0 && t >= distBound.x && t <= distBound.y)
  {
    normal = normalize(m0*(m0*(oa+t*rd)+rr*ba*ra)-ba*hy*y);
    return t;
  }

  return MAX_DIST;
}

float
iBox (vec3 ro, vec3 rd, vec2 distBound, inout vec3 normal, vec3 boxSize)
{
  vec3 m = sign(rd)/max(abs(rd), 1e-8);
  vec3 n = m*ro;
  vec3 k = abs(m)*boxSize;
  
  vec3 t1 = -n - k;
  vec3 t2 = -n + k;

  float tN = max( max( t1.x, t1.y ), t1.z );
  float tF = min( min( t2.x, t2.y ), t2.z );

  if (tN > tF || tF <= 0.)
    return MAX_DIST;

  if (tN >= distBound.x && tN <= distBound.y)
  {
    normal = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
    return tN;
  }
  else if (tF >= distBound.x && tF <= distBound.y)
  {
  //  normal = sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
    return tF;
  }

  return MAX_DIST;
}

vec3
FresnelSchlick (vec3 SpecularColor, vec3 E, vec3 H)
{
  return SpecularColor + (1. - SpecularColor) * pow(1.0 - max(0., dot(E, H)), 5.);
}

vec2
randomInUnitDisk (vec2 seed)
{
  vec2 h = hash22(seed) * vec2(1.,6.28318530718);
  float phi = h.y;
  float r = sqrt(h.x);
  return r*vec2(sin(phi),cos(phi));
}

vec2
activeSphereGrid (float t)
{
  vec2 p = randomInUnitDisk(vec2(floor(t),.5));
  return floor(p * 8.5 + 1.75*normalize(p));
}

vec3
sphereCenter (vec2 pos)
{
  vec3 c = vec3(pos.x, 0., pos.y)+vec3(.25,.25,.25);
  c.xz += .5*hash22(pos);
  return c;
}

vec3
sphereCol (float t)
{
  return normalize(.5 + .5*cos(6.28318530718*(1.61803398875*floor(t)+vec3(0,.1,.2))));
}

void
initDynamics ()
{
  time = iTime * .25;

  activeSpheres[0] = activeSphereGrid(time);
  activeSpheres[1] = activeSphereGrid(time+1.);

  vec3 ta0 = sphereCenter(activeSpheres[0]);
  vec3 ta1 = sphereCenter(activeSpheres[1]);

  float taa0 = atan(-ta0.z, ta0.x);  
  float taa1 = atan(-ta1.z, ta1.x);

  if (abs(taa0-taa1) > 3.14159265359)
  {
    taa1 += taa1 < taa0 ? 2. * 3.14159265359 : -2. * 3.14159265359;  
  }
  jointYRot = mix(taa0, taa1, clamp(fract(time)*2.-.5,0.,1.));    

  float tal = mix(length(ta0), length(ta1), clamp(fract(time)*2.5-1.,0.,1.));

  vec2 target = vec2(tal,.5-.5*smoothstep(.35,.4,abs(fract(time)-.5)));  

  float c0 = length(target);
  float b0 = min(11., 4. + 2. * c0 / 11.);

  vec2 sd = normalize(target);
  float t0 = asin(sd.y)+acos(-(b0*b0-25.-c0*c0)/(10.*c0));

  joints[0] = vec2(5. * cos(t0), 5.* sin(t0));
  joint0Rot = t0;

  sd = normalize(target-joints[0]);  
  float c1 = min(6., distance(joints[0], target));
  const float b1 = 2.;  

  float t1 = asin(sd.y) * sign(sd.x) + acos(-(b1*b1-16.-c1*c1)/(8.*c1));
  t1 += sd.x < 0. ? 3.1415 : 0.;
  joints[1] = joints[0] + 4. * vec2(cos(t1),sin(t1));
  joints[2] = target;
}

vec3
opU (vec3 d, float iResult, float mat)
{
  return (iResult < d.y) ? vec3(d.x, iResult, mat) : d;
}

vec3
iPlaneInt (vec3 ro, vec3 rd, float d)
{
  d = -(ro.y - d) / rd.y;
  return ro + d * rd;
}

vec3
traceSphereGrid (vec3 ro, vec3 rd, vec2 dist, out vec3 normal, int maxsteps)
{
  float m = 0.;
  if (ro.y < .5 || rd.y < 0.)
  {
    vec3 ros = ro.y < .5 ? ro : iPlaneInt(ro, rd, .5);
    if (length(ros.xz) < 11.)
    {
      vec3 roe = iPlaneInt(ro, rd,rd.y < 0. ?0.:.5);
      vec3 pos = floor(ros);
      vec3 rdi = 1./rd;
      vec3 rda = abs(rdi);
      vec3 rds = sign(rd);
      vec3 dis = (pos-ros+ .5 + rds*.5) * rdi;
      bool b_hit = false;

      vec2 mm = vec2(0);
      for (int i = ZERO; i<maxsteps; i++)
      {
        float l = length(pos.xz+.5);
        if (pos.y > .5 || pos.y < -1.5 || l > 11.)
          break;
        else if ( l > 2. && pos.y > -.5 && pos.y < 1.5 )
        {
          float d = iSphere(ro-sphereCenter(pos.xz), rd, dist, normal, .25);
          if (d < dist.y)
          {
            m = 2.;
            dist.y = d;
            break;
          }
        }
        vec3 mm = step(dis.xyz, dis.yxy) * step(dis.xyz, dis.zzx);
        dis += mm*rda;
        pos += mm*rds;
      }
    }
  }

  return vec3(dist, m);
}

vec3
rotateY (vec3 p, float t)
{
  float co = cos(t);
  float si = sin(t);
  vec2 xz = mat2(co,si,-si,co)*p.xz;
  return vec3(xz.x, p.y, xz.y);
}

vec3
worldhit (vec3 ro, vec3 rd, vec2 dist, out vec3 normal)
{
  vec3 d = vec3(dist, 0.);

  d = traceSphereGrid(ro, rd, d.xy, normal, 10);

  d = opU(d, iPlane   (ro, rd, d.xy, normal, vec3(0,1,0), 0.), 1.);
  d = opU(d, iCone    (ro-vec3(0,.2,0), rd, d.xy, normal, vec3(0,.2,0), 1.5, 1.4), 4.);
  d = opU(d, iCylinder(ro, rd, d.xy, normal, vec3(0,.2,0), 1.5, false), 4.);

  float dmax = d.y;
  vec3 roa = rotateY(vec3(ro.x, ro.y-1., ro.z), jointYRot);    
  vec3 rda = rotateY(rd, jointYRot); 

  vec3 bb = vec3(.5*max(joints[1].x,joints[2].x), joints[0].y*.5, .0);
  vec3 bbn;

  if (iBox(roa-bb, rda, vec2(0,100), bbn, bb+vec3(.75,.75,.8)) < 100.)
  {
    vec3 dr = vec3(-sin(joint0Rot), cos(joint0Rot), 0);
    vec2 j21 = joints[2]-joints[1];

    for (int axis=0; axis<=1; axis++)
    {
      float a = axis == 0 ? -1. : 1.;
      d = opU(d, iCylinder(roa-vec3(0,0,a*.67), rda, d.xy, normal, vec3(0,0,-a*.2),.55, true), 3.);
      d = opU(d, iCylinder(roa-vec3(0,0,a*.58)-.4*dr, rda, d.xy, normal, vec3(joints[0],-a*.24)-.24*dr,.07, false), 4.);
      d = opU(d, iCylinder(roa-vec3(0,0,a*.58)+.4*dr, rda, d.xy, normal, vec3(joints[0],-a*.24)+.24*dr,.07, false), 4.);
      d = opU(d, iCylinder(roa-vec3(joints[0],a*.45), rda, d.xy, normal, vec3(0,0,-a*.2),.35, true), 3.);
      d = opU(d, iCylinder(roa-vec3(joints[1],a*.29), rda, d.xy, normal, vec3(0,0,-a*.08),.25, true), 3.);
      d = opU(d, iCylinder(roa-vec3(joints[1],a*.24), rda, d.xy, normal, vec3(j21,a*.08),.03, false), 4.);
    }

    vec2 j10 = joints[1]-joints[0];
    d = opU(d, iCylinder(roa-vec3(0,0,-.72), rda, d.xy, normal, vec3(0,0,1.44),.5, true), 5.);
    d = opU(d, iBox     (roa+vec3(0,.5,0), rda, d.xy, normal, vec3(.5,.5,.47)), 5.);
    d = opU(d, iCone    (roa-vec3(joints[0],0), rda, d.xy, normal, vec3(j10,0),.25, .15), 5.);
    d = opU(d, iCylinder(roa-vec3(joints[0],-.5), rda, d.xy, normal, vec3(0,0,1.),.3, true), 5.);
    d = opU(d, iCylinder(roa-vec3(joints[1],-.35), rda, d.xy, normal, vec3(0,0,.7),.2, true), 5.);
    d = opU(d, iCylinder(roa-vec3(joints[2],-.4), rda, d.xy, normal, vec3(0,0,.8),.2, true), 3.);
    d = opU(d, iSphere  (roa-vec3(joints[2],0), rda, d.xy, normal, .32), 5.);
    d = opU(d, iCylinder(roa-vec3(joints[2],0), rda, d.xy, normal, vec3(0,-.5,0),.06, true), 3.);

    if (d.y < dmax)
      normal = rotateY(normal, -jointYRot);
  }

  return d;
}

float
shadowhit (vec3 ro, vec3 rd, float dist)
{
  vec3 normal;
  float d = traceSphereGrid( ro, rd, vec2(.3, dist), normal, 4).y;
  d = min(d, iCylinder(ro, rd, vec2(.3, dist), normal, vec3(0,.2,0), 1.5, false));
  return d < dist-0.001 ? 0. : 1.;
}

float
getSphereLightIntensity (float num)
{
  return num > .5 ? clamp(fract(time)*10.-1., 0., 1.) : max(0., 1.-fract(time)*10.);
}

float
getLightIntensity (vec3 pos, vec3 normal, vec3 light, float intensity)
{
  vec3 rd = pos - light;
  float i = max(0., dot(normal, -normalize(rd)) / dot(rd,rd));
  i = i > 0.0001 ? i * intensity * shadowhit(light, normalize(rd), length(rd)) : 0.;
  return max(0., i-0.0001);              
}

vec3
getLighting (vec3 p, vec3 normal)
{
  vec3 l = vec3(0.);

  float i = getSphereLightIntensity(0.);
  if (i > 0.)
    l += sphereCol(time) * (i * getLightIntensity(p, normal, sphereCenter(activeSpheres[0]), .375));
  else
  {
    i = getSphereLightIntensity(1.);
    if (i > 0.)
      l += sphereCol(time+1.) * (i * getLightIntensity(p, normal, sphereCenter(activeSpheres[1]), .25));
  }

  vec3 robot = mix(sphereCol(time), sphereCol(time-1.), getSphereLightIntensity(0.));
  vec3 lp = rotateY(vec3(joints[2].x, joints[2].y+1.,0), -jointYRot);
  i = getLightIntensity(p, normal, lp, .5);
  i += getLightIntensity(p, normal, vec3(0,2,0), .25);
  l += i * robot;

  return l;
}

vec3
getEmissive (vec2 pos, float mat)
{
  if (mat > 2.5 )
  {
    return mix(sphereCol(time), sphereCol(time-1.), getSphereLightIntensity(0.));
  }
  else if (mat > 1.5 )
  {
    float li0 = getSphereLightIntensity(0.);
    float li1 = getSphereLightIntensity(1.);
    if (li0 > 0. && pos == activeSpheres[0])
      return sphereCol(time) * li0 * 1.25;
    if (li1 > 0. && pos == activeSpheres[1])
      return sphereCol(time+1.) * li1;
    return vec3(0);
  }

  return vec3(0);
}

vec3
render (vec3 ro, vec3 rd)
{
  vec3 col = vec3(1);
  vec3 emitted = vec3(0);
  vec3 normal;

  for (int i=ZERO; i<PATH_LENGTH; ++i)
  {
    vec3 res = worldhit( ro, rd, vec2(MIN_DIST, MAX_DIST-1.), normal );
    if (res.z > 0.)
    {
      ro += rd * res.y;

      if (res.z < 3.5)
      {
        vec3 F = FresnelSchlick(vec3(0.4), normal, -rd);
        emitted += (col * (getEmissive(floor(ro.xz), res.z) + .5 * getLighting(ro, normal))) * (1.-F);
        col *= .5 * F;
      }
      else
        col *= .15;   

      rd = normalize(reflect(rd,normal));
    }
    else
      return emitted;
  }

  return emitted;
}

mat3
setCamera (vec3 ro, vec3 ta, float cr )
{
  vec3 cw = normalize(ta-ro);
  vec3 cp = vec3(sin(cr), cos(cr),0.0);
  vec3 cu = normalize( cross(cw,cp) );
  vec3 cv =          ( cross(cu,cw) );
  return mat3( cu, cv, cw );
}

void
main ()
{
  initDynamics();

  vec2 mo = iMouse.xy == vec2(0) ? vec2(.4,-.1) : abs(iMouse.xy)/iResolution.xy - .5;

  vec3 ro = vec3(10.5*cos(1.5+6.*mo.x), 6.+10.*mo.y, 8.5*sin(1.5+6.*mo.x));
  vec3 ta = vec3(ro.x*ro.y*.02, .8, 0);
  mat3 ca = setCamera(ro, ta, 0.);    

  vec3 col = vec3(0);

#if AA>1
  for( int m=ZERO; m<AA; m++ )
    for( int n=ZERO; n<AA; n++ )
    {
      vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
      vec2 p = (-iResolution.xy + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else    
      vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
#endif
      vec3 rd = ca * normalize( vec3(p.xy,1.6) );  
      col += pow(8. * render(ro, rd), vec3(1./2.2));
#if AA>1
    }
    col /= float(AA*AA);
#endif

  col = clamp(col + ((hash22(gl_FragCoord.xy).x-.5)/64.), vec3(0), vec3(1));

  gl_FragColor = vec4(col , 1);
}
