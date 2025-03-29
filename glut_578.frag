uniform vec2 iResolution;
uniform float iTime;

#define I_MAX   100
#define E       0.0001
#define FAR     30.

#define UNIFORM_ROTATION

vec4  march(vec3 pos, vec3 dir);
vec3  camera(vec2 uv);
vec3  calcNormal(in vec3 pos, float e, vec3 dir);
vec2  rot(vec2 p, vec2 ang);
void  rotate(inout vec2 v, float angle);
float mylength(vec2 p);
float mylength(vec3 p);

float t;
vec3  id;
vec3  h;

void
main ()
{
  h *= 0.;
  t = iTime;
  vec3	col = vec3(0., 0., 0.);
  vec2	R = iResolution.xy, uv  = (gl_FragCoord.xy-R*.5)/R.yy;
  vec3	dir = camera(uv);
  vec3	pos = vec3(-.0, .0, 2.-iTime);

  vec4	inter = (march(pos, dir));

  col = h*1.-.5;

  gl_FragColor = vec4(col, h.x);
}

float
scene (vec3 p)
{
  float	mind = 1e5; 
  p.z -= iTime*-1.725;

  p.y += sin(iTime*-1.+p.z*.5)*.5;
  p.x += cos(iTime*-1.+p.z*.5)*.5;
  rotate(p.xy, p.z*.25 + 1.0*sin(p.z*.125 - iTime*0.5) + 0.*iTime);

  float	tube = max(-(length(p.yx)-2.), (length(p.yx)-8.));
  tube = max(tube, p.z-10.-0./length(p.yx*.06125) );
  tube = max(tube, -p.z-10.-0./length(p.yx*.06125) );
  vec3	pr = p;
  pr.xy = (fract(p.xy*.5)-.5)*1.;
  id = vec3(floor(p.xy*1.-.0), (floor(p.z*.5)-.0)*4.);

  pr.xy = abs(pr.xy)-.2505;
#ifdef UNIFORM_ROTATION
  rotate(pr.xy, iTime);
#else
  rotate(pr.xy, clamp( + (mod(id.x*.5, 2.)-1. <= 0. ? 1. : -1.) + (mod(id.y*.5, 2.)-1. <= 0. ? 1. : -1.), -2., 2.) * iTime*.5 );
#endif
  pr.z = (fract(pr.z*.5)-.5)*4.;
  mind = mylength(vec2(mylength(pr.xyz)-.31, pr.z ))-.084;
  return(mind);
}

vec4
march (vec3 pos, vec3 dir)
{
  vec2	dist = vec2(0.0, 0.0);
  vec3	p = vec3(0.0, 0.0, 0.0);
  vec4	step = vec4(0.0, 0.0, 0.0, 0.0);
  vec3	dirr;

  for (int i = -1; i < I_MAX; ++i)
  {
    dirr = dir;
    rotate(dirr.xy, .51025*dist.y+iTime*-.0 );
    p = pos + dirr * dist.y;
    dist.x = scene(p);
    dist.y += dist.x*.5;
    float	d = 5.;
    h -= vec3(.3, .2, .3)*.1/ (d+.0);
    h += (
        .001/(dist.x*dist.x+0.01) 
        -
        1./(dist.y*dist.y+40.)
          )
        *
        vec3
    (
    1.+(sin(1.0*(id.y+id.x+id.z)+0.00) )
        ,
        1.+(sin(1.0*(id.x+id.y+id.z)+1.04) )
        ,
        1.+(sin(1.0*(id.z+id.x+id.z)+2.08) )
    )*.5;

    if (log(dist.y*dist.y/dist.x/1e5)>0. || dist.x < E || dist.y >= FAR)
    {
      if (dist.x < E || log(dist.y*dist.y/dist.x/1e5)>0.)
        step.y = 1.;
      break;
    }
    step.x++;
  }
  step.w = dist.y;
  return (step);
}

float
mylength (vec3 p)
{
  float	ret = 1e5;

  p = p*p;
  p = p*p;
  p = p*p;

  ret = p.x + p.y + p.z;
  ret = pow(ret, 1./8.);

  return ret;
}

float
mylength (vec2 p)
{
  float	ret = 1e5;

  p = p*p;
  p = p*p;
  p = p*p;

  ret = p.x + p.y;
  ret = pow(ret, 1./8.);

  return ret;
}

void
rotate (inout vec2 v, float angle)
{
  v = vec2(cos(angle)*v.x+sin(angle)*v.y,-sin(angle)*v.x+cos(angle)*v.y);
}

vec2
rot (vec2 p, vec2 ang)
{
  float	c = cos(ang.x);
  float	s = sin(ang.y);
  mat2	m = mat2(c, -s, s, c);
    
  return (p * m);
}

vec3
calcNormal (vec3 pos, float e, vec3 dir)
{
  vec3 eps = vec3(e,0.0,0.0);

  return normalize(vec3(
    march(pos+eps.xyy, dir).w - march(pos-eps.xyy, dir).w,
    march(pos+eps.yxy, dir).w - march(pos-eps.yxy, dir).w,
    march(pos+eps.yyx, dir).w - march(pos-eps.yyx, dir).w ));
}

vec3
camera (vec2 uv)
{
  float fov = 1.;
  vec3  forw  = vec3(0.0, 0.0, -1.0);
  vec3  right = vec3(1.0, 0.0, 0.0);
  vec3  up    = vec3(0.0, 1.0, 0.0);

  return (normalize((uv.x) * right + (uv.y) * up + fov * forw));
}
