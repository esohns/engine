#version 130

// glut_991_common.glsl
#define PHI (sqrt(5.)*0.5 + 0.5)

#define GDFVector3 normalize(vec3(1, 1, 1 ))
#define GDFVector4 normalize(vec3(-1, 1, 1))
#define GDFVector5 normalize(vec3(1, -1, 1))
#define GDFVector6 normalize(vec3(1, 1, -1))

#define GDFVector7 normalize(vec3(0, 1, PHI+1.))
#define GDFVector8 normalize(vec3(0, -1, PHI+1.))
#define GDFVector9 normalize(vec3(PHI+1., 0, 1))
#define GDFVector10 normalize(vec3(-PHI-1., 0, 1))
#define GDFVector11 normalize(vec3(1, PHI+1., 0))
#define GDFVector12 normalize(vec3(-1, PHI+1., 0))

#define GDFVector13 normalize(vec3(0, PHI, 1))
#define GDFVector14 normalize(vec3(0, -PHI, 1))
#define GDFVector15 normalize(vec3(1, 0, PHI))
#define GDFVector16 normalize(vec3(-1, 0, PHI))
#define GDFVector17 normalize(vec3(PHI, 1, 0))
#define GDFVector18 normalize(vec3(-PHI, 1, 0))

#define fGDFBegin float d = 0.;
#define fGDF(v) d = max(d, abs(dot(p, v)));
#define fGDFEnd return d - r;

float
fDodecahedron (vec3 p, float r)
{
  fGDFBegin
  fGDF(GDFVector13)fGDF(GDFVector14)fGDF(GDFVector15)fGDF(GDFVector16)
  fGDF(GDFVector17)fGDF(GDFVector18)
  fGDFEnd
}

float
fIcosahedron (vec3 p, float r)
{
  fGDFBegin
  fGDF(GDFVector3)fGDF(GDFVector4)fGDF(GDFVector5)fGDF(GDFVector6)
  fGDF(GDFVector7)fGDF(GDFVector8)fGDF(GDFVector9)fGDF(GDFVector10)
  fGDF(GDFVector11)fGDF(GDFVector12)
  fGDFEnd
}
// glut_991_common.glsl

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform samplerCube iChannel0;

#if HW_PERFORMANCE==0
#else
#define AA
#endif

#define MAX_DIST        60.
#define PI              radians(180.)
#define TAU             radians(360.)
#define MAX_BOUNCES     4

#define GAMMA           0

float
map (vec3 p, out vec3 absorb, out vec3 refcol)
{
  float id  = fDodecahedron(p, 0.8);
  float od = fDodecahedron(p, 0.805);
  float oi = fIcosahedron(p, 1.5);

  float d = min(max(oi, -od), id);
  if (d == id)
  {
    absorb = vec3(0, 1, 1) * 10.;
    refcol = vec3(1,0.5,0.5);
  }
  else
  {
    absorb = vec3(0);
    refcol = vec3(1);
  }
  return oi;
}

vec3
getSkyColor (vec3 rd)
{
  vec3 col = texture(iChannel0, rd).rgb;
#if GAMMA
  col = pow(col, vec3(2.2));
#endif
  return col;
}

vec3
normal (vec3 pos)
{
  vec3	_absorb;
  vec3	_refcolor;
  vec2	eps = vec2(0.0001, 0);
  float	d = map(pos, _absorb, _refcolor);

  return normalize(vec3(map(pos + eps.xyy, _absorb, _refcolor) - d, map(pos + eps.yxy, _absorb, _refcolor) - d, map(pos + eps.yyx, _absorb, _refcolor) - d));
}

float
rayMarch (float sgn, vec3 ro, vec3 rd, float offT, out vec3 absorb, out vec3 refcolor)
{
  float t = offT;
  for (int i = 0; i < 16; i++)
  {
    float h = sgn * map(ro + rd*t, absorb, refcolor);
    if (h < 0.0005 || t > MAX_DIST)
      break;
    t += h;
  }

  return t;
}

vec3
Render (vec3 ro, vec3 rd, float cref)
{
  float sgn = 1.;
  vec3  rel = vec3(1.);
  vec3  col = vec3(0);
  float transp = 1.;
  for(int i = 0; i < MAX_BOUNCES; i++)
  {
    vec3	absorb;
    vec3	refcolor;
    float t = rayMarch(sgn, ro, rd, 0.003, absorb,refcolor);
    if( t> MAX_DIST)
      break;
    vec3 rabs = mix(absorb, vec3(0), (sgn + 1.) / 2.);
    vec3 beerlamb = exp(-rabs * t);
    vec3 p = ro + rd * t;
    vec3 n = sgn * normal(p);
    vec3 refl = reflect(rd, n);
    vec3 refr = refract(rd, n, cref);

    float fresnel = pow(1.0 - abs(dot(n, rd)), 2.6);
    float reflectorFactor = mix (0.2, 1., fresnel);
    float refractionFactor = mix (transp, 0., fresnel);
     
    col += (1. - refractionFactor) * rel * beerlamb * getSkyColor(refl) * refcolor * reflectorFactor;
    rel *= refractionFactor * beerlamb;     
        
    ro = p;     
    if (refr == vec3(0.0))
      rd = refl;
    else
    {
      rd = refr; 
      sgn *= -1.;
      cref = 1.2 / cref;
    }        
  }

  return col;
}

mat3
setCamera (vec3 ro, vec3 ta )
{
  vec3 cw = normalize(ta-ro);
  vec3 up = vec3(0, 1, 0);
  vec3 cu = normalize( cross(cw,up) );
  vec3 cv = normalize( cross(cu,cw) );

  return mat3( cu, cv, cw );
}

vec3
vignette (vec3 color, vec2 q, float v)
{
  color *= 0.3 + 0.8 * pow(16.0 * q.x * q.y * (1.0 - q.x) * (1.0 - q.y), v);

  return color;
}

vec3
desaturate (vec3 c, float a)
{
  float l = dot(c, vec3(1. / 3.));

  return mix(c, vec3(l), a);
}

void
main ()
{
  vec3 tot = vec3(0.0);

#ifdef AA
  vec2 rook[4];
  rook[0] = vec2( 1./8., 3./8.);
  rook[1] = vec2( 3./8.,-1./8.);
  rook[2] = vec2(-1./8.,-3./8.);
  rook[3] = vec2(-3./8., 1./8.);
  for( int n=0; n<4; ++n )
  {
    vec2 o = rook[n];
    vec2 p = (-iResolution + 2.0*(gl_FragCoord.xy+o))/iResolution.y;
#else //AA
    vec2 p = (-iResolution + 2.0*gl_FragCoord.xy)/iResolution.y;
#endif //AA
 
    float theta	= radians(360.)*(iMouse.x/iResolution.x-0.5) + iTime*.2;
    float phi	= radians(90.)*(iMouse.y/iResolution.y-0.5)-1.;
    vec3 ro = 4.*vec3( sin(phi)*cos(theta),cos(phi),sin(phi)*sin(theta));
    vec3 ta = vec3( 0 );
    mat3 ca = setCamera( ro, ta );

    vec3 rd =  ca*normalize(vec3(p,.8));        

    vec3 col;
    float cbase = .8;
    float cvar = 0.02;
    col.r = Render(ro , rd, cbase - cvar).r;
    col.g = Render(ro , rd, cbase).g;
    col.b = Render(ro , rd, cbase + cvar).b;

    tot += col;
#ifdef AA
  }
  tot /= 2.;
#endif

#if GAMMA
  tot = pow(tot, vec3(1. / 2.2));
#endif

  tot = vignette(tot, gl_FragCoord.xy / iResolution, 1.2);

  gl_FragColor = vec4( sqrt(tot), 1.0 );
}
