#version 330

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define FK(k) floatBitsToInt(cos(k))^floatBitsToInt(k)

float
hash (vec2 k)
{
  int x = FK(k.x);
  int y = FK(k.y);
  return float((x*x-y)*(y*y+x)-x)/2.14e9;
}

float
hash3 (vec3 k)
{
  float h1 = hash(k.xy);
  return hash(vec2(h1, k.z));
}

vec3
hash33 (vec3 k)
{
  float h1 = hash3(k);
  float h2 = hash3(k*h1);
  float h3 = hash3(k*h2);
  return vec3(h1, h2, h3);
}

float
smin (float a, float b, float k)
{
  float h = max( k-abs(a-b), 0.0 )/k;
  return min( a, b ) - h*h*h*k*(1.0/6.0);
}

vec3
sphercoord (vec2 p)
{
  float l1 = acos(p.x);
  float l2 = acos(-1.)*p.y;
  return vec3(cos(l1), sin(l1)*sin(l2), sin(l1)*cos(l2));
}

vec3
erot (vec3 p, vec3 ax, float ro)
{
  return mix(dot(p,ax)*ax, p, cos(ro)) + sin(ro)*cross(p,ax);
}

float
comp (vec3 p, vec3 ro, float t)
{
  vec3 ax = sphercoord(ro.xy);
  p.z -= t;
  p = erot(p, ax, ro.z*acos(-1.));
  float scale = 4. + hash(ro.xz)*0.5+0.5;
  p = (fract(p/scale)-0.5)*scale;
  return length(p) - 0.8;
}

float
scene (vec3 p)
{
  float rad = 3.+p.z+sin(p.y/2.+iTime)+cos(p.x/3.+iTime*0.9);
  float dist = 10000.;
  for (int i = 0; i < 4; i++)
  {
    vec3 rot = hash33(vec3(float(i+1), cos(float(i)), sin(float(i))));
    float d = comp(p, rot, iTime/2.*(float(i+1)));
    dist = smin(dist, d, 1.);
  }

  return mix(dist, rad, mix(0.3,0.8+sin(iTime)*0.2, 0.1));
}

vec3
norm (vec3 p)
{
  mat3 k = mat3(p,p,p)-mat3(0.1);
  return normalize(scene(p) - vec3(scene(k[0]),scene(k[1]),scene(k[2])));
}

float drawdist = 50.;
vec3
trace (vec3 init, vec3 cam, int itcout, inout bool hit)
{
  vec3 p = init;
  hit = false;
  float dist;
  for (int i = 0; i < itcout; i++)
  {
    dist = scene(p)*1.1;
    if (dist*dist < 0.00001)
    {
      hit = true;
      p += dist*cam;
      break;
    }
    if (distance(p, init) > drawdist)
      break;
    p += dist*cam*1.2;
  }

  return p;
}

float
refcomp (vec3 p, vec3 cam, float n)
{
  float dst = scene(p+cam*n);
  float sharpness = 1.0;
  return smoothstep(-1., 1., (dst*sharpness));
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  uv -= 0.5;
  uv /= vec2(iResolution.y / iResolution.x, 1);

  vec3 cam = normalize(vec3(0.8,uv));
  bool hit = false;
  vec3 col = vec3(1);
  vec3 p, n;
  vec3 init = vec3(-4,0,sin(iTime*0.37)*1.4);
  vec2 m = iMouse.xy/iResolution.xy;
  if (iMouse.z != 1.)
    m = vec2(0.4);
  cam = erot(cam, vec3(0,1,0), -m.y*2.+0.5);
  init = erot(init, vec3(0,1,0), -m.y*2.+0.5);
  cam = erot(cam, vec3(0,0,1), -m.x*6.);
  init = erot(init, vec3(0,0,1), -m.x*6.);
  vec3 origin = init;
  p = trace(init, cam, 400, hit);
  if (hit)
  {
    n = norm(p);
    float fresnel = pow(1.-abs(dot(cam, n))*0.5,2.);
    col *= fresnel;
    cam = reflect(cam, n);
    hit = false;
    init = p+n*.01;
    float comps = refcomp(init, cam, 5.) + refcomp(init, cam, 4.)*2. + refcomp(init, cam, 2.)*3. + refcomp(init, cam, 1.)*4.;
    comps /= 10.;
    float dst2 = max(scene(p+n*2.)/2.*0.5+0.5,0.);
    col = dst2*vec3(1.)*(pow(comps,2.))*fresnel;
  }

  float fog = pow(smoothstep(drawdist*.5,drawdist,distance(p,origin)),2.);
  gl_FragColor.xyz = mix(col*0.8+0.02, vec3(1),fog);

  gl_FragColor.xyz = pow(smoothstep(vec3(0), vec3(1), sqrt(gl_FragColor.xyz)), vec3(1.7, 1.6, 1.5));
}
