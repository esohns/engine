#version 130

uniform vec2 iResolution;
uniform int iFrame;
uniform sampler2D iChannel1;

#define SIZX 130.0
#define SIZY 66.0

const vec3 windvel = vec3(0.01,0.0,-0.005);
const float gravity = 0.0022;
float mytime=0.0;

vec3
getpos (vec2 uv)
{
  return texture(iChannel1,(uv+0.01)/iResolution.xy).xyz;
}

vec3
getvel (vec2 uv)
{
  return texture(iChannel1,(uv+0.01+vec2(SIZX,0.0))/iResolution.xy).xyz;
}

vec3 pos,vel,ovel;
vec2 c;

void
edge (vec2 dif)
{
  if (
      (dif+c).x>=0.0 && (dif+c).x<SIZX &&
      (dif+c).y>=0.0 && (dif+c).y<SIZY
     )
  {
    float edgelen = length(dif);
    vec3 posdif = getpos(dif+c)-pos;
    vec3 veldif = getvel(dif+c)-ovel;

    vel += normalize(posdif)*(clamp(length(posdif)-edgelen,-1.0,1.0)*0.15); // spring

    //            vel += dot(posdif,veldif)*posdif/dot(posdif,posdif)*0.33; // damper
    vel +=normalize(posdif)*( dot(normalize(posdif),veldif)*0.10); // damper
    //            vel += normalize(posdif)*(-0.005); // spring
  }
}

vec3
findnormal (vec2 c)
{
  return normalize(cross(  getpos(c-vec2(1.0,0.0))-getpos(c+vec2(1.0,0.0)) ,  getpos(c-vec2(0.0,1.0))-getpos(c+vec2(0.0,1.0)) ));
}

vec3 ballpos()
{
  return vec3(128.0*0.4,64.0*0.7,64.0*(cos(mytime/3.0)*0.8));;
}

void ballcollis()
{
  float ballradius = 64.0*0.3;
  vec3 ballpos2 = ballpos();
  if ( length(pos-ballpos2)<ballradius)
  {
      vel -= normalize(pos-ballpos2)*dot(normalize(pos-ballpos2),vel);
      vel += (normalize(pos-ballpos2)*ballradius+ballpos2-pos);
  }
}

void
main ()
{
  mytime = float(iFrame) / 60.0;
  //mytime = getpos(vec2(0.0,SIZY)).x;

  gl_FragColor = vec4 (0.0);
  vec2 fc = gl_FragCoord.xy+0.1;
  fc-=fract(fc);

  if (fc.y>=SIZY+2.0 || fc.x>=SIZX*2.0)
    discard;

  c = fc;
  c.x = fract(c.x/SIZX)*SIZX;

  pos = getpos(c);
  vel = getvel(c);

  ovel = vel;

  edge(vec2(0.0,1.0));
  edge(vec2(0.0,-1.0));
  edge(vec2(1.0,0.0));
  edge(vec2(-1.0,0.0));
  edge(vec2(1.0,1.0));
  edge(vec2(-1.0,-1.0));

  edge(vec2(0.0,2.0));
  edge(vec2(0.0,-2.0));
  edge(vec2(2.0,0.0));
  edge(vec2(-2.0,0.0));
  edge(vec2(2.0,-2.0));
  edge(vec2(-2.0,2.0));

  ballcollis();

  pos += vel;
  vel.y += gravity; // gravity

  vec3 norm = findnormal(c);
  vel -= norm * (dot(norm,vel-windvel)*0.05 );

  if (iFrame==1 || c.y==0.0) // init
  {
    pos = vec3(fc.x*0.85,fc.y,fc.y*0.01);
    vel = vec3(0.0,0.0,0.0);
  }

  gl_FragColor = vec4(fc.x>=SIZX ? vel : pos, 0.0);

  if (fc.y>=SIZY)
  {
    mytime += 1.0/4.0/60.0;
    gl_FragColor = vec4 (mytime);
  }
}
