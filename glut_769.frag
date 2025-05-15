uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iMouse;

#define PI 3.14159265
#define MAT_PLANE 3.     
#define MAT_TREE  5.
//#define DEBUG

mat4
Ry (float angle)
{
  float c = cos(angle);
  float s = sin(angle);
    
  return mat4(
    vec4(c, 0, -s, 0),
    vec4(0, 1, 0, 0),
    vec4(s, 0, c, 0),
    vec4(0, 0, 0, 1)); 
}

mat4
Rz (float angle)
{
  float c = cos(angle);
  float s = sin(angle);
    
  return mat4(
    vec4(c, s, 0, 0),
    vec4(-s, c, 0, 0),
    vec4(0, 0, 1, 0),
    vec4(0, 0, 0, 1)); 
}

mat4
Disp (vec3 displacement)
{
  return mat4(
    vec4(1, 0, 0, 0),
    vec4(0, 1, 0, 0),
    vec4(0, 0, 1, 0),
    vec4(displacement, 1)); 
}

float
sdSphere (vec3 p, float s)
{
  return length(p)-s;
}

vec2
opU (float d1, float d2, float m1, float m2)
{
  return (d1<d2) ? vec2(d1, m1) : vec2(d2, m2);
}

float
sdCappedCylinder (vec3 p, vec2 h)
{
  p -= vec3(0.,h.y, 0);
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float
sdPlane (vec3 p)
{
  return p.y;
}

float
c_t (vec3 pt, float x1, float x2, float x3)
{
  mat4 posR = Rz(-(25.7/360.)*2.*PI);
  mat4 negR = Rz(25.7/360.*2.*PI);
  mat4 bendP = Ry(25.7/360.*2.*PI);
  mat4 bendR = Ry(-25.7/360.*2.*PI);

  const int depth = 7;
  const int branches = 3; 
  float len = 1.5;
  float wid = .05;
  float widf= .9;

  float trunk = sdCappedCylinder(pt-vec3(0.,0., 0.), vec2(wid,len));
  float d = trunk;
  float x = sdSphere((Disp(vec3(0.,-2.5*len,0.))*vec4(pt, 1.)).xyz,1.8*len);
  if (x > 2.*len/2.) return min(x,d);

  vec3 pt_n = pt;
  for (int i = 1; i <= depth; ++i)
  {
    wid *= widf;
    float l = len*pow(.5,float(i));

    mat4 mx1 = Rz(-0.2*sin(iTime+6.2))*posR*bendP*Disp(vec3(0,-2.*l - l/2.,0));

    mat4 wind = Rz(0.2*sin(iTime+6.2));
    mat4 mx2 = wind*negR*bendP*Disp(vec3(0,-2.*l,0));

    wind = Rz(0.2*sin(iTime+1.));
    mat4 mx3 = wind*Disp(vec3(0,-4.*l,0)) ;

    vec3 pt_1 = (mx1 * vec4(pt_n,1)).xyz;
    vec3 pt_2 = (mx2 * vec4(pt_n,1)).xyz;
    vec3 pt_3 = (mx3 * vec4(pt_n,1)).xyz;

    float z1 = sdSphere((Disp(vec3(0.,-x1*l,0.))*vec4(pt_1, 1.)).xyz,2.5*l);
    float z2 = sdSphere((Disp(vec3(0.,-x2*l,0.))*vec4(pt_2, 1.)).xyz,2.5*l);
    float z3 = sdSphere((Disp(vec3(0.,-x3*l,0.))*vec4(pt_3, 1.)).xyz,2.5*l);

    float y1= sdCappedCylinder(pt_1, vec2(wid,l));
    float y2= sdCappedCylinder(pt_2, vec2(wid,l));
    float y3= sdCappedCylinder(pt_3, vec2(wid,l));

    float mi = min(z1, min(z2,z3));

    vec3 pt = (z1<z2) ? pt_1 : pt_2;
    pt_n = (min(z1,z2)<z3) ? pt   : pt_3;

    d = min( d, min(y1,min(y2,y3)) );
    float epsilon = .5;
    #ifdef DEBUG
    epsilon = .0;
    #endif
    if (mi < epsilon) {continue;} 

    return min(mi,d);
  }

  return d; 
}

float
c (vec3 pt)
{
  float y1 = c_t(pt, 3., 3., 2.);
  return y1;
}

vec2
scene (vec3 ray)  
{
  vec2 objs = opU(c(ray), sdPlane(ray), MAT_TREE, MAT_PLANE);
  return objs ;
}

float
softshadow (vec3 ro, vec3 rd, float mint, float tmax)
{
  float res = 1.0;
  float t = mint;
  for( int i=0; i<50; i++ )
  {
    float h = scene( ro + rd*t ).x;
    res = min( res, 8.0*h/t );
    t += clamp( h, 0.02, 0.10 );
    if( h<0.01 || t>tmax )
      break;
  }

  return clamp( res, 0.0, 1.0 );
}

vec2
trace (vec3 ray, vec3 viewportxy)
{
  vec3 p = vec3(0);
  float t = 0.0;
  float tmax = 21.0;
  float tmin = .1;
  float m = 0.0;
  for (int i = 0; i < 70; ++i)
  {	
    p = viewportxy + (t* ray);
    vec2 data = scene(p);
    float dist = data.x;
    m  = data.y;
    t += dist;

    if ( (t > tmax) || ((t < tmin) && (t > 0.)))
      break;
  }
  if( t>tmax ) m=-1.0;
  return vec2(t, m);
}

vec3
calcNormal (vec3 pos)
{
  vec3 eps = vec3( 0.001, 0.0, 0.0 );
  vec3 nor = vec3(
    scene(pos+eps.xyy).x - scene(pos-eps.xyy).x,
    scene(pos+eps.yxy).x - scene(pos-eps.yxy).x,
    scene(pos+eps.yyx).x - scene(pos-eps.yyx).x );
  return normalize(nor);
}

mat4
LookAtRH (vec3 eye, vec3 target, vec3 up)
{
  vec3 zaxis = normalize(target-eye);      // The "forward" vector.
  vec3 xaxis = normalize(cross(up, zaxis));// The "right" vector.
  vec3 yaxis = cross(zaxis, xaxis);        // The "up" vector.

  return  mat4(
      vec4(      xaxis,       0 ),
      vec4(      yaxis,       0 ),
      vec4(      zaxis,       0 ),
      vec4(      eye,         1 ));
}

vec3
shade (vec3 pos, vec3 nrm, vec4 light)
{
  vec3 toLight = light.xyz - pos;

  float toLightLen = length( toLight );
  toLight = normalize( toLight );

  float comb = 0.1;
  float vis = softshadow( pos, toLight, 0.02, 2.5 );

  if ( vis > 0.0 )
  {
    float diff = 2.0 * max( 0.0, dot( nrm, toLight ) );
    float attn = 1.0 - pow( min( 1.0, toLightLen / light.w ), 2.0 );
    comb += diff * attn * vis;
  }

  return vec3( comb, comb, comb );
}

vec4
color (float t, float m, vec3 p)
{
  vec4 col = vec4(0.0);
  float f = mod( floor(p.z) + floor(p.x), 2.0);

  if (abs(p.x) > 10. || abs(p.z) > 10.)
    return vec4(0.);
  else if (abs(m -MAT_TREE) < .001)
  {
    vec4 brown = vec4(.8, .6, .45, 1.);
    vec4 green = vec4(.125,.49,.22,1.);
    col = mix(brown,green,p.y/10.);
  }
  else if( abs(m -MAT_PLANE) < .001 )
    col = vec4( 0.8 + 0.1*f*vec3(1.0), 1.);
  vec3 nrm = calcNormal(p);
  vec4 light1 = vec4(2., 8., 2.,   10.0 );
  vec4 light2 = vec4(-2., 8., -2., 10.0 );
  vec3 cc  = shade(p,nrm,light1);
  vec3 cc1 = shade(p,nrm,light2);

  return (cc.x+cc1.x)*col;
}

vec4
render (vec3 ray, vec3 pt)
{
  vec2 tm    = trace(ray, pt);
  float t    = tm.x;
  float mat  = tm.y;
  vec3 point = pt + (t * ray);
  return color(t, mat, point);
}

void
main ()
{
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  uv = uv * 2.0 - 1.0;
  uv.x *= iResolution.x / iResolution.y;
  vec2 mo = iMouse.xy/iResolution.xy;

  float f = 3.;
  vec4 pixel = vec4(uv, 0, 1);

  vec3 position = vec3(cos(iMouse.x/100. - .8)*10., 3., sin(iMouse.y/100.-.3)*10.);

  mat4 mat = LookAtRH(position, vec3(0,3,0), vec3(0,1,0));
  vec3 pt = (mat*pixel).xyz;
  vec3 ray = normalize(mat*(pixel - vec4(0,0,-f,1))).xyz;

  gl_FragColor = render(ray, pt);
}
