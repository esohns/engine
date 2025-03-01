uniform vec2 iResolution;
uniform float iTime;

#define TRIANGLE_PATTERN
#define GRASS

mat2 rot2(in float a){ float c = cos(a), s = sin(a); return mat2(c, -s, s, c); }
float hash21(vec2 p){ return fract(sin(dot(p, vec2(141.13, 289.97)))*43758.5453); }


vec2
hash22 (vec2 p)
{
  float n = sin(dot(p, vec2(41, 289)));
  //return fract(vec2(262144, 32768)*n)*2. - 1.; 
    
  p = fract(vec2(262144, 32768)*n);
  return sin(p*6.2831853 + iTime); 
}

float
n2D3G (vec2 p)
{
  vec2 i = floor(p); p -= i;
    
  vec4 v;
  v.x = dot(hash22(i), p);
  v.y = dot(hash22(i + vec2(1, 0)), p - vec2(1, 0));
  v.z = dot(hash22(i + vec2(0, 1)), p - vec2(0, 1));
  v.w = dot(hash22(i + 1.), p - 1.);

#if 1
  p = p*p*p*(p*(p*6. - 15.) + 10.);
#else
  p = p*p*(3. - 2.*p);
#endif

  return mix(mix(v.x, v.y, p.x), mix(v.z, v.w, p.x), p.y);
  //return v.x + p.x*(v.y - v.x) + p.y*(v.z - v.x) + p.x*p.y*(v.x - v.y - v.z + v.w);
}

float isoFunction (vec2 p) { return n2D3G(p/4. + .07); }

float
distLine (vec2 a, vec2 b)
{
	b = a - b;
	float h = clamp(dot(a, b)/dot(b, b), 0., 1.);
  return length(a - b*h);
}

float
distEdge (vec2 a, vec2 b)
{
  return dot((a + b)*.5, normalize((b - a).yx*vec2(-1, 1)) );
}

vec2
inter (vec2 p1, vec2 p2, float v1, float v2, float isovalue)
{
  return mix(p1, p2, (isovalue - v1)/(v2 - v1)*.75 + .25/2.); 
}

int
isoLine (vec3 n3, vec2 ip0, vec2 ip1, vec2 ip2, float isovalue, float i, inout vec2 p0, inout vec2 p1)
{
  p0 = vec2(1e5), p1 = vec2(1e5);

  int iTh = 0;
  if(n3.x>isovalue) iTh += 4;
  if(n3.y>isovalue) iTh += 2;
  if(n3.z>isovalue) iTh += 1;

  if(iTh == 1 || iTh == 6)
  {
    p0 = inter(ip1, ip2, n3.y, n3.z, isovalue); // Edge two.
    p1 = inter(ip2, ip0, n3.z, n3.x, isovalue); // Edge three.
  }
  else if(iTh == 2 || iTh == 5)
  {
    p0 = inter(ip0, ip1, n3.x, n3.y, isovalue); // Edge one.
    p1 = inter(ip1, ip2, n3.y, n3.z, isovalue); // Edge two.
  }
  else if(iTh == 3 || iTh == 4)
  {
    p0 = inter(ip0, ip1, n3.x, n3.y, isovalue); // Edge one.
    p1 = inter(ip2, ip0, n3.z, n3.x, isovalue); // Edge three.       
  }

  if(iTh>=4 && iTh<=6){ vec2 tmp = p0; p0 = p1; p1 = tmp; }
  if(i == 0.){ vec2 tmp = p0; p0 = p1; p1 = tmp; }

  return iTh;
}

vec3
simplexContour (vec2 p)
{
  const float gSc = 8.;
  p *= gSc;

  vec2 oP = p;

  p += vec2(n2D3G(p*3.5), n2D3G(p*3.5 + 7.3))*.015;

  vec2 s = floor(p + (p.x + p.y)*.36602540378);

  p -= s - (s.x + s.y)*.211324865;

  float i = p.x < p.y? 1. : 0.;
  vec2 ioffs = vec2(1. - i, i);

  vec2 ip0 = vec2(0), ip1 = ioffs - .2113248654, ip2 = vec2(.577350269); 

  vec2 ctr = (ip0 + ip1 + ip2)/3.;
  ip0 -= ctr; ip1 -= ctr; ip2 -= ctr; p -= ctr;

  vec3 n3;
  n3.x = isoFunction(s);
  n3.y = isoFunction(s + ioffs);
  n3.z = isoFunction(s + 1.);

  float d = 1e5, d2 = 1e5, d3 = 1e5, d4 = 1e5, d5 = 1e5; 

  float isovalue = 0.;

  vec2 p0, p1; 

  int iTh = isoLine(n3, ip0, ip1, ip2, isovalue, i, p0, p1);

  d = min(d, distEdge(p - p0, p - p1)); 

  if (iTh == 7)
    d = 0.;

  d3 = min(d3, distLine((p - p0), (p - p1))); 

  d4 = min(d4, min(length(p - p0), length(p - p1))); 

  float tri = min(min(distLine(p - ip0, p - ip1), distLine(p - ip1, p - ip2)), distLine(p - ip2, p - ip0));

  d5 = min(d5, tri);

  d5 = min(d5, length(p) - .02);   
    
#ifdef TRIANGULATE_CONTOURS
  vec2 oldP0 = p0;
  vec2 oldP1 = p1;

  float td = (iTh>0 && iTh<7)? 1. : 0.;
    
  if(iTh==3 || iTh==5 || iTh==6)
  {
    vec2 pt = p0;
    if(i==1.) pt = p1;
    d5 = min(d5, distLine((p - pt), (p - ip0))); 
    d5 = min(d5, distLine((p - pt), (p - ip1)));  
    d5 = min(d5, distLine((p - pt), (p - ip2))); 
  }
#endif

  isovalue = -.15;

  int iTh2 = isoLine(n3, ip0, ip1, ip2, isovalue, i, p0, p1);

  d2 = min(d2, distEdge(p - p0, p - p1)); 

  float oldD2 = d2;

  if (iTh2 == 7)
    d2 = 0.; 
  if (iTh == 7)
    d2 = 1e5;
  d2 = max(d2, -d);

  d3 = min(d3, distLine((p - p0), (p - p1)));

  d4 = min(d4, min(length(p - p0), length(p - p1))); 

  d4 -= .075;
  d3 -= .0125;

#ifdef TRIANGULATE_CONTOURS
  float td2 = (iTh2>0 && iTh2<7)? 1. : 0.;

  if(td==1. && td2==1.)
  {
    d5 = min(d5, distLine(p - p0, p - oldP0)); 
    d5 = min(d5, distLine(p - p0, p - oldP1));  
    d5 = min(d5, distLine(p - p1, p - oldP1));
         
    if(oldD2>0.)
    {
      vec2 pt = p0;
      if(i==1.)
        pt = p1;
      d5 = min(d5, distLine(p - pt, p - ip0)); 
      d5 = min(d5, distLine(p - pt, p - ip1));  
      d5 = min(d5, distLine(p - pt, p - ip2)); 
    }
  }   
  else if(td==1. && td2==0.)
  {
    vec2 pt = oldP0;
    if(i==1.) pt = oldP1;
    d5 = min(d5, distLine(p - pt, p - ip0)); 
    d5 = min(d5, distLine(p - pt, p - ip1));  
    d5 = min(d5, distLine(p - pt, p - ip2)); 
  }
  else if (td==0. && td2==1.)
  {
    vec2 pt = p0;
    if(i==1.) pt = p1;
    d5 = min(d5, distLine(p - pt, p - ip0)); 
    d5 = min(d5, distLine(p - pt, p - ip1));  
    d5 = min(d5, distLine(p - pt, p - ip2));  
  }
#endif

  d /= gSc;
  d2 /= gSc;
  d3 /= gSc;
  d4 /= gSc;    
  d5 /= gSc; 

  vec3 col = vec3(1, .85, .6);

  float sf = .004; 

  if(d>0. && d2>0.)
    col = vec3(1, 1.8, 3)*.45;
  if(d>0.)
    col = mix(col, vec3(1, 1.85, 3)*.3, (1. - smoothstep(0., sf, d2 - .012)));

  col = mix(col, vec3(1.1, .85, .6),  (1. - smoothstep(0., sf, d2)));

  col = mix(col, vec3(1.5, .9, .6)*.6, (1. - smoothstep(0., sf, d - .012)));

#ifdef GRASS
  col = mix(col, vec3(1, .8, .6)*vec3(.7, 1., .75)*.95, (1. - smoothstep(0., sf, d))); 
#else
  col = mix(col, vec3(1, .82, .6)*.95, (1. - smoothstep(0., sf, d))); 
#endif

  if(d2>0.)
    col *= (abs(dot(n3, vec3(1)))*1.25 + 1.25)/2.;
  else
    col *= max(2. - (dot(n3, vec3(1)) + 1.45)/1.25, 0.);

  //if(iTh!=0) col *= float(iTh)/7.*.5 + .6;
  //else col *= float(3.)/7.*.5 + .75;

#ifdef TRIANGULATE_CONTOURS
  //if(td==1. || td2==1.) col *= vec3(1, .4, .8); 
#endif
    
#ifdef TRIANGLE_PATTERN
  float pat = abs(fract(tri*12.5 + .4) - .5)*2.;
  col *= pat*.425 + .75; 
#endif

  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d5))*.95);

  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d3)));

  col = mix(col, vec3(0), (1. - smoothstep(0., sf, d4)));
  col = mix(col, vec3(1), (1. - smoothstep(0., sf, d4 + .005)));

  vec2 q = oP*1.5;
  col = min(col, 1.);
  float gr = sqrt(dot(col, vec3(.299, .587, .114)))*1.25;
  float ns = (n2D3G(q*4.*vec2(1./3., 3))*.64 + n2D3G(q*8.*vec2(1./3., 3))*.34)*.5 + .5;
  ns = gr - ns;
  q *= rot2(3.14159/3.);
  float ns2 = (n2D3G(q*4.*vec2(1./3., 3))*.64 + n2D3G(q*8.*vec2(1./3., 3))*.34)*.5 + .5;
  ns2 = gr - ns2;
  ns = smoothstep(0., 1., min(ns, ns2)); // Rough pencil sketch layer.
  col = mix(col, col*(ns + .35), .4);
  //col *= vec3(.8)*ns + .5;    
  //col = softLight(col, vec3(ns)*.75);
  //col = vec3(ns);

  //oP = floor(oP/gSc*1024.);
  //vec3 rn3 = vec3(hash21(oP), hash21(oP + 2.37), hash21(oP + 4.83));
  //col *= .9 + .1*rn3.xyz  + .1*rn3.xxx;

  return col;
}

void
main ()
{
	vec2 uv = (gl_FragCoord.xy - iResolution.xy*.5)/min(650., iResolution.y);

  vec2 p = rot2(3.14159/12.)*uv + vec2(.8660254, .5)*iTime/16.; 
    
  vec3 col = simplexContour(p);

  uv = gl_FragCoord.xy/iResolution.xy;
  col *= pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .0625) + .1;
  //col = mix(col.zyx/2., col, pow(16.*uv.x*uv.y*(1. - uv.x)*(1. - uv.y) , .125));

  gl_FragColor = vec4(sqrt(max(col, 0.)), 1);
}
