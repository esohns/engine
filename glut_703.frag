#version 130

uniform vec2 iResolution;
uniform float iTime;
uniform sampler2D iChannel0;

// Grey: 0, Brown: 1, Blue: 2, Green: 3
#define CELL_COLOR 3

float objID;

mat2 r2 (float a) { float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }

float
smin2 (float a, float b, float r)
{
  float f = max(0., 1. - abs(b - a)/r);
  return min(a, b) - r*.25*f*f;
}

vec2
hash22H (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));

  p = fract(vec2(262144, 32768)*n);

  return sin( p*6.2831853 + iTime )*.3660254 + .5; 
}

vec2
pixToHex (vec2 p)
{
  return floor(vec2(p.x + .57735*p.y, 1.1547*p.y) + 1./3.);
}

vec2
hexPt (vec2 p)
{
  return vec2(p.x - p.y*.5, .866025*p.y) + (hash22H(p) - .5)*.866025/2.;
}

vec3
Voronoi (vec2 p)
{
  vec2 pH = pixToHex(p); // Map the pixel to the hex grid.

  const vec2 hp[7] = vec2[7](vec2(-1), vec2(0, -1), vec2(-1, 0), vec2(0), vec2(1), vec2(1, 0), vec2(0, 1)); 

  vec2 minCellID = vec2(0);

  vec2 mo, o;

  float md = 8., lMd = 8., lMd2 = 8., lnDist, d;

  for (int i=0; i<7; i++)
  {
    vec2 h = hexPt(pH + hp[i]) - p;

    d = dot(h, h);
    if( d<md )
    {
      md = d;
      mo = h; 
      minCellID = hp[i];
    }
  }

  for (int i=0; i<7; i++)
  {
    vec2 h = hexPt(pH + hp[i] + minCellID) - p - mo;

    if(dot(h, h)>.00001)
    {
      lnDist = dot(mo + h*.5, normalize(h));

      lMd = smin2(lMd, lnDist, (lnDist*.5 + .5)*.15);

      lMd2 = min(lMd2, lnDist);
    }
  }

  return max(vec3(lMd, lMd2, md), 0.);
}

float
bumpFunc (vec2 p)
{
  vec3 v = Voronoi(p);

  float c = v.x;

  float ew = .07;

  if(c<ew)
  {
    objID = 1.;

    c = abs(c - ew)/ew;

    c = smoothstep(0., .25, c)/4. + clamp(-cos(c*6.283*1.5) - .5, 0., 1.);
  }
  else
  {
    objID = 0.;
    c = mix(v.x,  v.y, .75);
    c = (c - ew)/(1. - ew);
    c = clamp(c + cos(c*6.283*24.)*.002, 0., 1.);
  }

  return c;
}

void
main ()
{
  vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/min(iResolution.y, 800.);

  vec2 aspect = vec2(iResolution.y/iResolution.x, 1);
  uv *= 1. + dot(uv*aspect, uv*aspect)*.05;

  vec3 r = normalize(vec3(uv.xy, 1.));

  vec2 p = uv*3.5 + vec2(0, iTime*.5);

  float c = bumpFunc(p);

  float svObjID = objID; 

  vec3 sp = vec3(p, 0.);

  vec3 lp = sp + vec3(-1.3*sin(iTime/2.), .8*cos(iTime/2.), -.5);
  vec3 lp2 = sp + vec3(1.3*sin(iTime/2.), -.8*cos(iTime/2.), -.5);

  sp.z -= c*.1;

  vec2 e = vec2(8./iResolution.y, 0);
  float bf = .4;

  if (svObjID>.5)
    e.x = 2./iResolution.y;

  float fx = (bumpFunc(p - e) - bumpFunc(p + e));
  float fy = (bumpFunc(p - e.yx) - bumpFunc(p + e.yx));
  vec3 n = normalize(vec3(fx, fy, -e.x/bf));

  float edge = abs(c*2. - fx) + abs(c*2. - fy);

  vec3 tx = texture(iChannel0, (p + n.xy*.125)*.25).xyz; tx *= tx;
  tx = smoothstep(0., .5, tx);

  vec3 oCol = tx; 

  if(svObjID>.5)
    oCol *= .9;
#if CELL_COLOR > 0
  else
  {
#if CELL_COLOR == 1
    oCol *= vec3(1.4, 1, .7);
#elif CELL_COLOR == 2
    oCol *= vec3(.9, 1.1, 1.3); 
#else
    oCol *= vec3(1.1, 1.4, .7); 
#endif
  }
#endif

  float lDist = length(lp - sp);
  float atten = 1./(1. + lDist*lDist*.5);
  vec3 l = (lp - sp)/max(lDist, .001);
  float diff = max(max(dot(l, n), 0.), 0.);
  float spec = pow(max(dot(reflect(l, n), r), 0.), 64.);

  float lDist2 = length(lp2 - sp);
  float atten2 = 1./(1. + lDist2*lDist2*.5);
  vec3 l2 = (lp2 - sp)/max(lDist2, .001);
  float diff2 = max(max(dot(l2, n), 0.), 0.);
  float spec2 = pow(max(dot(reflect(l2, n), r), 0.), 64.);

  diff = pow(diff, 4.)*2.;
  diff2 = pow(diff2, 4.)*2.;

  vec3 col = oCol*(diff*vec3(.5, .7, 1) + .25 + vec3(.25, .5, 1)*spec*32.)*atten*.5;

  col += oCol*(diff2*vec3(1, .7, .5) + .25 + vec3(1, .3, .1)*spec2*32.)*atten2*.5;

  col *= edge;

  vec2 u = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*u.x*u.y*(1. - u.x)*(1. - u.y) , .125);

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
