uniform vec2 iResolution;
uniform float iTime;

#define SCENE 0
//#define BUMPY
#define AUTOCAM
#define SPIN

vec3 sunDir = normalize(vec3(0.0,0.3,1.0));

#define pee 3.141592653
#ifdef AUTOCAM
#define anglex2 (sin(iTime*0.3)*0.4)
#define angley2 (iTime*0.2-0.4)
#else
float anglex2 = (0.5 - iMouse.y/iResolution.y)*pee*1.2; // mouse cam
float angley2 = -iMouse.x/iResolution.x*pee*2.0;
#endif

vec3 campos;
vec3 dir;
vec3
backGround2 ()
{
  if (dir.y>0.0) return vec3(1,1,1);
  vec2 floorcoords = campos.xz + dir.xz*(-campos.y/dir.y);
  vec2 t = (fract(floorcoords.xy*0.5))-vec2(0.5,0.5);
  return vec3(1,1,1) - vec3(0.6,0.3,0)*float(t.x*t.y>0.0);
}

float
texture_iq_heart (vec2 pos)
{
  float rot = pos.x;
  pos.x += pos.y*0.3;
  pos.y -= rot*0.3;
  pos=(fract((pos))-vec2(0.5,0.7))*0.8;
  float f1 = abs(atan(pos.x,pos.y)/pee);
  return (f1*6.5 - f1*f1*11.0 + f1*f1*f1*5.0)/(6.0-f1*5.0)-length(pos);
}

vec3
sky ()
{
  float f = max(dir.y,0.0);
  vec3 color = 1.0-vec3(1,0.85,0.7)*f;
  color *= dir.z*0.2+0.8;

  if (dot(sunDir,dir)>0.0)
  {
    f = max(length(cross(sunDir,dir))*10.0,1.0);
    color += vec3(1,0.9,0.7)*40.0/(f*f*f*f);
  }
  return color;
}

vec3
backGround ()
{
  if (dir.y>=0.0) return sky();
  vec3 raypos2 = campos - dir*(campos.y / dir.y);
  float fog = exp(length(raypos2)/-8.0);
  return sky()*(1.0-fog)+(vec3(0.3,0.5,0.7)+vec3(0.3,0.15,0.0)*((clamp(texture_iq_heart(raypos2.xz)*20.0,0.0,1.0))))*fog;
}

vec3
rotatex (vec3 v,float anglex)
{
  float t;
  t =   v.y*cos(anglex) - v.z*sin(anglex);
  v.z = v.z*cos(anglex) + v.y*sin(anglex);
  v.y = t;
  return v;
}

vec3
rotcam (vec3 v)
{
  float t;
  v = rotatex(v,anglex2);

  t = v.x * cos(angley2) - v.z*sin(angley2);
  v.z = v.z*cos(angley2) + v.x*sin(angley2);
  v.x = t;
  return v;
}

int side;
float gTravel;
vec3 gNormal;
float travelMax,travelMin;
vec3 normalMax,normalMin;

void
update (float surfaceside, float travel, vec3 normal)
{
  if (surfaceside<0.0)
  {
    if (travelMax<travel)
    {
      travelMax = travel;
      normalMax = normal;
    }
  }
  else
  {
    if (travelMin>travel)
    {
      travelMin = travel;
      normalMin = normal;
    }
  }
}

void
hitPlane (vec3 normal, float shift)
{
#ifdef SPIN
  float angle = fract(iTime*0.25);
  angle = min(angle*1.5,1.0);
  normal = rotatex(normal,angle*pee*2.0);
#endif
  shift += normal.y*1.0;

  float distFromPlane = dot(normal,campos) - shift;
  float travel = -distFromPlane / dot(normal,dir);
  update(dot(normal,dir),travel,normal);
}

void
startObj ()
{
  travelMax = -1e35;
  travelMin = 1e35;
}

void
endObj ()
{
  {
    if (side>0)
    {
      if (travelMax<travelMin && travelMax>0.0 && travelMax<gTravel)
      {
        gTravel = travelMax;
        gNormal = normalMax;
      }
    }
    else
    {
      if (travelMin>0.0 && travelMin<gTravel)
      {
        gTravel = travelMin;
        gNormal = -normalMin;
      }
    }
  }
}

void
hitKocka ()
{
  startObj ();

  if (SCENE==0)
  {
    hitPlane(vec3(1,0,0),0.6);
    hitPlane(vec3(-1,0,0),0.6);
    hitPlane(vec3(0,0,1),0.2);
    hitPlane(vec3(0,0,-1),0.2);
    hitPlane(vec3(0,1,0),0.75);
    hitPlane(vec3(0,-1,0),0.75);
    hitPlane(normalize(vec3(1,1,0)),0.6); // cut off it's edge
  }
  
  if (SCENE==1)
  {
    for(float angle=0.0;angle<pee*2.0;angle+=pee/4.0)
    {
      hitPlane(vec3(sin(angle),0.5,cos(angle)),0.4);
      hitPlane(vec3(sin(angle),-2.0,cos(angle)),1.5);
    }
  }
  
  if (SCENE==2)
  {
    for(float angle2=pee/8.0;angle2<pee;angle2+=pee/6.0)
      for(float angle=0.0;angle<pee*2.0;angle+=pee/6.0)
      {
        hitPlane(vec3(sin(angle)*sin(angle2),cos(angle2),cos(angle)*sin(angle2)),0.7);
      }
  }

  endObj ();
}

vec3
getInsideFake ()
{
  gTravel = 1e35;
  hitKocka ();
  if (gTravel>1e34) { return backGround (); }
  return vec3(  dot(vec3(0.7,0.7,0.2),gNormal)*0.5+0.5,0,0);
}

vec3
glassColorFunc (float dist)
{
  if(side>0)
    return vec3(1,1,1);
  return vec3(exp(dist*-0.4),exp(dist*-0.05),exp(dist*-0.2));
}

vec3
black ()
{
  return vec3 (0.0);
}

void bumpit()
{
#ifdef BUMPY
  gNormal.x += sin(campos.x*30.0)*0.007;
  gNormal.y += sin(campos.y*30.0)*0.007;
  gNormal.z += sin(campos.z*30.0)*0.007;
  gNormal = normalize(gNormal);
#endif
}

// recursion unsupported, let's overcome it like this
// CHILD0 refracted ray proc
// CHILD1 reflected ray proc
#define GET(BASE,CHILD0,CHILD1) vec3 BASE(){if (!(length(dir)<1.01) || !(length(dir)>0.99)) return vec3(0.0,0.0,0.0); gTravel = 1e35; hitKocka();if (gTravel>1e34){return  backGround();}campos += dir * gTravel;bumpit();vec3 glassColor = glassColorFunc(gTravel); vec3 originalDir = dir;	vec3 originalPos = campos;	vec3 originalNormal = gNormal;	dir = refract(originalDir,originalNormal,side>0 ? 1.0/1.55 : 1.55);  float t = clamp(1.0+dot(gNormal,side>0?originalDir : dir),0.0,1.0);	float fresnel = 0.1 + (t*t*t*t*t)*0.9;		side *=-1; vec3 color =  CHILD0()*(1.0-fresnel);	side *=-1; campos = originalPos;	dir = reflect(originalDir,originalNormal);	 color += CHILD1()*(fresnel);  return color*glassColor;	}
// having to deal with just one convex object, any ray refracting out of it is casted 
// to the background directly without other ray checking
GET(get8,backGround,black) 
GET(get7,backGround,get8)
GET(get6,backGround,get7)
GET(get5,backGround,get6)
GET(get4,backGround,get5)
GET(get3,backGround,get4)
GET(get2,backGround,get3) 
GET(get,get2,backGround)

float
func (float x)
{
  return x/(x+3.0)*3.0;
}

vec3
HDR (vec3 color)
{
  float pow = length(color);
  return color * func(pow)/pow*1.2;
}

void
main ()
{
  float brightNess = min(iTime/5.0,1.0);
  vec2 uv = gl_FragCoord.xy / iResolution.xy;

  campos = vec3(0,1.0,0);
  dir = vec3(uv*2.0-1.0,1);
  dir.y *= 9.0/16.0;
  dir = normalize(rotcam(dir));

  campos -= rotcam(vec3(0,0,2)); // back up from subject

  gTravel = 1e35;
  side = 1;

  gl_FragColor = vec4((get()*brightNess),1.0);
}
